#include <fstream>
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

namespace framework
{
    namespace serializable
    {
        // Specify the serialization of s
        template <>
        struct serializable_specification <s>
        {
            template <typename Input>
            static bool read (Input& in, s& out)
            {
                if (!in.read(reinterpret_cast <char*> (&out), sizeof(out)))
                    return false;

                return true;
            }

            template <typename Output>
            static bool write (s const& in, Output& out)
            {
                if (!out.write(reinterpret_cast <char const*> (&in), sizeof(in)))
                    return false;

                return true;
            }
        };

        // Specialize the serialization of a mutator type
        template <typename Size>
        struct serializable_specification <stl_vector <Size, s>>
        {
            private:
                using size_type = typename type_extractor <Size>::type;
                using container_type = typename type_extractor <stl_vector <Size, s>>::type;

            public:
                template <typename Input>
                static bool read (Input& in, container_type& out)
                {
                    size_type size;
                    if (!serializable_specification <Size>::read(in, size))
                        return false;

                    out.resize(size);
                    if (!in.read(reinterpret_cast <char*> (&out[0]), out.size()*sizeof(out[0])))
                        return false;
    
                    return true;
                }
    
                template <typename Output>
                static bool write (container_type const& in, Output& out)
                {
                    if (!serializable_specification <Size>::write(in.size(), out))
                        return false;
                    if (!out.write(reinterpret_cast <char const*> (&in[0]), in.size()*sizeof(in[0])))
                        return false;
    
                    return true;
                }
        };
    }
}

int main ()
{
    using ::framework::serializable::inline_object;
    using ::framework::serializable::value;
    using ::framework::serializable::stl_vector;

    // Define an object type
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
