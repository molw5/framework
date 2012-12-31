#include <fstream>
#include <cassert>

#include <framework/serializable.hpp>
#include <framework/serializable/mutators/stl_string.hpp>

int main ()
{
    using ::framework::serializable::stl_wstring;
    using ::framework::serializable::little_endian;
    using ::framework::serializable::big_endian;
    using ::framework::serializable::value;
    
    // Define an object type
    using object = ::framework::serializable::inline_object <
        // Store the first field using little endian byte ordering
        value <NAME("Field 1"), little_endian <int32_t>>,

        // Store the second field using big endian byte ordering
        value <NAME("Field 2"), big_endian <uint64_t>>,

        // Store the wide string as a size-delimited container, storing the size and 
        // characters (see stl_wstring documentation) using little-endian byte ordering.
        value <NAME("Field 3"), stl_wstring <little_endian <uint32_t>>>>;

    // Create an object
    object o1 {1, 2, L"Hello World!"};

    // Write the object to a file
    assert(write(o1, std::ofstream("filename")));

    // Read the object from a file
    object o2;
    assert(read(std::ifstream("filename"), o2));

    // Check the value
    assert(o1 == o2);

    return 0;
}
