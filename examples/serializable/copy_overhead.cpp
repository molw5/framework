#include <cassert>
#include <iostream>
#include <iomanip>

#include <framework/serializable.hpp>

using ::framework::serializable::inline_object;
using ::framework::serializable::value;
using ::framework::serializable::read;
using ::framework::serializable::write;
    
struct s1
{
    int x1;
    int x2;
};

using s2 = inline_object <
    value <NAME("x1"), int>,
    value <NAME("x2"), int>>;

template <typename T>
void copy_control (T const& lhs, T& rhs) __attribute__ ((noinline));

template <typename T>
void copy_test (T const& lhs, T& rhs) __attribute__ ((noinline));

template <typename T>
void move_control (T& lhs, T& rhs) __attribute__ ((noinline));

template <typename T>
void move_test (T& lhs, T& rhs) __attribute__ ((noinline));

int main ()
{
    s1 control_lhs{1, 2};
    s2 test_lhs{1, 2};

    s1 control_rhs;
    s2 test_rhs;

    copy_control(control_lhs, control_rhs);
    copy_test(test_lhs, test_rhs);
    
    move_control(control_lhs, control_rhs);
    move_test(test_lhs, test_rhs);
    
    return 0;
}

template <typename T>
void copy_control (T const& lhs, T& rhs)
{
    rhs = lhs;
}

template <typename T>
void copy_test (T const& lhs, T& rhs)
{
    rhs = lhs;
}

template <typename T>
void move_control (T& lhs, T& rhs)
{
    rhs = lhs;
}

template <typename T>
void move_test (T& lhs, T& rhs)
{
    rhs = lhs;
}
