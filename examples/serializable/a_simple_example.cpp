#include <fstream>
#include <cassert>

#include <framework/serializable.hpp>
#include <framework/serializable/mutators/stl_string.hpp>

int main ()
{
    using ::framework::serializable::inline_object;
    using ::framework::serializable::stl_null_string;
    using ::framework::serializable::value;
    
    // Define an object type
    using object = inline_object <
        value <NAME("Field 1"), int32_t>,
        value <NAME("Field 2"), double>,
        value <NAME("Field 3"), stl_null_string>>;

    // Create an object
    object o1 {1, 2.0, "Hello World!"};

    // Write the object to a file
    assert(write(o1, std::ofstream("filename")));

    // Read the object from a file
    object o2;
    assert(read(std::ifstream("filename"), o2));

    // Check the result
    assert(o1 == o2);

    return 0;
}
