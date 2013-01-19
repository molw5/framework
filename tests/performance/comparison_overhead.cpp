#include <cassert>

#include <framework/serializable.hpp>

using ::framework::serializable::inline_object;
using ::framework::serializable::value;
using ::framework::serializable::read;
using ::framework::serializable::write;
    
struct s1
{
    int x;
};

using s2 = inline_object <
    value <NAME("x"), int>>;

template <typename T>
bool compare_control (T const& lhs, T const& rhs) __attribute__ ((noinline));

template <typename T>
bool compare_test (T const& lhs, T const& rhs) __attribute__ ((noinline));

int main ()
{
    s1 const control_lhs{1}, control_rhs{2};
    s2 const test_lhs{1}, test_rhs{2};

    assert(compare_control(control_lhs, control_rhs));
    assert(compare_test(test_lhs, test_rhs));
    
    return 0;
}

template <typename T>
bool compare_control (T const& lhs, T const& rhs)
{
    return lhs.x < rhs.x;
}

template <typename T>
bool compare_test (T const& lhs, T const& rhs)
{
    return lhs < rhs;
}
