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
bool read_dispatch (s*, Input&& in, s& out)
{
    using framework::serializable::stream_read;
    return stream_read(in, reinterpret_cast <char*> (&out), sizeof(out));
}

template <typename Output>
bool write_dispatch (s*, s const& in, Output&& out)
{
    using framework::serializable::stream_write;
    return stream_write(out, reinterpret_cast <char const*> (&in), sizeof(in));
}

namespace framework{ 
    namespace serializable{
        template <
            typename Size,
            typename Input,
            typename Output>
        bool read_dispatch (
            stl_vector <Size, s>*,
            Input&& in, Output&& out)
        {
            type_extractor <Size> size;
            if (!dispatch_read <Size> (in, size))
                return false;

            out.resize(size);
            return stream_read(in, reinterpret_cast <char*> (&out[0]), out.size()*sizeof(out[0]));
        }

        template <
            typename Size,
            typename Input,
            typename Output>
        bool write_dispatch (
            stl_vector <Size, s>*,
            Input&& in, Output&& out)
        {
            type_extractor <Size> const& size = in.size();
            if (!dispatch_write <Size> (size, out))
                return false;

            return stream_write(out, reinterpret_cast <char const*> (&in[0]), in.size()*sizeof(in[0]));
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
