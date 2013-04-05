// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <chrono>

#include <framework/serializable.hpp>
#include <framework/serializable/containers/bit_field.hpp>
#include <framework/serializable/mutators/bit_value.hpp>
#include <framework/serializable/mutators/endian.hpp>
#include <framework/serializable/streams/modular_sum.hpp>

using namespace framework;
using namespace framework::serializable;

template <typename T>
__attribute__ ((noinline))
uint16_t sum (T const& in)
{
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

#if defined(SUM_CONTROL_1)
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

        result += sum(x);
#elif defined(SUM_TEST_1)
        inline_object <
            bit_field <big_endian <uint32_t>, 
                value <NAME("Version"),                   bit_value <4>>,
                value <NAME("Internet Header Length"),    bit_value <4>>,
                value <NAME("Type of Service"),           bit_value <8>>,
                value <NAME("Total Length"),              bit_value <16>>,
                value <NAME("Identification"),            bit_value <16>>,
                value <NAME("Flags"),                     bit_value <3>>,
                value <NAME("Fragment Offset"),           bit_value <13>>,
                value <NAME("Time to Live"),              bit_value <8>>,
                value <NAME("Protocol"),                  bit_value <8>>,
                value <NAME("Header Checksum"),           bit_value <16>>,
                value <NAME("Source Address"),            bit_value <32>>,
                value <NAME("Destination Address"),       bit_value <32>>>
        > x {tmp >> 4, tmp, tmp, tmp, tmp, tmp >> 13, tmp, tmp, tmp, tmp, tmp, tmp};

        result += sum(x);
#elif defined(SUM_TEST_2)
        inline_object <bit_field <uint8_t,
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
        >> x {tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp};

        result += sum(x);
#elif defined(SUM_TEST_3)
        inline_object <
            bit_field <big_endian <uint32_t>, 
                value <NAME("Version"),                   bit_value <4>>,
                value <NAME("Internet Header Length"),    bit_value <4>>,
                value <NAME("Type of Service"),           inline_object <
                    value <NAME("Precedence"),                bit_value <3>>,
                    value <NAME("Low Delay"),                 bit_value <1>>,
                    value <NAME("High Throughput"),           bit_value <1>>,
                    value <NAME("High Reliability"),          bit_value <1>>,
                    value <NAME("Reserved"),                  bit_value <2>>>>,
                value <NAME("Total Length"),              bit_value <16>>,
                value <NAME("Identification"),            bit_value <16>>,
                value <NAME("Flags"),                     inline_object <
                    value <NAME("Reserved"),                  bit_value <1>>,
                    value <NAME("Don't Fragment"),            bit_value <1>>,
                    value <NAME("More Fragments"),            bit_value <1>>>>,
                value <NAME("Fragment Offset"),           bit_value <13>>,
                value <NAME("Time to Live"),              bit_value <8>>,
                value <NAME("Protocol"),                  bit_value <8>>,
                value <NAME("Header Checksum"),           bit_value <16>>,
                value <NAME("Source Address"),            bit_value <32>>,
                value <NAME("Destination Address"),       bit_value <32>>>
        > x {tmp >> 4, 
             tmp,
             std::forward_as_tuple(tmp >> 5, tmp >> 4, tmp >> 3, tmp >> 2, tmp), 
             tmp, 
             tmp, 
             std::forward_as_tuple(tmp >> 15, tmp >> 14, tmp >> 13), 
             tmp, 
             tmp, 
             tmp, 
             tmp, 
             tmp, 
             tmp};

        result += sum(x);
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

