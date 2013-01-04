// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <UnitTest++/UnitTest++.h>

#include <framework/common/variadic_switch_return.hpp>

int switch_check (std::size_t i)
{
    switch (i)
    {
        case 0: return 0;
        case 1: return 1;
        case 2: return 2;
        case 3: return 3;
        case 4: return 4;
        default: return 999;
    }
}

struct switch_value
{
    template <std::size_t N>
    int operator() ()
    {
        return N;
    }

    int operator() ()
    {
        return 999;
    }
};

SUITE(framework_variadic_switch_return_hpp)
{
    TEST(Basic)
    {
        using namespace framework;

        for (int i=0; i < 10; ++i)
        {
            auto const x1 = switch_check(i);
            auto const x2 = variadic_switch_return <make_value_indices <5>> (i, switch_value());

            CHECK(x1 == x2);
        }
    }
}
