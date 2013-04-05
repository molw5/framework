// Copyright (C) 2013 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Protocol buffers benchmarks.
*
* \note
* The deserialization methods used here primarily measure heap performance.  Early benchmarks used a reset
* method, similar to protobuf's implementation, to clear an object without releasing memory - this approach
* was abandoned as it seemed a poor solution in most use cases outside these benchmarks.
*
* \copyright
* Copyright &copy; 2013 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <memory>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <iomanip>

#include <framework/protocol_buffers.hpp>

using namespace framework::protocol_buffers;

using Message1 = message <
    required <NAME("field1"), 1, string_>,
    optional <NAME("field9"), 9, string_>,
    optional <NAME("field18"), 18, string_>,
    optional <NAME("field80"), 80, bool_>,
    optional <NAME("field81"), 81, bool_>,
    required <NAME("field2"), 2, int32>,
    required <NAME("field3"), 3, int32>,
    optional <NAME("field280"), 280, int32>,
    optional <NAME("field6"), 6, int32>,
    optional <NAME("field22"), 22, int64>,
    optional <NAME("field4"), 4, string_>,
    repeated <NAME("field5"), 5, fixed64>,
    optional <NAME("field59"), 59, bool_>,
    optional <NAME("field7"), 7, string_>,
    optional <NAME("field16"), 16, int32>,
    optional <NAME("field130"), 130, int32>,
    optional <NAME("field12"), 12, bool_>,
    optional <NAME("field17"), 17, bool_>,
    optional <NAME("field13"), 13, bool_>,
    optional <NAME("field14"), 14, bool_>,
    optional <NAME("field104"), 104, int32>,
    optional <NAME("field100"), 100, int32>,
    optional <NAME("field101"), 101, int32>,
    optional <NAME("field102"), 102, string_>,
    optional <NAME("field103"), 103, string_>,
    optional <NAME("field29"), 29, int32>,
    optional <NAME("field30"), 30, bool_>,
    optional <NAME("field60"), 60, int32>,
    optional <NAME("field271"), 271, int32>,
    optional <NAME("field272"), 272, int32>,
    optional <NAME("field150"), 150, int32>,
    optional <NAME("field23"), 23, int32>,
    optional <NAME("field24"), 24, bool_>,
    optional <NAME("field25"), 25, int32>,
    optional <NAME("field15"), 15, message <
        optional <NAME("field1"), 1, int32>,
        optional <NAME("field2"), 2, int32>,
        optional <NAME("field3"), 3, int32>,
        optional <NAME("field15"), 15, string_>,
        optional <NAME("field12"), 12, bool_>,
        optional <NAME("field13"), 13, int64>,
        optional <NAME("field14"), 14, int64>,
        optional <NAME("field16"), 16, int32>,
        optional <NAME("field19"), 19, int32>,
        optional <NAME("field20"), 20, bool_>,
        optional <NAME("field28"), 28, bool_>,
        optional <NAME("field21"), 21, fixed64>,
        optional <NAME("field22"), 22, int32>,
        optional <NAME("field23"), 23, bool_>,
        optional <NAME("field206"), 206, bool_>,
        optional <NAME("field203"), 203, fixed32>,
        optional <NAME("field204"), 204, int32>,
        optional <NAME("field205"), 205, string_>,
        optional <NAME("field207"), 207, uint64>,
        optional <NAME("field300"), 300, uint64>>>,
    optional <NAME("field78"), 78, bool_>,
    optional <NAME("field67"), 67, int32>,
    optional <NAME("field68"), 68, int32>,
    optional <NAME("field128"), 128, int32>,
    optional <NAME("field129"), 129, string_>,
    optional <NAME("field131"), 131, int32>>;

