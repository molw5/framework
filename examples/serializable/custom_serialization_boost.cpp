#include <fstream>
#include <stdexcept>
#include <cassert>

#include <framework/serializable.hpp>
#include <framework/serializable/mutators/stl_vector.hpp>

// Raw structure
struct s
{
    int x;
    int y;
    int z;

    bool operator== (s const& rhs) const
    {
        return x == rhs.x &&
               y == rhs.y &&
               z == rhs.z;
    }
};

// Define Boost.Serialize style operators for an arbitrary stream type.  In practice the 
// definition below are much too general, use_boost_syntax should assume that an appropriate 
// operator is provided by the underlying stream type.  Used here for the sake of brevity.
namespace boost_serialization_operators
{
    using ::framework::serializable::serializable_specification;

    template <typename Archive, typename T>
    Archive& operator& (Archive& ar, T& x)
    {
        if (!serializable_specification <T>::read(ar, x))
            throw std::runtime_error("Read failed");

        return ar;
    }

    template <typename Archive, typename T>
    Archive& operator& (Archive& ar, T const& x)
    {
        if (!serializable_specification <T>::write(x, ar))
            throw std::runtime_error("Write failed");

        return ar;
    }
}

// Redirect read/write calls to the provided serialize function
template <typename T>
struct use_boost_syntax
{
    template <typename Input, typename Output>
    static bool read (Input& in, Output& out)
    {
        return T::serialize(in, out);
    }

    template <typename Input, typename Output>
    static bool write (Input const& in, Output& out)
    {
        return T::serialize(out, in);
    }
};

namespace framework
{
    namespace serializable
    {
        // Use Boost.Serialize style syntax in the definition
        template <>
        struct serializable_specification <s> 
            : use_boost_syntax <serializable_specification <s>>
        {
            // Note: Object must be a template parameter here; 
            //       write expects a constant object type
            template <typename Archive, typename Object>
            static bool serialize(Archive& ar, Object& obj)
            {
                using namespace boost_serialization_operators;
                return ar & obj.x & obj.y & obj.z;
            }
        };
    }
}

int main ()
{
    using ::framework::serializable::inline_object;
    using ::framework::serializable::value;
    using ::framework::serializable::stl_vector;

    using object = inline_object <
        value <NAME("Field 1"), s>,
        value <NAME("Field 2"), stl_vector <uint32_t, s>>>;

    // Create an object
    object o1 {s{1, 2, 3}, 
        std::forward_as_tuple(
            s{4,   5,  6},
            s{7,   8 , 9},
            s{10, 11, 12})};

    // Write the object to a file
    assert(write(o1, std::ofstream("filename")));

    // Read the object from a file
    object o2;
    assert(read(std::ifstream("filename"), o2));

    // Check the value
    assert(o1 == o2);

    return 0;
}

