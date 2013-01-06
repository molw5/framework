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

#if FRAMEWORK_HOST_ENDIANNESS != FRAMEWORK_LITTLE_ENDIAN
    #error "Not supported - tests are sensitive to host-endianness"
#endif

template <typename T>
uint16_t sum_control_1 (T& in) __attribute__ ((noinline));

template <typename T>
uint16_t sum_control_2 (T& in) __attribute__ ((noinline));

template <typename T>
uint16_t sum_control_3 (T& in) __attribute__ ((noinline));

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

    auto const& start = std::chrono::high_resolution_clock::now();
    for (int i=0; i < COUNT; ++i)
    {
        inline_object <
            value <NAME("Field 1"), uint8_t>,
            value <NAME("Field 2"), uint64_t>,
            value <NAME("Field 3"), uint32_t>,
            value <NAME("Field 4"), uint8_t>,
            value <NAME("Field 5"), uint16_t>,
            value <NAME("Field 6"), uint8_t>,
            value <NAME("Field 7"), uint32_t>,
            value <NAME("Field 8"), uint64_t>,
            value <NAME("Field 9"), uint64_t>
        > x {i, i, i, i, i, i, i, i, i};

#ifdef SUM_CONTROL_1
        result += sum_control_1(x);
#else
 #ifdef SUM_CONTROL_2
        result += sum_control_2(x);
 #else
  #ifdef SUM_CONTROL_3
        result += sum_control_3(x);
  #else
   #ifdef SUM_TEST
        result += sum_test(x);
   #else
    #error "Test type must be specified"
   #endif
  #endif
 #endif
#endif
    }
    auto const& end = std::chrono::high_resolution_clock::now();

    auto const& duration = std::chrono::duration_cast <std::chrono::nanoseconds> (end - start);

    std::cout << "Result: " << result << std::endl;
    std::cout << "Duration: " << duration.count() << "ns" << std::endl;
    std::cout << "Operation Time: " << double(duration.count()) / double(COUNT) << "ns" << std::endl;
    std::cout << "Time Per Byte: " << double(duration.count()) / double(37*COUNT) << "ns" << std::endl;

    return 0;
}

template <typename T>
uint16_t sum_control_1 (T& in)
{
    // Literal translation, byte by byte
    auto result = static_cast <uint32_t> (
        (static_cast <uint32_t> (get <NAME("Field 1")> (in)) << 8) +
        (((static_cast <uint32_t> (get <NAME("Field 2")> (in) >>  0) & 0xFF)) << 0) +
        (((static_cast <uint32_t> (get <NAME("Field 2")> (in) >>  8) & 0xFF)) << 8) +
        (((static_cast <uint32_t> (get <NAME("Field 2")> (in) >> 16) & 0xFF)) << 0) +
        (((static_cast <uint32_t> (get <NAME("Field 2")> (in) >> 24) & 0xFF)) << 8) +
        (((static_cast <uint32_t> (get <NAME("Field 2")> (in) >> 32) & 0xFF)) << 0) +
        (((static_cast <uint32_t> (get <NAME("Field 2")> (in) >> 40) & 0xFF)) << 8) +
        (((static_cast <uint32_t> (get <NAME("Field 2")> (in) >> 48) & 0xFF)) << 0) +
        (((static_cast <uint32_t> (get <NAME("Field 2")> (in) >> 56) & 0xFF)) << 8) +
        (((static_cast <uint32_t> (get <NAME("Field 3")> (in) >>  0) & 0xFF)) << 0) +
        (((static_cast <uint32_t> (get <NAME("Field 3")> (in) >>  8) & 0xFF)) << 8) +
        (((static_cast <uint32_t> (get <NAME("Field 3")> (in) >> 16) & 0xFF)) << 0) +
        (((static_cast <uint32_t> (get <NAME("Field 3")> (in) >> 24) & 0xFF)) << 8) +
        (static_cast <uint32_t> (get <NAME("Field 4")> (in)) << 0) +
        (((static_cast <uint32_t> (get <NAME("Field 5")> (in) >>  0) & 0xFF)) << 8) +
        (((static_cast <uint32_t> (get <NAME("Field 5")> (in) >>  8) & 0xFF)) << 0) +
        (static_cast <uint32_t> (get <NAME("Field 6")> (in)) << 8) +
        (((static_cast <uint32_t> (get <NAME("Field 7")> (in) >>  0) & 0xFF)) << 0) +
        (((static_cast <uint32_t> (get <NAME("Field 7")> (in) >>  8) & 0xFF)) << 8) +
        (((static_cast <uint32_t> (get <NAME("Field 7")> (in) >> 16) & 0xFF)) << 0) +
        (((static_cast <uint32_t> (get <NAME("Field 7")> (in) >> 24) & 0xFF)) << 8) +
        (((static_cast <uint32_t> (get <NAME("Field 8")> (in) >>  0) & 0xFF)) << 0) +
        (((static_cast <uint32_t> (get <NAME("Field 8")> (in) >>  8) & 0xFF)) << 8) +
        (((static_cast <uint32_t> (get <NAME("Field 8")> (in) >> 16) & 0xFF)) << 0) +
        (((static_cast <uint32_t> (get <NAME("Field 8")> (in) >> 24) & 0xFF)) << 8) +
        (((static_cast <uint32_t> (get <NAME("Field 8")> (in) >> 32) & 0xFF)) << 0) +
        (((static_cast <uint32_t> (get <NAME("Field 8")> (in) >> 40) & 0xFF)) << 8) +
        (((static_cast <uint32_t> (get <NAME("Field 8")> (in) >> 48) & 0xFF)) << 0) +
        (((static_cast <uint32_t> (get <NAME("Field 8")> (in) >> 56) & 0xFF)) << 8) +
        (((static_cast <uint32_t> (get <NAME("Field 9")> (in) >>  0) & 0xFF)) << 0) +
        (((static_cast <uint32_t> (get <NAME("Field 9")> (in) >>  8) & 0xFF)) << 8) +
        (((static_cast <uint32_t> (get <NAME("Field 9")> (in) >> 16) & 0xFF)) << 0) +
        (((static_cast <uint32_t> (get <NAME("Field 9")> (in) >> 24) & 0xFF)) << 8) +
        (((static_cast <uint32_t> (get <NAME("Field 9")> (in) >> 32) & 0xFF)) << 0) +
        (((static_cast <uint32_t> (get <NAME("Field 9")> (in) >> 40) & 0xFF)) << 8) +
        (((static_cast <uint32_t> (get <NAME("Field 9")> (in) >> 48) & 0xFF)) << 0) +
        (((static_cast <uint32_t> (get <NAME("Field 9")> (in) >> 56) & 0xFF)) << 8));

    while (result >> 16)
        result = (result & 0xFFFF) + (result >> 16);

    return ~result;
}

