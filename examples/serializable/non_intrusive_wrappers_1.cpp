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
using ::framework::serializable::type_extractor;

// First approach - non-intrusive specification is used for serialization only.

// Value type requires an implementation template, provide a void alias
template <typename T>
using olink_implementation = void;

// Define a value type used to olink a data member to a particular specification
template <typename Type, Type Offset, typename Specification>
struct olink : value_type <Type, Specification, olink_implementation, false>
{
};

// Define a template to provide olink with the required parameters
#define LINK(Member) decltype(&Member), &Member

// Implement the serialization of the 'olink' data member
template <
    typename Type,
    Type Offset,
    typename Specification,
    typename Input,
    typename Output>
bool read_dispatch (
    olink <Type, Offset, Specification>*,
    Input&& in, Output&& out)
{
    using ::framework::serializable::dispatch_read;

    type_extractor <Specification> value;
    if (!dispatch_read <Specification> (in, value))
        return false;

    out.*Offset = std::move(value);
    return true;
}

template <
    typename Type,
    Type Offset,
    typename Specification,
    typename Input,
    typename Output>
bool write_dispatch (
    olink <Type, Offset, Specification>*,
    Input&& in, Output&& out)
{
    using ::framework::serializable::dispatch_write;

    type_extractor <Specification> const& value = in.*Offset;
    return dispatch_write <Specification> (value, out);
}

// Define a template used to bind a specification to a particular structure
#define BIND(Specification, Structure) \
template <typename Input, typename Output> \
bool read_dispatch (\
    Structure*, \
    Input&& in, Output&& out) \
{ \
    using ::framework::serializable::dispatch_read; \
    return dispatch_read <Specification> (in, out); \
} \
\
template <typename Input, typename Output> \
bool write_dispatch ( \
    Structure*, \
    Input&& in, Output&& out) \
{ \
    using ::framework::serializable::dispatch_write; \
    return dispatch_write <Specification> (in, out); \
}

// Usage example

struct Object
{
    uint32_t x;
    uint32_t y;
    uint32_t z;
};

using ObjectSpecification = alias <
    olink <LINK(Object::x), little_endian <uint32_t>>,
    olink <LINK(Object::y), little_endian <uint32_t>>,
    olink <LINK(Object::z), little_endian <uint32_t>>>;

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
