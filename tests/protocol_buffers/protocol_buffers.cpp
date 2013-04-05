// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <iostream>
#include <sstream>
#include <fstream>
#include <UnitTest++/UnitTest++.h>

#include <framework/protocol_buffers.hpp>

SUITE(framework_protocol_buffers_protocol_buffers_hpp)
{
    TEST(BasicMessageStream)
    {
        using namespace ::framework::serializable;
        using namespace ::framework::protocol_buffers;

        message <
            required <NAME("Field 1"), 1, int32>,
            repeated <NAME("Field 2"), 2, int32>,
            optional <NAME("Field 3"), 3, int32>,
            repeated <NAME("Group"), 4, group <
                required <NAME("Field 1"), 1, int32>,
                repeated <NAME("Field 2"), 2, int32>,
                optional <NAME("Field 3"), 3, int32>>>
        > out, in {1, std::forward_as_tuple(2, 3, 4), 5,
            std::forward_as_tuple(
                std::forward_as_tuple(6, std::forward_as_tuple(7, 8, 9), 10),
                std::forward_as_tuple(11, std::forward_as_tuple(12, 13, 14), 15),
                std::forward_as_tuple(16, std::forward_as_tuple(17, 18, 19), 20))};

        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in == out);
    }

    TEST(BasicMessageRaw)
    {
        using namespace ::framework::serializable;
        using namespace ::framework::protocol_buffers;

        message <
            required <NAME("Field 1"), 1, int32>,
            repeated <NAME("Field 2"), 2, int32>,
            optional <NAME("Field 3"), 3, int32>,
            repeated <NAME("Group"), 4, group <
                required <NAME("Field 1"), 1, int32>,
                repeated <NAME("Field 2"), 2, int32>,
                optional <NAME("Field 3"), 3, int32>>>
        > out, in {1, std::forward_as_tuple(2, 3, 4), 5,
            std::forward_as_tuple(
                std::forward_as_tuple(6, std::forward_as_tuple(7, 8, 9), 10),
                std::forward_as_tuple(11, std::forward_as_tuple(12, 13, 14), 15),
                std::forward_as_tuple(16, std::forward_as_tuple(17, 18, 19), 20))};

        size_frame size;
        CHECK(write(in, size));

        std::vector <char> buffer(static_cast <std::size_t> (size));
        char* wbegin = &buffer[0];
        char* wend = &buffer[buffer.size()];
        char const* rbegin = &buffer[0];
        char const* rend = &buffer[buffer.size()];

        CHECK(dispatch_write <decltype(in)> (in, wbegin, wend, wbegin));
        CHECK(dispatch_read <decltype(in)> (rbegin, rend, rbegin, out));
        CHECK(in == out);
    }

    TEST(ConsistentWrites)
    {
        using namespace ::framework::serializable;
        using namespace ::framework::protocol_buffers;

        message <
            required <NAME("Field 1"), 1, int32>,
            repeated <NAME("Field 2"), 2, int32>,
            optional <NAME("Field 3"), 3, int32>,
            repeated <NAME("Group"), 4, group <
                required <NAME("Field 1"), 1, int32>,
                repeated <NAME("Field 2"), 2, int32>,
                optional <NAME("Field 3"), 3, int32>>>
        > out, in {1, std::forward_as_tuple(2, 3, 4), 5,
            std::forward_as_tuple(
                std::forward_as_tuple(6, std::forward_as_tuple(7, 8, 9), 10),
                std::forward_as_tuple(11, std::forward_as_tuple(12, 13, 14), 15),
                std::forward_as_tuple(16, std::forward_as_tuple(17, 18, 19), 20))};

        size_frame size;
        CHECK(write(in, size));

        std::vector <char> buffer1(static_cast <std::size_t> (size));
        char* begin = &buffer1[0];
        char* end = &buffer1[buffer1.size()];

        std::vector <char> buffer2(static_cast <std::size_t> (size));
        raw_output_frame <true> frame {&buffer2[0], &buffer2[buffer2.size()]};

        CHECK(dispatch_write <decltype(in)> (in, begin, end, begin));
        CHECK(dispatch_write <decltype(in)> (in, frame));
        CHECK(buffer1 == buffer2);
    }
}
