#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>

#include <framework/serializable.hpp>
#include <framework/serializable/mutators/stl_vector.hpp>

using ::framework::serializable::type_extractor;
using ::framework::serializable::mutator_type;

// Define the buffer mutator
template <typename SizeType>
struct buffer : mutator_type <
    // Define the child type, used in the definition of the underlying type.  In this
    // case the mutator defines the underlying type (std::vector) directly.
    std::vector <char>> 
{
};

// Read overload - in general these methods should avoid unnecessary restrictions on the
// input types.  For example, stl_vector <int, int> should accept any object that behaves
// like a vector, not just type_extractor <stl_vector <int, int>>.
template <
    typename Size,
    typename Input,
    typename Output>
bool read_dispatch (
    buffer <Size>*,
    Input&& in, Output&& out)
{
    using ::framework::serializable::dispatch_read;
    using ::framework::serializable::stream_read;

    // Read the buffer's size
    type_extractor <Size> size;
    if (!dispatch_read <Size> (in, size))
        return false;

    // Read the buffer's data
    out.resize(size);
    return stream_read(in, reinterpret_cast <char*> (&out[0]), out.size());
}

// Write overload
template <
    typename Size,
    typename Input,
    typename Output>
bool write_dispatch (
    buffer <Size>*,
    Input&& in, Output&& out)
{
    using ::framework::serializable::dispatch_write;
    using ::framework::serializable::stream_write;

    // Write the buffer's size
    type_extractor <Size> const& size = in.size();
    if (!dispatch_write <Size> (size, out))
        return false;

    // Write the buffer's data
    return stream_write(out, reinterpret_cast <char const*> (&in[0]), in.size());
}

int main ()
{
    using ::framework::serializable::inline_object;
    using ::framework::serializable::big_endian;
    using ::framework::serializable::little_endian;
    using ::framework::serializable::value;
    
    // Define an object type
    using object = inline_object <
        value <NAME("Field 1"), little_endian <int32_t>>,
        value <NAME("Field 2"), little_endian <double>>,
        value <NAME("Field 3"), buffer <little_endian <uint32_t>>>>;

    // Create an object
    object o1 {1, 2.0,
        // Note: Arguments unpacked by the field's in-place constructor.
        //       Constructed as: vector <uint8_t> {'H', 'e', ...}
        std::forward_as_tuple('H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!')};

    // Write the object to a file
    assert(write(o1, std::ofstream("filename")));

    // Read the object from a file
    object o2;
    assert(read(std::ifstream("filename"), o2));

    // Check the result
    assert(o1 == o2);

    return 0;
}
