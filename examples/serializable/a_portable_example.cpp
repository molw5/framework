#include <fstream>
#include <cassert>

#include <framework/serializable.hpp>
#include <framework/serializable/mutators/stl_string.hpp>

using ::framework::serializable::stl_wstring;
using ::framework::serializable::little_endian;
using ::framework::serializable::big_endian;
using ::framework::serializable::value;

using object = ::framework::serializable::inline_object <
    value <NAME("Field 1"), little_endian <int32_t>>,
    value <NAME("Field 2"), big_endian <double>>,
    value <NAME("Field 3"), stl_wstring <little_endian <uint32_t>>>>;

int main ()
{
    // Create an object
    object o1 {1, 2.0, L"Hello World!"};

    // Write the object to a file
    assert(write(o1, std::ofstream("filename")));

    // Read the object from a file
    object o2;
    assert(read(std::ifstream("filename"), o2));

    // Check the value
    assert(o1 == o2);

    return 0;
}
