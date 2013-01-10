#include <fstream>
#include <cassert>
#include <vector>
#include <iostream>

#include <framework/serializable.hpp>
#include <framework/serializable/mutators/stl_array.hpp>

using ::framework::pack_container;
using ::framework::merge_packs;
using ::framework::serializable::equal;
using ::framework::serializable::comparable;
using ::framework::serializable::inline_object;
using ::framework::serializable::stl_array;
using ::framework::serializable::alias;
using ::framework::serializable::value;
using ::framework::serializable::serializable_implementation;
using ::framework::serializable::extract_values;
using ::framework::serializable::container_specification;
using ::framework::serializable::container_children;

// Extracts the smallest element of a pack_container
template <typename Args>
struct minimum;

template <typename Head, typename... Tail>
struct minimum <pack_container <Head, Tail...>>
{
    using next = minimum <pack_container <Tail...>>;
    enum { smallest = sizeof(inline_object <Head>) < sizeof(inline_object <typename next::type>) };

    using type = 
        typename std::conditional <
            smallest,
            Head,
            typename next::type
        >::type;
    
    using remaining =
        typename std::conditional <
            smallest,
            merge_packs <pack_container <typename next::type>, typename next::remaining>,
            merge_packs <pack_container <Head>, typename next::remaining>
        >::type;
};

template <typename Head>
struct minimum <pack_container <Head>>
{
    using type = Head;
    using remaining = pack_container <>;
};

// Sorts a list of types by size
template <typename Args>
struct sort_by_size
{
    using current = minimum <Args>;
    using type = merge_packs <pack_container <typename current::type>,
            typename sort_by_size <typename current::remaining>::type>;
};

template <>
struct sort_by_size <pack_container <>>
{
    using type = pack_container <>;
};

// Adds a new serializable_implementation
template <typename Derived, typename... Specification>
using packed_serializable = ::framework::serializable::serializable_implementation <
    // Derived class name and specification are unchanged
    Derived,
    alias <Specification...>,
    
    // The list of inherited types is sorted by size.  A better implementation could
    // solve the associated bin-packing problem.
    typename sort_by_size <
        extract_values <alias <Specification...>, container_specification>
    >::type,

    // The constructed and visible types are unchanged
    extract_values <alias <Specification...>, container_children>,
    extract_values <alias <Specification...>, container_children>>;

template <typename... Types>
class packed_object final : 
    public comparable <packed_object <Types...>>,
    public packed_serializable <packed_object <Types...>, Types...>
{
    private:
        DEFINE_BASE_TEMPLATE(packed_object <Types...>);

    public:
        // Define an interface equivalent to inline_object
        
        packed_object () = default;

        template <typename... Args>
        packed_object (Args&&... args)
            : packed_serializable <packed_object <Types...>, Types...> (std::forward <Args> (args)...)
        {
        }

        template <typename Name, typename... Args>
        auto get (Args&&... args) const ->
        decltype(std::declval <base <Name>> ().get(std::forward <Args> (args)...))
        {
            return base <Name>::get(std::forward <Args> (args)...);
        }

        template <typename Name, typename... Args>
        auto set (Args&&... args) ->
        decltype(std::declval <base <Name>> ().set(std::forward <Args> (args)...))
        {
            return base <Name>::set(std::forward <Args> (args)...);
        }

        template <typename Name, typename... Args>
        auto check (Args&&... args) const ->
        decltype(std::declval <base <Name>> ().check(std::forward <Args> (args)...))
        {
            return base <Name>::check(std::forward <Args> (args)...);
        }

        template <typename Name, typename... Args>
        auto clear (Args&&... args) ->
        decltype(std::declval <base <Name>> ().clear(std::forward <Args> (args)...))
        {
            return base <Name>::clear(std::forward <Args> (args)...);
        }
};

int main ()
{
    // Define an object type
    using test_structure = alias <
        value <NAME("Field 1"), uint16_t>,
        value <NAME("Field 2"), uint32_t>,
        value <NAME("Field 3"), uint8_t>,
        value <NAME("Field 4"), uint64_t>,
        value <NAME("Field 5"), uint8_t>,
        value <NAME("Field 6"), stl_array <3, uint8_t>>,
        value <NAME("Field 7"), uint16_t>,
        value <NAME("Field 8"), uint8_t>,
        value <NAME("Field 9"), uint32_t>>;

    // The packed container is compatible with the original definition, as demonstrated below
    
    // Create an object, using the original object layout
    inline_object <test_structure> o1 {1, 2, 3, 4, 5, std::array <uint8_t, 3> {{6, 7, 8}}, 9, 10, 11};

    // Write the object to a file
    assert(write(o1, std::ofstream("filename")));
    
    // Read the object from a file using the packed object layout
    packed_object <test_structure> o2;
    assert(read(std::ifstream("filename"), o2));

    // Check the value
    assert(equal(o1, o2));

    // Output the original and packed sizes
    std::cout << "Inline object size: " << sizeof(o1) << std::endl;
    std::cout << "Packed object size: " << sizeof(o2) << std::endl;

    return 0;
}
