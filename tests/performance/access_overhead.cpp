#include <iostream>

#include <framework/serializable.hpp>
#include <framework/serializable/mutators/stl_string.hpp>

using ::framework::serializable::inline_object;
using ::framework::serializable::value;
using ::framework::serializable::get;
using ::framework::serializable::set;
    
struct s1
{
    int x;
};

using s2 = inline_object <
    value <NAME("x"), int>>;

template <typename T>
void assignment_control (T& in) __attribute__ ((noinline));

template <typename T>
void assignment_test (T& in) __attribute__ ((noinline));

template <typename T>
int access_control (T& in) __attribute__ ((noinline));

template <typename T>
int access_test (T& in) __attribute__ ((noinline));

template <typename T>
void construction_control (T& in) __attribute__ ((noinline));

template <typename T>
void construction_test (T& in) __attribute__ ((noinline));

int main ()
{
    s1 control{0};
    s2 test{0};

    std::cout << access_control(control) << std::endl;
    std::cout << access_test(test) << std::endl;

    assignment_control(control);
    assignment_test(test);

    std::cout << control.x << std::endl;
    std::cout << test.get <NAME("x")> () << std::endl;

    construction_control(control);
    construction_test(test);
    
    std::cout << control.x << std::endl;
    std::cout << test.get <NAME("x")> () << std::endl;

    return 0;
}

template <typename T>
void assignment_control (T& in)
{
    in.x = 1;
}

template <typename T>
void assignment_test (T& in)
{
    set <NAME("x")> (in, 1);
}

template <typename T>
int access_control (T& in)
{
    return in.x;
}

template <typename T>
int access_test (T& in)
{
    return get <NAME("x")> (in);
}

template <typename T>
void construction_control (T& in)
{
    T result{1234};
    in = std::move(result);
}

template <typename T>
void construction_test (T& in)
{
    T result{1234};
    in = std::move(result);
}
