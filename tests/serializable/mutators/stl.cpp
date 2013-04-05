// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#include <iostream>
#include <sstream>
#include <UnitTest++/UnitTest++.h>

#include <framework/serializable/inline_object.hpp>
#include <framework/serializable/containers/value.hpp>
#include <framework/serializable/mutators/endian.hpp>
#include <framework/serializable/mutators/stl_array.hpp>
#include <framework/serializable/mutators/stl_deque.hpp>
#include <framework/serializable/mutators/stl_forward_list.hpp>
#include <framework/serializable/mutators/stl_list.hpp>
#include <framework/serializable/mutators/stl_map.hpp>
#include <framework/serializable/mutators/stl_pair.hpp>
#include <framework/serializable/mutators/stl_set.hpp>
#include <framework/serializable/mutators/stl_string.hpp>
#include <framework/serializable/mutators/stl_tuple.hpp>
#include <framework/serializable/mutators/stl_unordered_map.hpp>
#include <framework/serializable/mutators/stl_unordered_set.hpp>
#include <framework/serializable/mutators/stl_vector.hpp>
#include <framework/serializable/common_macros.hpp>

SUITE(framework_serializable_mutators_stl_hpp)
{
    TEST(array)
    {
        using namespace framework::serializable;

        inline_object <
            value <NAME("Field 1"), stl_array <1, int>>
        > out, in { std::array <int, 1> {{1}} };

        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in == out);
    }

    TEST(deque)
    {
        using namespace framework::serializable;

        inline_object <
            value <NAME("Field 1"), stl_deque <int, int>>
        > out, in { std::deque <int> {1, 2, 3} };

        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in == out);
    }

    /*
    TEST(forward_list)
    {
        using namespace framework::serializable;

        inline_object <value <NAME("Field 1"), stl_forward_list <int, int>>> in, out;

        std::forward_list <int> test {1, 2, 3};
        in.set <NAME("Field 1")> (test);

        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in == out);
    }
    */

    TEST(list)
    {
        using namespace framework::serializable;

        inline_object <
            value <NAME("Field 1"), stl_list <int, int>>
        > out, in{std::forward_as_tuple(std::forward_as_tuple(1, 2, 3))};
        
        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in == out);
    }

    TEST(multiset)
    {
        using namespace framework::serializable;

        inline_object <
            value <NAME("Field 1"), stl_multiset <int, int>>
        > out, in{std::forward_as_tuple(std::forward_as_tuple(1, 1, 2, 3, 3, 3))};

        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in == out);
    }

    TEST(map)
    {
        using namespace framework::serializable;

        inline_object <
            value <NAME("Field 1"), stl_map <int, int, int>>,
            value <NAME("Field 2"), stl_multimap <int, int, int>>
        > out, in {std::map <int, int> {{1, 2}}, std::multimap <int, int> {{1, 2}, {1, 3}, {2, 4}}};
            
        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in == out);
    }

    TEST(pair)
    {
        using namespace framework::serializable;

        inline_object <
            value <NAME("Field 1"), stl_pair <int, int>>
        > out, in {std::forward_as_tuple(std::forward_as_tuple(1, 2))};

        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in == out);
    }

    TEST(set)
    {
        using namespace framework::serializable;

        inline_object <
            value <NAME("Field 1"), stl_set <int, int>>,
            value <NAME("Field 2"), stl_multiset <int, int>>
        > out, in {std::forward_as_tuple(1, 2, 3),
                   std::forward_as_tuple(1, 1, 2, 3, 3, 3)};

        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in == out);
    }

    TEST(string)
    {
        using namespace framework::serializable;

        inline_object <
            value <NAME("Field 1"), stl_string <int>>,
            value <NAME("Field 2"), stl_null_string>,
            value <NAME("Field 3"), stl_wstring <int>>,
            value <NAME("Field 4"), stl_null_wstring>
        > out, in {std::string("Hello World!"),
                   std::string("Hello World!"),
                   std::wstring(L"Wide Hello World!"),
                   std::wstring(L"Wide Hello World!")};

        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in == out);
    }

    TEST(tuple)
    {
        using namespace framework::serializable;

        inline_object <
            value <NAME("Field 1"), stl_tuple <int, float, double>>
        > out, in {std::forward_as_tuple(std::forward_as_tuple(1, 2.0f, 3.0))};
        
        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in == out);
    }

    TEST(unordered_map)
    {
        using namespace framework::serializable;

        inline_object <
            value <NAME("Field 1"), stl_unordered_map <int, int, int>>,
            value <NAME("Field 2"), stl_unordered_multimap <int, int, int>>
        > out, in {std::unordered_map <int, int> {{1, 2}},
                   std::unordered_multimap <int, int> {{1, 2}, {1, 3}, {2, 4}}};

        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in == out);
    }

    TEST(unordered_set)
    {
        using namespace framework::serializable;

        inline_object <
            value <NAME("Field 1"), stl_unordered_set <int, int>>,
            value <NAME("Field 2"), stl_unordered_multiset <int, int>>
        > out, in {std::forward_as_tuple(1, 2, 3),
                   std::forward_as_tuple(1, 1, 2, 3, 3, 3)};

        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in == out);
    }

    TEST(vector)
    {
        using namespace framework::serializable;

        inline_object <
            value <NAME("Field 1"), stl_vector <int, int>>
        > out, in {std::forward_as_tuple(std::forward_as_tuple(1, 2, 3))};

        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in == out);
    }

    TEST(NestedContainers)
    {
        using namespace framework::serializable;

        // NOTE: clang++ (3.2) / libc++ (?) fail with certain nested container combinations.
        inline_object <value <NAME("Field 1"), 
            stl_vector <int,
            stl_deque <int,
            stl_list <int,
            stl_set <int,
            stl_multiset <int,
            stl_map <int, int,
            stl_multimap <int, int,
            stl_vector <int, int>
        >>>>>>>>> in, out;
        
        std::vector <
            std::deque <
            std::list <
            std::set <
            std::multiset <
            std::map <int,
            std::multimap <int,
            std::vector <int>
        >>>>>>> test {{{{{{{1, {{1, {1}}}}}}}}}};
        in.set <NAME("Field 1")> (test);

        std::stringstream ss;
        CHECK(write(in, ss));
        CHECK(read(ss, out));
        CHECK(in == out);
    }
}
