#include <fstream>
#include <cassert>

#include <framework/protocol_buffers.hpp>

int main ()
{
    using ::framework::protocol_buffers::message;
    using ::framework::protocol_buffers::required;
    using ::framework::protocol_buffers::optional;
    using ::framework::protocol_buffers::repeated;
    using ::framework::protocol_buffers::int32;
    using ::framework::protocol_buffers::string_;

    // Define an object type
    using object = message <
        required <NAME("Field 1"), 1, int32>,
        optional <NAME("Field 2"), 2, string_>,
        repeated <NAME("Field 3"), 3, int32>>;

    // Create an object
    object o1 {1, "Hello World!", std::forward_as_tuple(2, 3, 4)};

    // Write the object to a file
    assert(write(o1, std::ofstream("filename")));

    // Read the object from a file
    object o2;
    assert(read(std::ifstream("filename"), o2));

    // Check the result
    assert(o1 == o2);

    return 0;
}