template <typename T>
uint16_t sum_control_2 (T& in)
{
    // Optimized summation
    uint64_t tmp1 = 
        (get <NAME("Field 2")> (in) >> 32) +
        (get <NAME("Field 2")> (in) & 0xFFFFFFFF) +
        static_cast <uint64_t> (get <NAME("Field 3")> (in)) +
        static_cast <uint64_t> (get <NAME("Field 4")> (in)) +
        static_cast <uint64_t> (get <NAME("Field 7")> (in)) +
        (get <NAME("Field 8")> (in) >> 32) +
        (get <NAME("Field 8")> (in) & 0xFFFFFFFF) +
        (get <NAME("Field 9")> (in) >> 32) +
        (get <NAME("Field 9")> (in) & 0xFFFFFFFF);

    uint32_t tmp2 = 
        static_cast <uint32_t> (get <NAME("Field 1")> (in)) +
        static_cast <uint32_t> (get <NAME("Field 5")> (in)) +
        static_cast <uint32_t> (get <NAME("Field 6")> (in));

    tmp1 = (tmp1 & 0xFFFFFFFF) + (tmp1 >> 32);
    tmp1 = (tmp1 & 0xFFFF) + (tmp1 >> 16);
    tmp2 = (tmp2 & 0xFFFF) + (tmp2 >> 16);
    tmp2 = tmp1 + (tmp2 << 8);
    tmp2 = (tmp2 & 0xFFFF) + (tmp2 >> 16);
    tmp2 = (tmp2 & 0xFFFF) + (tmp2 >> 16);

    return ~tmp2;
}

struct unsafe_buffer
{
    unsafe_buffer ()
        : it(reinterpret_cast <char*> (&buffer[0])),
          begin(reinterpret_cast <char*> (&buffer[0]))
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
    uint16_t buffer[128];
};

template <typename T>
uint16_t sum_control_3 (T& in)
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

    result <<= 8;
    result = (result & 0xFFFF) + (result >> 16);
    result = (result & 0xFFFF) + (result >> 16);
    return ~static_cast <uint16_t> (result);
}

template <typename T>
uint16_t sum_test (T& in)
{
    // Modular sum test
    internet_checksum out;
    if (!write(in, out))
        throw std::runtime_error("Failed");

    return out.get();
}
