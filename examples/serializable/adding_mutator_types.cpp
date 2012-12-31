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

// Define the serialization of the vector mutator - must be placed within serializable's namespace
namespace framework
{
    namespace serializable
    {
        // Note - the specialization uses the same syntax provided to mutator_type's specification parameter
        template <typename SizeType>
        struct serializable_specification <buffer <SizeType>>
        {
            private:
                // Define the types used by this mutator
                // Note: type_extractor is used here to define the underlying type of a specification.
                using value_type = typename type_extractor <buffer <SizeType>>::type;
                using size_type = typename type_extractor <SizeType>::type;
        
            public:
                // Read definition - note that the output type is restricted to the expected type here
                template <typename Input>
                static bool read (Input& in, value_type& out)
                {
                    // Read the buffer's size
                    size_type size;
                    if (!serializable_specification <SizeType>::read(in, size))
                        return false;
        
                    // Read the buffer's data
                    out.resize(size);
                    if (!in.read(reinterpret_cast <char*> (&out[0]), out.size()))
                        return false;
        
                    return true;
                }
        
                // Write definition - note that the input type is restricted to the expected type here
                template <typename Output>
                static bool write (value_type const& in, Output& out)
                {
                    // Write the buffer's size
                    if (!serializable_specification <SizeType>::write(in.size(), out))
                        return false;
        
                    // Write the buffer's data
                    if (!out.write(reinterpret_cast <char const*> (&in[0]), in.size()))
                        return false;
        
                    return true;
                }
        };
    }
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
