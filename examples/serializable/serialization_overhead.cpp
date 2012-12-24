#include <sstream>
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

template <typename T, typename Output>
bool write_control (T const& in, Output& out) __attribute__ ((noinline));

template <typename T, typename Output>
bool write_test (T const& in, Output& out) __attribute__ ((noinline));

template <typename Input, typename T>
bool read_control (Input& in, T& out) __attribute__ ((noinline));

template <typename Input, typename T>
bool read_test (Input& in, T& out) __attribute__ ((noinline));

int main ()
{
    s1 const control{1234};
    s2 const test{1234};
    
    s1 out_control{0};
    s2 out_test{0};
        
    std::stringstream ss;

    write_control(control, ss);
    write_test(test, ss);

    read_control(ss, out_control);
    read_test(ss, out_test);

    assert(control.x == out_control.x);
    assert(test == out_test);

    return 0;
}

template <typename T, typename Output>
bool write_control (T const& in, Output& out)
{
    if (!out.write(reinterpret_cast <char const*> (&in.x), sizeof(in.x)))
        return false;

    return true;
}

template <typename T, typename Output>
bool write_test (T const& in, Output& out)
{
    return write(in, out);
}

template <typename Input, typename T>
bool read_control (Input& in, T& out)
{
    // Note the default behaviour of read is to use a temporary in place of
    // out to avoid altering out's state on failure.  We need to reproduce
    // this behaviour here.
    T tmp;
    if (!in.read(reinterpret_cast <char*> (&tmp.x), sizeof(tmp.x)))
        return false;

    out = std::move(tmp);
    return true;
}

template <typename Input, typename T>
bool read_test (Input& in, T& out)
{
    return read(in, out);
}
