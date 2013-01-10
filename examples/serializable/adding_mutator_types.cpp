#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>

#include <framework/serializable.hpp>
#include <framework/serializable/mutators/stl_vector.hpp>

using ::framework::serializable::type_extractor;
using ::framework::serializable::mutator_type;

// Define the buffer mutator - may be placed into an arbitrary namespace
template <typename SizeType>
struct buffer : mutator_type <
    // Define the child type, used in the definition of the underlying type.  In this
    // case the mutator defines the underlying type (std::vector) directly.
    std::vector <char>> 
{
};

// Read overload
template <typename Input, typename SizeType>
bool dispatch_read (Input& in, type_extractor <buffer <SizeType>>& out, buffer <SizeType>*)
{
    using ::framework::serializable::dispatch_read;

    // Read the buffer's size
    type_extractor <SizeType> size;
    if (!dispatch_read <SizeType> (in, size))
        return false;

    // Read the buffer's data
    out.resize(size);
    if (!in.read(reinterpret_cast <char*> (&out[0]), out.size()))
        return false;

    return true;
}
        
// Write overload
template <typename Output, typename SizeType>
bool dispatch_write (type_extractor <buffer <SizeType>> const& in, Output& out, buffer <SizeType>*)
{
    using ::framework::serializable::dispatch_write;

    // Write the buffer's size
    type_extractor <SizeType> const& size = in.size();
    if (!dispatch_write <SizeType> (size, out))
        return false;

    // Write the buffer's data
    if (!out.write(reinterpret_cast <char const*> (&in[0]), in.size()))
        return false;

    return true;
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
