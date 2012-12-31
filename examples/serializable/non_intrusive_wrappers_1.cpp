#include <fstream>
#include <cassert>
#include <vector>
#include <iostream>

#include <framework/serializable.hpp>
#include <framework/serializable/mutators/stl_array.hpp>

using ::framework::serializable::read;
using ::framework::serializable::write;
using ::framework::serializable::value_type;
using ::framework::serializable::alias;
using ::framework::serializable::extract_values;
using ::framework::serializable::little_endian;

// First approach - non-intrusive specification is used for serialization only.

// Value type requires an implementation template, provide a void alias
template <typename T>
using link_implementation = void;

// Define a value type used to link a data member to a particular specification
template <typename Type, Type Offset, typename Specification>
struct link : value_type <Type, Specification, link_implementation, false>
{
};

// Define a template to provide link with the required parameters
#define LINK(Member) decltype(&Member), &Member

// Implement the serialization of the 'link' data member
namespace framework
{
    namespace serializable
    {
        template <typename Type, Type Offset, typename Specification>
        struct serializable_specification <link <Type, Offset, Specification>>
        {
            template <typename Input, typename Output>
            static bool read (Input& in, Output& out)
            {
                if (!serializable_specification <Specification>::read(in, out.*Offset))
                    return false;

                return true;
            }

            template <typename Input, typename Output>
            static bool write (Input const& in, Output& out)
            {
                if (!serializable_specification <Specification>::write(in.*Offset, out))
                    return false;

                return true;
            }
        };
    }
}

// Define a template used to bind a specification to a particular structure
#define BIND(Specification, Structure) \
namespace framework \
{ \
    namespace serializable \
    { \
        template <> \
        struct serializable_specification <Structure> \
        { \
            template <typename Input> \
            static bool read (Input& in, Structure& out) \
            { \
                if (!serializable_specification <Specification>::read(in, out)) \
                    return false; \
\
                return true; \
            } \
\
            template <typename Output> \
            static bool write (Structure const& in, Output& out) \
            { \
                if (!serializable_specification <Specification>::write(in, out)) \
                    return false; \
\
                return true; \
            } \
        }; \
    } \
}

// Usage example

struct Object
{
    uint32_t x;
    uint32_t y;
    uint32_t z;
};

using ObjectSpecification = alias <
    link <LINK(Object::x), little_endian <uint32_t>>,
    link <LINK(Object::y), little_endian <uint32_t>>,
    link <LINK(Object::z), little_endian <uint32_t>>>;

BIND(ObjectSpecification, Object)

int main ()
{
    // Create an object
    Object o1 {1, 2, 3};

    // Write the object to a file
    assert(write(o1, std::ofstream("filename")));
    
    // Read the object from a file
    Object o2;
    assert(read(std::ifstream("filename"), o2));

    // Check the value
    assert(o1.x == o2.x);
    assert(o1.y == o2.y);
    assert(o1.z == o2.z);

    return 0;
}
