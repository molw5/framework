#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>

#include <framework/serializable.hpp>
#include <framework/serializable/mutators/stl_string.hpp>

namespace object_impl
{
    template <typename T>
    struct version
    {
        typename T::value_type get () const
        { 
            return 5; 
        }

        void set (typename T::value_type const& x) const
        {
            if (x != 5)
                throw std::runtime_error("Invalid object: Version not supported");
        }

        protected:
            ~version () = default;
            version () = default;

            version (typename T::value_type const& x)
            {
                if (x != 5)
                    throw std::runtime_error("Invalid object: Version not supported");
            }
    };
}

template <typename T>
struct widen_value
{
    private:
        enum { widen = std::is_arithmetic <typename T::value_type>::value && sizeof(typename T::value_type) <= sizeof(int) };

        typename std::conditional <
            widen,
            int,
            typename T::value_type
        >::type p_tValue;

    public:
        typename std::conditional <
            widen,
            typename T::value_type,
            typename T::value_type const&
        >::type get () const
        {
            return p_tValue;
        }

        void set (typename T::value_type x)
        {
            p_tValue = std::move(x);
        }

    protected:
        ~widen_value () = default;
        widen_value () = default;
    
        widen_value (typename T::value_type x)
            : p_tValue(std::move(x))
        {
        }

        // An in-place constructor could be provided here; omitted for brevity.
        // See value_implementation for an example of this.
};

using ::framework::serializable::inline_object;
using ::framework::serializable::value;

using object = inline_object <
    value <NAME("Version"), uint8_t, object_impl::version>,
    value <NAME("Value"), short, widen_value>>;

int main ()
{
    // Create an object
    object o1 {5, 2.0};

    o1.get <NAME("Value")> ();

    // Write the object to a file
    assert(write(o1, std::ofstream("filename")));

    // Read the object from a file
    object o2;
    assert(read(std::ifstream("filename"), o2));

    // Check the value
    //assert(o1 == o2);

    std::cout << "Original bool size: " << sizeof(inline_object <value <NAME("x"), bool>>) << std::endl;
    std::cout << "Widened bool size: " << sizeof(inline_object <value <NAME("x"), bool, widen_value>>) << std::endl;

    return 0;
}
