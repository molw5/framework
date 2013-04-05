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
using ::framework::serializable::serializable;
using ::framework::serializable::object_names;
using ::framework::serializable::inline_object;
using ::framework::serializable::extract_values;
using ::framework::serializable::little_endian;
using ::framework::serializable::comparable;
using ::framework::pack_container;
using ::framework::make_indices;

// Second approach - bind the object to a wrapper object

// Wrapper around an implementation template - forwards the appropriate
// template parameters
template <
    typename Type,
    Type Offset,
    typename Specification,
    template <typename> class Implementation>
struct link_implementation_wrapper
{
    template <typename Derived>
    struct parameters
    {
        using offset_type = Type;
        static constexpr Type offset_value {Offset};
        using value_specification = Specification;
        using derived = Derived;
    };

    template <typename Derived>
    using type = Implementation <parameters <Derived>>;
};

// The value type and class name of the data member pointer are required 
// here - extract them
template <typename T>
struct extract_pointer_types;

template <typename Type, typename Class>
struct extract_pointer_types <Type Class::*>
{
    using value_type = Type;
    using class_type = Class;
};

// Define the link implementation - used to bind a member variable of an object to 
// a serializable value type
template <typename T>
struct link_implementation
{
    private:
        using class_type = typename extract_pointer_types <typename T::offset_type>::class_type;
        using value_type = typename extract_pointer_types <typename T::offset_type>::value_type;

    public:
        value_type const& get () const { return p_tValue; }
        void set (value_type value) { p_tValue = std::move(value); }

    public:
        link_implementation (class_type& t)
            : p_tValue(t.*T::offset_value)
        {
        }

    private:
        value_type& p_tValue;
};

// As above, but binds a constant member variable to a serializable value type
template <typename T>
struct const_link_implementation
{
    private:
        using class_type = typename extract_pointer_types <typename T::offset_type>::class_type;
        using value_type = typename extract_pointer_types <typename T::offset_type>::value_type;

    public:
        value_type const& get () const { return p_tValue; }

    public:
        const_link_implementation (class_type const& t)
            : p_tValue(t.*T::offset_value)
        {
        }

    private:
        value_type const& p_tValue;
};

// We need to assign a unique typename to the value type; this will suffice
template <typename Type, Type Offset>
struct member_name
{
};

// Define a value type used to link a data member to a particular specification
template <
    typename Type,
    Type Offset,
    typename Specification,
    template <typename> class Implementation = link_implementation>
struct link : value_type <
    member_name <Type, Offset>,
    Specification,
    link_implementation_wrapper <
        Type,
        Offset,
        Specification,
        Implementation
    >::template type>
{
};

template <typename... Args>
struct bind_object : 
    public comparable <bind_object <Args...>>,
    public serializable <bind_object <Args...>, Args...>
{
    private:
        using value_names = object_names <inline_object <Args...>>;

    public:
        template <typename T>
        bind_object (T& t)
            : bind_object (t, static_cast <make_indices <value_names::size>*> (nullptr))
        {
        }

    private:
        template <typename T, typename I>
        struct passthrough
        {
            using type = T;
        };

        template <typename T, typename... Indices>
        bind_object (T& t, pack_container <Indices...>*)
            : serializable <bind_object <Args...>, Args...> (static_cast <typename passthrough <T, Indices>::type&> (t)...)
        {
        }
};

// Define a template to provide link with the required parameters
#define LINK(Member) decltype(&Member), &Member

// Define a template used to bind a specification to a particular structure
#define BIND(Specification, Structure) \
template <typename Input, typename Output> \
bool read_dispatch ( \
    Structure*, \
    Input&& in, Output&& out) \
{ \
    return read(std::forward <Input> (in), Specification <link_implementation> {out}); \
} \
\
template <typename Input, typename Output> \
bool write_dispatch ( \
    Structure*, \
    Input&& in, Output&& out) \
{ \
    return write(Specification <const_link_implementation> {in}, std::forward <Output> (out)); \
}

// Usage example

struct Object
{
    uint32_t x;
    uint32_t y;
    uint32_t z;
};

template <template <typename> class Implementation>
using ObjectSpecification = bind_object <
    link <LINK(Object::x), little_endian <uint32_t>, Implementation>,
    link <LINK(Object::y), little_endian <uint32_t>, Implementation>,
    link <LINK(Object::z), little_endian <uint32_t>, Implementation>>;

BIND(ObjectSpecification, Object)

// Basic check - is the compiler able to optimize out the wrapper class?
bool compare_control (Object const& lhs, Object const& rhs) __attribute__ ((noinline));
bool compare_test (Object const& lhs, Object const& rhs) __attribute__ ((noinline));

int main ()
{
    using wrapper = ObjectSpecification <link_implementation>;
    // Create an object
    Object o1 {1, 2, 3};

    // Write the object to a file
    assert(write(o1, std::ofstream("filename")));
    
    // Read the object from a file
    Object o2;
    assert(read(std::ifstream("filename"), o2));

    // Check the value
    assert(wrapper(o1) == wrapper(o2));

    // Perform the tests
    assert(compare_control(o1, o2));
    assert(compare_test(o1, o2));

    return 0;
}

// Comparison tests
bool compare_control (Object const& lhs, Object const& rhs)
{
    return lhs.x == rhs.x &&
           lhs.y == rhs.y &&
           lhs.z == rhs.z;
}

bool compare_test (Object const& lhs, Object const& rhs)
{
    using wrapper = ObjectSpecification <const_link_implementation>;
    return wrapper(lhs) == wrapper(rhs);
}
