// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <chrono>
#include <UnitTest++/UnitTest++.h>

#include <framework/serializable.hpp>
#include <framework/serializable/mutators/little_endian.hpp>
#include <framework/serializable/mutators/big_endian.hpp>
#include <framework/serializable/mutators/stl_array.hpp>
#include <framework/serializable/streams/modular_sum.hpp>

using namespace framework;
using namespace framework::serializable;

struct unsafe_buffer
{
    unsafe_buffer ()
        : it(reinterpret_cast <char*> (&buffer[0])),
          begin(reinterpret_cast <char*> (&buffer[0])),
          end(reinterpret_cast <char*> (&buffer[128]))
    {
    }

    bool write (char const* s, std::size_t n)
    {
        memcpy(it, s, n);
        it += n;
        return true;
    }

    char* it;
    char* const begin;
    char* const end;
    uint16_t buffer[128];
};

template <typename T>
__attribute__ ((noinline))
uint16_t sum_control_1 (T const& in)
{
    // Optimized summation
    uint64_t result =
        static_cast <uint64_t> (get <NAME("Type of Service")> (in)) +
        static_cast <uint64_t> (get <NAME("Total Length")> (in)) +
        static_cast <uint64_t> (get <NAME("Identification")> (in)) +
        static_cast <uint64_t> (get <NAME("Flags / Fragment Offset")> (in)) +
        static_cast <uint64_t> (get <NAME("Protocol")> (in)) + 
        static_cast <uint64_t> (get <NAME("Header Checksum")> (in)) +
        static_cast <uint64_t> (get <NAME("Source Address")> (in)) +
        static_cast <uint64_t> (get <NAME("Destination Address")> (in));

    result <<= 8;
    result += 
        static_cast <uint64_t> (get <NAME("Version / IHL")> (in)) +
        static_cast <uint64_t> (get <NAME("Time to Live")> (in));

    while (result >> 16)
        result = (result & 0xFFFF) + (result >> 16);

#if FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_LITTLE_ENDIAN
    return ~FRAMEWORK_BYTESWAP16(static_cast <uint16_t> (result));
#elif FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_BIG_ENDIAN
    return ~static_cast <uint16_t> (result);
#else
    #error "Host endianness not supported"
#endif
}

template <typename T>
__attribute__ ((noinline))
uint16_t sum_control_2 (T const& in)
{
    // Buffered summation
    unsafe_buffer ss;
    if (!write(in, ss))
        exit(1);

    uint32_t result = 0;
    auto const size = ss.it - ss.begin;
    for (unsigned int i=0; i < size/2; ++i)
        result += ss.buffer[i];

    if (size % 2)
        result += ss.buffer[size/2+1];

    while (result >> 16)
        result = (result & 0xFFFF) + (result >> 16);

#if FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_LITTLE_ENDIAN
    return ~FRAMEWORK_BYTESWAP16(static_cast <uint16_t> (result));
#elif FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_BIG_ENDIAN
    return ~static_cast <uint16_t> (result);
#else
    #error "Host endianness not supported"
#endif
}

template <typename T>
__attribute__ ((noinline))
uint16_t sum_control_3 (T const& in)
{
    // Raw summation
    uint32_t const* buffer = reinterpret_cast <uint32_t const*> (&in);

    uint64_t result = 
        static_cast <uint64_t> (buffer[0]) + 
        static_cast <uint64_t> (buffer[1]) + 
        static_cast <uint64_t> (buffer[2]) + 
        static_cast <uint64_t> (buffer[3]) + 
        static_cast <uint64_t> (buffer[4]);

    while (result >> 16)
        result = (result & 0xFFFF) + (result >> 16);

#if FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_LITTLE_ENDIAN
    return ~static_cast <uint16_t> (result);
#elif FRAMEWORK_HOST_ENDIANNESS == FRAMEWORK_BIG_ENDIAN
    return ~FRAMEWORK_BYTESWAP16(static_cast <uint16_t> (result));
#else
    #error "Host endianness not supported"
#endif
}

template <typename T>
__attribute__ ((noinline))
uint16_t sum_test (T const& in)
{
    // Modular sum test
    internet_checksum out;
    if (!write(in, out))
        exit(1);

    return out.checksum();
}

#ifndef COUNT
    #define COUNT 10000000
#endif

int main ()
{
    int result = 0;
    uint32_t tmp = 0;
    
    auto const& start = std::chrono::high_resolution_clock::now();
    for (int i=0; i < COUNT; ++i)
    {
        tmp = (tmp << 1) ^ i;
        inline_object <
            value <NAME("Version / IHL"),             big_endian <uint8_t>>,
            value <NAME("Type of Service"),           big_endian <uint8_t>>,
            value <NAME("Total Length"),              big_endian <uint16_t>>,
            value <NAME("Identification"),            big_endian <uint16_t>>,
            value <NAME("Flags / Fragment Offset"),   big_endian <uint16_t>>,
            value <NAME("Time to Live"),              big_endian <uint8_t>>,
            value <NAME("Protocol"),                  big_endian <uint8_t>>,
            value <NAME("Header Checksum"),           big_endian <uint16_t>>,
            value <NAME("Source Address"),            big_endian <uint32_t>>,
            value <NAME("Destination Address"),       big_endian <uint32_t>>
        > x {tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp};

#if defined(SUM_CONTROL_1)
        result += sum_control_1(x);
#elif defined(SUM_CONTROL_2)
        result += sum_control_2(x);
#elif defined(SUM_CONTROL_3)
        result += sum_control_3(x);
#elif defined(SUM_TEST)
        result += sum_test(x);
#else
    #error "Test type must be specified"
#endif
    }
    auto const& end = std::chrono::high_resolution_clock::now();

    auto const& duration = std::chrono::duration_cast <std::chrono::nanoseconds> (end - start);

    std::cout << "Result: " << result << std::endl;
    std::cout << "Duration: " << duration.count() << "ns" << std::endl;
    std::cout << "Operation Time: " << double(duration.count()) / double(COUNT) << "ns" << std::endl;
    std::cout << "Time Per Byte: " << double(duration.count()) / (20.0*COUNT) << "ns" << std::endl;

    return 0;
}
