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

template <typename Input>
bool custom_read (Input& in, s& out)
{
    if (!in.read(reinterpret_cast <char*> (&out), sizeof(out)))
        return false;

    return true;
}

template <typename Output>
bool custom_write (s const& in, Output& out)
{
    if (!out.write(reinterpret_cast <char const*> (&in), sizeof(in)))
        return false;

    return true;
}

namespace framework
{
    namespace serializable
    {
        // Specialize the serialization of a mutator type
        template <typename Input, typename Size>
        bool read (Input& in, type_extractor <stl_vector <Size, s>>& out, stl_vector <Size, s>*)
        {
            type_extractor <Size> size;
            if (!read <Size> (in, size))
                return false;

            out.resize(size);
            if (!in.read(reinterpret_cast <char*> (&out[0]), out.size()*sizeof(out[0])))
                return false;

            return true;
        }
    
        template <typename Output, typename Size>
        bool write (type_extractor <stl_vector <Size, s>> const& in, Output& out, stl_vector <Size, s>*)
        {
            type_extractor <Size> const& size = in.size();
            if (!write <Size> (size, out))
                return false;

            if (!out.write(reinterpret_cast <char const*> (&in[0]), in.size()*sizeof(in[0])))
                return false;

            return true;
        }
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
