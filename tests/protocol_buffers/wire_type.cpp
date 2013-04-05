// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <iostream>
#include <sstream>
#include <fstream>
#include <UnitTest++/UnitTest++.h>

#include <framework/protocol_buffers/wire_type.hpp>

template <typename T>
bool run_stream_test ()
{
    using framework::serializable::type_extractor;
    using framework::serializable::dispatch_read;
    using framework::serializable::dispatch_write;

    type_extractor <T> out, in {0};

    for (int i=0; i < 200; ++i)
    {
        in = (in * 2) + i;
        out = 0;

        std::stringstream ss;
        if (!dispatch_write <T> (in, ss))
            return false;
        if (!dispatch_read <T> (ss, out))
            return false;
        if (in != out)
            return false;
    }

    return true;
}

template <typename T>
bool run_raw_test ()
{
    using framework::serializable::type_extractor;
    using framework::serializable::dispatch_read;
    using framework::serializable::dispatch_write;

    type_extractor <T> out, in {0};

    for (int i=0; i < 200; ++i)
    {
        char buffer[10] = {0};
        in = (in * 2) + i;
        out = 0;

        {
            char* begin {&buffer[0]};
            char* end {&buffer[10]};
            if (!dispatch_write <T> (in, begin, end, begin))
                return false;
        }

        {
            char const* begin {&buffer[0]};
            char const* end {&buffer[10]};
            if (!dispatch_read <T> (begin, end, begin, out))
                return false;
        }

        if (in != out)
            return false;
    }

    return true;
}

SUITE(framework_protocol_buffers_varint_hpp)
{
    TEST(BasicNumericStream)
    {
        using namespace framework::protocol_buffers;

        CHECK(run_stream_test <float_> ());
        CHECK(run_stream_test <double_> ());
        CHECK(run_stream_test <bool_> ());
        CHECK(run_stream_test <int32> ());
        CHECK(run_stream_test <int64> ());
        CHECK(run_stream_test <uint32> ());
        CHECK(run_stream_test <uint64> ());
        CHECK(run_stream_test <sint32> ());
        CHECK(run_stream_test <sint64> ());
        CHECK(run_stream_test <fixed32> ());
        CHECK(run_stream_test <fixed64> ());
        CHECK(run_stream_test <sfixed32> ());
        CHECK(run_stream_test <sfixed64> ());
    }

    TEST(BasicNumericRaw)
    {
        using namespace framework::protocol_buffers;

        CHECK(run_raw_test <float_> ());
        CHECK(run_raw_test <double_> ());
        CHECK(run_raw_test <bool_> ());
        CHECK(run_raw_test <int32> ());
        CHECK(run_raw_test <int64> ());
        CHECK(run_raw_test <uint32> ());
        CHECK(run_raw_test <uint64> ());
        CHECK(run_raw_test <sint32> ());
        CHECK(run_raw_test <sint64> ());
        CHECK(run_raw_test <fixed32> ());
        CHECK(run_raw_test <fixed64> ());
        CHECK(run_raw_test <sfixed32> ());
        CHECK(run_raw_test <sfixed64> ());
    }

    TEST(BasicStringStream)
    {
        using framework::serializable::dispatch_read;
        using framework::serializable::dispatch_write;
        using framework::protocol_buffers::string_;
        using framework::protocol_buffers::bytes;

        std::string in {"Hello World!"};
        std::string out;

        std::stringstream ss;
        CHECK(dispatch_write <string_> (in, ss));
        CHECK(dispatch_read <string_> (ss, out));
        CHECK(in == out);

        CHECK(dispatch_write <bytes> (in, ss));
        CHECK(dispatch_read <bytes> (ss, out));
        CHECK(in == out);
    }

    TEST(BasicStringRaw)
    {
        using framework::serializable::dispatch_read;
        using framework::serializable::dispatch_write;
        using framework::protocol_buffers::string_;
        using framework::protocol_buffers::bytes;

        std::string in {"Hello World!"};
        std::string out;
        std::vector <char> buffer (1+in.size());

        char const* rbegin = &buffer[0];
        char const* rend = &buffer[buffer.size()];
        auto rit = rbegin;

        char* wbegin = &buffer[0];
        char* wend = &buffer[buffer.size()];
        auto wit = wbegin;

        std::stringstream ss;
        CHECK(dispatch_write <string_> (in, wbegin, wend, wit));
        CHECK(dispatch_read <string_> (rbegin, rend, rit, out));
        CHECK(in == out);

        CHECK(dispatch_write <bytes> (in, wbegin, wend, wit));
        CHECK(dispatch_read <bytes> (rbegin, rend, rit, out));
        CHECK(in == out);
    }

    TEST(NestedTypesStream)
    {
        using framework::serializable::dispatch_read;
        using framework::serializable::dispatch_write;
        using framework::serializable::stl_vector;
        using framework::protocol_buffers::length_delimited;
        using type = length_delimited <stl_vector <std::size_t, int>>;

        std::vector <int> in {1, 2, 3, 4, 5};
        std::vector <int> out;

        std::stringstream ss;
        CHECK(dispatch_write <type> (in, std::ofstream("out.dat")));
        CHECK(dispatch_write <type> (in, ss));
        CHECK(dispatch_read <type> (ss, out));
        CHECK(in == out);
    }

    TEST(NestedTypesRaw)
    {
        using framework::serializable::dispatch_read;
        using framework::serializable::dispatch_write;
        using framework::serializable::stl_vector;
        using framework::protocol_buffers::length_delimited;
        using type = length_delimited <stl_vector <std::size_t, int>>;

        std::vector <int> in {1, 2, 3, 4, 5};
        std::vector <int> out;
        std::vector <char> buffer(5 + 8 + 5*4);

        char const* rbegin = &buffer[0];
        char const* rend = &buffer[buffer.size()];
        auto rit = rbegin;

        char* wbegin = &buffer[0];
        char* wend = &buffer[buffer.size()];
        auto wit = wbegin;

        std::stringstream ss;
        CHECK(dispatch_write <type> (in, wbegin, wend, wit));
        CHECK(dispatch_read <type> (rbegin, rend, rit, out));
        CHECK(in == out);
    }
}
