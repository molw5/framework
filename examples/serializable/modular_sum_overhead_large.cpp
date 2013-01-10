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
#include <framework/serializable/mutators/stl_vector.hpp>
#include <framework/serializable/streams/modular_sum.hpp>

template <typename T>
uint16_t sum_control_1 (T& in) __attribute__ ((noinline));

template <typename T>
uint16_t sum_test (T& in) __attribute__ ((noinline));

using namespace framework;
using namespace framework::serializable;

#ifndef COUNT
    #define COUNT 10000
#endif

#ifndef SIZE
    #define SIZE 0xF000
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
            value <NAME("x"), stl_vector <big_endian <uint32_t>, uint8_t>>
        > x {std::vector <uint8_t> (SIZE, tmp)};

#ifdef SUM_CONTROL_1
        result += sum_control_1(x);
#else      
 #ifdef SUM_TEST
        result += sum_test(x);
 #else
  #error "Test type must be specified"
 #endif
#endif
    }
    auto const& end = std::chrono::high_resolution_clock::now();

    auto const& duration = std::chrono::duration_cast <std::chrono::nanoseconds> (end - start);

    std::cout << "Result: " << std::hex << result << std::dec << std::endl;
    std::cout << "Duration: " << duration.count() << "ns" << std::endl;
    std::cout << "Operation Time: " << double(duration.count()) / double(COUNT) << "ns" << std::endl;
    std::cout << "Time Per Byte: " << double(duration.count()) / (double(SIZE)*double(COUNT)+4) << "ns" << std::endl;

    return 0;
}

template <typename T>
uint16_t sum_control_1 (T& in)
{
    // Optimized summation
    auto const& value = get <NAME("x")> (in);

    uint64_t result = value.size();
    result = (result & 0xFFFFFFFF) + (result >> 32);

    for (unsigned int i=0; i < value.size()/2; ++i)
        result += (static_cast <uint64_t> (value[i*2])) +
                  (static_cast <uint64_t> (value[i*2+1]) << 8);

    if (value.size() % 2)
        result += static_cast <uint64_t> (value.back()) << 8;

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
uint16_t sum_test (T& in)
{
    // Modular sum test
    internet_checksum out;
    if (!write(in, out))
        exit(1);

    return out.checksum();
}
