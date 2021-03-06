#include <fstream>
#include <cassert>
#include <vector>

#include <framework/serializable.hpp>
#include <framework/serializable/mutators/stl_vector.hpp>

using ::framework::pack_container;
using ::framework::merge_packs;
using ::framework::serializable::container_type;

// Extracts the I'th element of a pack_container
template <unsigned int I, typename Args>
struct extract;

template <unsigned int I, typename Head, typename... Tail>
struct extract <I, pack_container <Head, Tail...>>
{
    using next = extract <I-1, pack_container <Tail...>>;
    using type = typename next::type;
    using remaining = merge_packs <pack_container <Head>, typename next::remaining>;
};

template <typename Head, typename... Tail>
struct extract <0, pack_container <Head, Tail...>>
{
    using type = Head;
    using remaining = pack_container <Tail...>;
};

// Randomizes a list of types using a simple LCG random number generator and
// the provided seed.
template <unsigned int Seed, typename Args>
struct randomize
{
    using current = extract <Seed % Args::size, Args>;
    using type = 
        merge_packs <
            pack_container <typename current::type>,
            typename randomize <
                static_cast <unsigned int> (1664525*Seed + 1013904223), 
                typename current::remaining
            >::type>;
};

template <unsigned int Seed>
struct randomize <Seed, pack_container <>>
{
    using type = pack_container <>;
};

// Randomizes the serialization order and layout in memory (generally, use of the
// \c serializable alias here is assumed here and below).
template <int Seed, typename... Args>
struct random_order : container_type <
    // Randomize the order in the specification; used to specify the object layout
    // and serialization order
    typename randomize <Seed, pack_container <Args...>>::type,

    // Retain the original logical order - constructors/comparison operations
    // continue to use the original order
    pack_container <Args...>>
{
};

#include <iostream>
int main ()
{
    using ::framework::serializable::inline_object;
    using ::framework::serializable::big_endian;
    using ::framework::serializable::little_endian;
    using ::framework::serializable::value;

    // Define an object type
    using object = inline_object <
        // Randomizes the serialization order and layout in memory of the contained fields
        // using the seed 28198236
        random_order <28198236,
            value <NAME("Field 1"), little_endian <uint32_t>>,
            value <NAME("Field 2"), little_endian <uint32_t>>,
            value <NAME("Field 3"), little_endian <uint32_t>>,
            value <NAME("Field 4"), little_endian <uint32_t>>,
            value <NAME("Field 5"), little_endian <uint32_t>>>>;

    // Create an object
    object o1 {1, 2, 3, 4, 5};

    // Note - initialization order is not affected
    {
        // Assert macro workaround
        auto const field_1 = o1.get <NAME("Field 1")> ();
        assert(field_1 == 1);
    }

    // Write the object to a file
    assert(write(o1, std::ofstream("filename")));
    
    // Read the object from a file
    object o2;
    assert(read(std::ifstream("filename"), o2));

    // Check the result
    assert(o1 == o2);

    // Read the fields directly
    using object_direct = inline_object <
        value <NAME("Field 1"), little_endian <uint32_t>>,
        value <NAME("Field 2"), little_endian <uint32_t>>,
        value <NAME("Field 3"), little_endian <uint32_t>>,
        value <NAME("Field 4"), little_endian <uint32_t>>,
        value <NAME("Field 5"), little_endian <uint32_t>>>;

    // Read the object from a file
    object_direct o3;
    assert(read(std::ifstream("filename"), o3));

    // Note - serialization order is affected
    {
        // Assert macro workaround
        auto const field_1 = o3.get <NAME("Field 1")> ();
        assert(field_1 == 2); // Note - seed dependent
    }

    return 0;
}
