// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <iostream>
#include <sstream>
#include <array>
#include <UnitTest++/UnitTest++.h>

#include <framework/serializable/base_types.hpp>
#include <framework/serializable/inline_object.hpp>
#include <framework/serializable/containers/value.hpp>
#include <framework/serializable/common_macros.hpp>

SUITE(framework_serializable_interfaces_comparable_hpp)
{
    TEST(Basic)
    {
        using namespace framework::serializable;
        
        using test_type = inline_object <
            value <NAME("Field 1"), uint64_t>,
            value <NAME("Field 2"), float>>;

        test_type const lhs          {1, 2.0f};
        test_type const rhs_equal    {1, 2.0f};
        test_type const rhs_less     {1, 1.0f};
        test_type const rhs_greater  {1, 3.0f};

        // lhs == rhs
        CHECK(!less(lhs, rhs_equal));
        CHECK(!less(rhs_equal, lhs));
        
        // rhs < lhs
        CHECK(!less(lhs, rhs_less));
        CHECK(less(rhs_less, lhs));
        
        // lhs < rhs
        CHECK(less(lhs, rhs_greater));
        CHECK(!less(rhs_greater, lhs));
    }

    TEST(Helpers)
    {
        using namespace framework::serializable;
        
        using test_type = inline_object <
            value <NAME("Field 1"), int>>;

        for (int i=-10; i < 10; ++i)
        {
            for (int j=-10; j < 10; ++j)
            {
                test_type const lhs {i};
                test_type const rhs {j};

                if (i < j) CHECK(less(lhs, rhs));
                else       CHECK(!less(lhs, rhs));

                if (i > j) CHECK(greater(lhs, rhs));
                else       CHECK(!greater(lhs, rhs));

                if (i <= j) CHECK(less_or_equal(lhs, rhs));
                else        CHECK(!less_or_equal(lhs, rhs));

                if (i >= j) CHECK(greater_or_equal(lhs, rhs));
                else        CHECK(!greater_or_equal(lhs, rhs));

                if (i == j) CHECK(equal(lhs, rhs));
                else        CHECK(!equal(lhs, rhs));
            }
        }
    }
}
