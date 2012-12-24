// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <iostream>
#include <sstream>
#include <UnitTest++/UnitTest++.h>

#include <framework/serializable/base_types.hpp>

namespace framework
{
    namespace serializable
    {
        template <typename T>
        struct TestBasic
        {
            static void run ()
            {
                T out, in {0};
                
                for (int i=0; i < 200; ++i)
                {
                    in = uint64_t(i) ^ (uint64_t(in) << 1);
                    out = 0;
                    
                    std::stringstream ss;
                    CHECK(write(in, ss));
                    CHECK(read(ss, out));
                    CHECK(in == out);
                }
            }
        };
    }
}

SUITE(framework_serializable_base_types_hpp)
{
    TEST(Basic)
    {
        framework::serializable::TestBasic <uint8_t>::run();
        framework::serializable::TestBasic <uint16_t>::run();
        framework::serializable::TestBasic <uint32_t>::run();
        framework::serializable::TestBasic <uint64_t>::run();
        framework::serializable::TestBasic <int8_t>::run();
        framework::serializable::TestBasic <int16_t>::run();
        framework::serializable::TestBasic <int32_t>::run();
        framework::serializable::TestBasic <int64_t>::run();
        framework::serializable::TestBasic <float>::run();
        framework::serializable::TestBasic <double>::run();
    }
}