using Message2 = message <
    optional <NAME("field1"), 1, string_>,
    optional <NAME("field3"), 3, int64>,
    optional <NAME("field4"), 4, int64>,
    optional <NAME("field30"), 30, int64>,
    optional <NAME("field75"), 75, bool_>,
    optional <NAME("field6"), 6, string_>,
    optional <NAME("field2"), 2, bytes>,
    optional <NAME("field21"), 21, int32>,
    optional <NAME("field71"), 71, int32>,
    optional <NAME("field25"), 25, float_>,
    optional <NAME("field109"), 109, int32>,
    optional <NAME("field210"), 210, int32>,
    optional <NAME("field211"), 211, int32>,
    optional <NAME("field212"), 212, int32>,
    optional <NAME("field213"), 213, int32>,
    optional <NAME("field216"), 216, int32>,
    optional <NAME("field217"), 217, int32>,
    optional <NAME("field218"), 218, int32>,
    optional <NAME("field220"), 220, int32>,
    optional <NAME("field221"), 221, int32>,
    optional <NAME("field222"), 222, float_>,
    optional <NAME("field63"), 63, int32>,
    repeated <NAME("group1"), 10, group <
        required <NAME("field11"), 11, float_>,
        optional <NAME("field26"), 26, float_>,
        optional <NAME("field12"), 12, string_>,
        optional <NAME("field13"), 13, string_>,
        repeated <NAME("field14"), 14, string_>,
        required <NAME("field15"), 15, uint64>,
        optional <NAME("field5"), 5, int32>,
        optional <NAME("field27"), 27, string_>,
        optional <NAME("field28"), 28, int32>,
        optional <NAME("field29"), 29, string_>,
        optional <NAME("field16"), 16, string_>,
        repeated <NAME("field22"), 22, string_>,
        repeated <NAME("field73"), 73, int32>,
        optional <NAME("field20"), 20, int32>,
        optional <NAME("field24"), 24, string_>,
        optional <NAME("field31"), 31, message <
            optional <NAME("field1"), 1, float_>,
            optional <NAME("field2"), 2, float_>,
            optional <NAME("field3"), 3, float_>,
            optional <NAME("field4"), 4, bool_>,
            optional <NAME("field5"), 5, bool_>,
            optional <NAME("field6"), 6, bool_>,
            optional <NAME("field7"), 7, bool_>,
            optional <NAME("field8"), 8, float_>,
            optional <NAME("field9"), 9, bool_>,
            optional <NAME("field10"), 10, float_>,
            optional <NAME("field11"), 11, int64>>>>>,
    repeated <NAME("field128"), 128, string_>,
    optional <NAME("field131"), 131, int64>,
    repeated <NAME("field127"), 127, string_>,
    optional <NAME("field129"), 129, int32>,
    repeated <NAME("field130"), 130, int64>,
    optional <NAME("field205"), 205, bool_>,
    optional <NAME("field206"), 206, bool_>>;

#ifndef MIN_SAMPLE_TIME_MS
    #define MIN_SAMPLE_TIME_MS (1 * 1000)
#endif

#ifndef TARGET_TIME_MS
    #define TARGET_TIME_MS (15 * 1000)
#endif

void usage ()
{
    std::cerr << "Usage: protocol_buffers [[type] [file]]..." << std::endl;
    exit(1);
}

template <typename T>
bool runTest (std::string const& file);

template <typename T>
void benchmark (std::string const& name, std::size_t dataSize, T action);

template <typename T>
long timeAction (T action, int iterations);

int main (int argc, char const* argv[])
{
    if (argc < 3 || (argc % 2) != 1)
        usage();

    bool result = true;
    for (int i=1; i < argc; i += 2)
    {
        std::string const type = argv[i];
        std::string const file = argv[i+1];

        std::cout << "Benchmarking " << type << " with file " << argv[i+1] << std::endl;
        if (type == "Message1")
            result &= runTest <Message1> (file);
        else if (type == "Message2")
            result &= runTest <Message2> (file);
        else
            std::cerr << "Unknown message type " << type << std::endl;
    }

    if (!result)
        std::cout << "Tests failed." << std::endl;

    return result ? 0 : 1;
}

template <typename T>
__attribute__((noinline))
auto run (T&& t) ->
decltype(t())
{
    return t();
}

