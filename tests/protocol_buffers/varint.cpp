// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <iostream>
#include <sstream>
#include <UnitTest++/UnitTest++.h>

// Note: zig-zag is tested using unsigned values here for completeness - this triggers various tautological compare
//       warnings (as errors, -Werror) when compiling these tests.  As zig-zag is not intended to support unsigned
//       values these warnings were not disabled in general.

#ifdef __clang__
 #pragma clang diagnostic push
 #pragma clang diagnostic ignored "-Wtautological-compare"
#endif

#include <framework/protocol_buffers/varint.hpp>

template <typename T>
bool run_stream_test ()
{
    // Note: we cannot represent 64-bit unsigned values using zig-zag in our implementation - the value is stored in
    //       an uint64_t container then processed to avoid duplication of varint code.  It did not seem worthwhile to
    //       remove this restriction given that zig-zag really is not suitable for unsigned values.

    enum{ mask = (1ull << 63) - 1 };

    using framework::serializable::type_extractor;
    using framework::serializable::dispatch_read;
    using framework::serializable::dispatch_write;

    type_extractor <T> out, in {0};

    for (int i=0; i < 200; ++i)
    {
        in = static_cast <type_extractor <T>> (mask & (uint64_t(i) ^ (uint64_t(in) << 1)));
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
    // Note: we cannot represent 64-bit unsigned values using zig-zag in our implementation - the value is stored in
    //       an uint64_t container then processed to avoid duplication of varint code.  It did not seem worthwhile to
    //       remove this restriction given that zig-zag really is not suitable for unsigned values.

    enum{ mask = (1ull << 63) - 1 };

    using framework::serializable::type_extractor;
    using framework::serializable::dispatch_read;
    using framework::serializable::dispatch_write;

    type_extractor <T> out, in {0};

    for (int i=0; i < 200; ++i)
    {
        char buffer[10] = {0};
        in = static_cast <type_extractor <T>> (mask & (uint64_t(i) ^ (uint64_t(in) << 1)));
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
    TEST(BasicStream)
    {
        using framework::protocol_buffers::varint;
        using framework::protocol_buffers::zig_zag;

        CHECK(run_stream_test <varint <bool>> ());
        CHECK(run_stream_test <varint <uint8_t>> ());
        CHECK(run_stream_test <varint <uint16_t>> ());
        CHECK(run_stream_test <varint <uint32_t>> ());
        CHECK(run_stream_test <varint <uint64_t>> ());
        CHECK(run_stream_test <varint <int8_t>> ());
        CHECK(run_stream_test <varint <int16_t>> ());
        CHECK(run_stream_test <varint <int32_t>> ());
        CHECK(run_stream_test <varint <int64_t>> ());
        CHECK(run_stream_test <zig_zag <bool>> ());
        CHECK(run_stream_test <zig_zag <uint8_t>> ());
        CHECK(run_stream_test <zig_zag <uint16_t>> ());
        CHECK(run_stream_test <zig_zag <uint32_t>> ());
        CHECK(run_stream_test <zig_zag <uint64_t>> ());
        CHECK(run_stream_test <zig_zag <int8_t>> ());
        CHECK(run_stream_test <zig_zag <int16_t>> ());
        CHECK(run_stream_test <zig_zag <int32_t>> ());
        CHECK(run_stream_test <zig_zag <int64_t>> ());
    }

    TEST(BasicRaw)
    {
        using framework::protocol_buffers::varint;
        using framework::protocol_buffers::zig_zag;

        CHECK(run_raw_test <varint <bool>> ());
        CHECK(run_raw_test <varint <uint8_t>> ());
        CHECK(run_raw_test <varint <uint16_t>> ());
        CHECK(run_raw_test <varint <uint32_t>> ());
        CHECK(run_raw_test <varint <uint64_t>> ());
        CHECK(run_raw_test <varint <int8_t>> ());
        CHECK(run_raw_test <varint <int16_t>> ());
        CHECK(run_raw_test <varint <int32_t>> ());
        CHECK(run_raw_test <varint <int64_t>> ());
        CHECK(run_raw_test <zig_zag <bool>> ());
        CHECK(run_raw_test <zig_zag <uint8_t>> ());
        CHECK(run_raw_test <zig_zag <uint16_t>> ());
        CHECK(run_raw_test <zig_zag <uint32_t>> ());
        CHECK(run_raw_test <zig_zag <uint64_t>> ());
        CHECK(run_raw_test <zig_zag <int8_t>> ());
        CHECK(run_raw_test <zig_zag <int16_t>> ());
        CHECK(run_raw_test <zig_zag <int32_t>> ());
        CHECK(run_raw_test <zig_zag <int64_t>> ());
    }
}

#ifdef __clang__
 #pragma clang diagnostic pop
#endif
