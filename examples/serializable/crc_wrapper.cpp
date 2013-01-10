// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <string.h>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <chrono>
#include <UnitTest++/UnitTest++.h>

#include <framework/serializable.hpp>
#include <framework/serializable/mutators/little_endian.hpp>
#include <framework/serializable/mutators/big_endian.hpp>
#include <framework/serializable/mutators/stl_array.hpp>
#include <framework/serializable/streams/crc_wrapper.hpp>

template <typename T>
uint16_t sum_test (T& in) __attribute__ ((noinline));

using namespace framework;
using namespace framework::serializable;

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

#ifdef SUM_TEST
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
    std::cout << "Time Per Byte: " << double(duration.count()) / double(20*COUNT) << "ns" << std::endl;

    return 0;
}

template <typename T>
uint16_t sum_test (T& in)
{
    // Modular sum test
    static crc_32_wrapper out;
    if (!write(in, out))
        exit(1);

    return out.checksum();
}