template <typename T>
bool runTest (std::string const& file)
{
    using framework::serializable::dispatch_read;
    using framework::serializable::dispatch_write;

    T sampleMessage;
    if (!read(std::ifstream(file), sampleMessage))
    {
        std::cout << "Read failed" << std::endl;
        return false;
    }

    size_frame size;
    max_size_frame max_size;
    if (!write(sampleMessage, size) || !write(sampleMessage, max_size))
    {
        std::cout << "Compute size failed" << std::endl;
        return false;
    }

    auto const d_size = double(static_cast <std::size_t> (size));
    auto const d_max_size = double(static_cast <std::size_t> (max_size));
    std::cout << "Maximum size bloat: " 
              << 100.0 * (d_max_size - d_size) / d_size 
              << "%" << std::endl;

    try {
        std::vector <char> buffer(static_cast <std::size_t> (size));
  
        benchmark("Compute object size", buffer.size(), [&]() {
                size_frame size;
                dispatch_write <T> (sampleMessage, size);
                return static_cast <std::size_t> (size);
            });

        benchmark("Compute maximum object size", buffer.size(), [&]() {
                max_size_frame size;
                dispatch_write <T> (sampleMessage, size);
                return static_cast <std::size_t> (size);
            });

        benchmark("Serialize to byte string", buffer.size(), [&]() {
                char* begin = &buffer[0];
                char* end = &buffer[buffer.size()];
                char* it = begin;

                return dispatch_write <T> (sampleMessage, begin, end, it);
            });

        std::string raw(static_cast <std::size_t> (max_size), '\0');
        benchmark("Serialize using write_to_array optimal", buffer.size(), [&]() {
                auto n = static_cast <std::size_t> (max_size);
                return write_to_array(sampleMessage, &raw[0], n);
            });

        benchmark("Serialize using write_to_array tight", buffer.size(), [&]() {
                auto n = static_cast <std::size_t> (size);
                return write_to_array(sampleMessage, &raw[0], n);
            });

        benchmark("Serialize using write_to_string_fast", buffer.size(), [&]() {
                return write_to_string_fast(sampleMessage, raw);
            });

        benchmark("Serialize using write_to_string_tight", buffer.size(), [&]() {
                return write_to_string_tight(sampleMessage, raw);
            });

        benchmark("Serialize using write_to_frame_fast", buffer.size(), [&]() {
                return write_to_frame_fast(sampleMessage, raw);
            });

        benchmark("Serialize using write_to_frame_tight", buffer.size(), [&]() {
                return write_to_frame_tight(sampleMessage, raw);
            });

        std::stringstream ss;
        benchmark("Serialize to stringstream", buffer.size(), [&]() {
                ss.clear();
                ss.seekp(0, std::ios::beg);
                return write(sampleMessage, ss);
            });

        benchmark("Serialize using write_to_ostream", buffer.size(), [&]() {
                ss.clear();
                ss.seekp(0, std::ios::beg);
                return write_to_ostream(sampleMessage, ss);
            });

        benchmark("Deserialize from byte string", buffer.size(), [&]() {
                T x;
                char const* begin = &buffer[0];
                char const* end = &buffer[buffer.size()];
                char const* it = begin;
                return dispatch_read <T> (begin, end, it, x);
            });
        
        benchmark("Deserialize using read_from_array", buffer.size(), [&]() {
                T x;
                return read_from_array(&buffer[0], buffer.size(), x);
            });

        benchmark("Deserialize using read_from_string", buffer.size(), [&]() {
                T x;
                return read_from_string(buffer, x);
            });

        benchmark("Deserialize using read_from_frame", buffer.size(), [&]() {
                T x;
                return read_from_frame(buffer, x);
            });
        
        benchmark("Deserialize from stringstream", buffer.size(), [&]() {
                T x;
                ss.clear();
                ss.seekg(0, std::ios::beg);
                return read(ss, x);
            });

        benchmark("Deserialize using read_from_istream", buffer.size(), [&]() {
                T x;
                ss.clear();
                ss.seekg(0, std::ios::beg);
                return read_from_istream(ss, x);
            });

        std::cout << std::endl;
        return true;
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
}

template <typename T>
void benchmark (std::string const& name, std::size_t dataSize, T action)
{
    int iterations = 1;
    long elapsed = timeAction(action, iterations);
    while (elapsed < MIN_SAMPLE_TIME_MS) {
      iterations *= 2;
      elapsed = timeAction(action, iterations);
    }
    
    iterations = (int) ((TARGET_TIME_MS / (double) elapsed) * iterations);
    elapsed = timeAction(action, iterations);

    std::cout << name << ": " << iterations << " iterations in "
        << elapsed/1000.0f << "s; "
        << (iterations * dataSize) / (elapsed * 1024 * 1024 / 1000.0f) << "MB/s" << std::endl;
}

template <typename T>
long timeAction (T action, int iterations)
{
    auto const start = std::chrono::steady_clock::now();
    for (int i=0; i < iterations; ++i)
        if (!run(action))
            throw std::runtime_error("timeAction failed");
    auto const end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast <std::chrono::milliseconds> (end-start).count();
}
