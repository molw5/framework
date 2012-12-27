#include <fstream>
#include <cassert>

#include <framework/serializable.hpp>
#include <framework/serializable/mutators/stl_string.hpp>

using ::framework::serializable::comparable;
using ::framework::serializable::serializable;
using ::framework::serializable::value;

// Define a new structure
struct object :
    // Provides the standard comparison operations
    comparable <object>,

    // Define the object's specification
    serializable <object,
        value <NAME("Field 1"), int>,
        value <NAME("Field 2"), double>>
{
    // Forward constructor arguments to the base class
    template <typename... Args>
    object (Args&&... args)
        : object::serializable_base(std::forward <Args> (args)...)
    {
    }

    // Defines a convenient 'base' alias for get_base
    DEFINE_BASE_TEMPLATE(object);

    // Example method
    void foo ()
    {
        using x1 = base <NAME("Field 1")>;
        using x2 = base <NAME("Field 2")>;

        if (x1::get() > 0)
            x2::set(x2::get() + 1.0);
        else
            x2::set(x2::get() - 1.0);
    }
};

int main ()
{
    // Create an object
    object o1 {1, 2.0};

    // Write the object to a file
    assert(write(o1, std::ofstream("filename")));

    // Read the object from a file
    object o2;
    assert(read(std::ifstream("filename"), o2));

    // Check the value
    assert(o1 == o2);

    return 0;
}
