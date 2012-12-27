#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>

#include <framework/serializable.hpp>
#include <framework/serializable/mutators/stl_string.hpp>

// Object implementation details
namespace object_impl
{
    // Example of a simple fixed-value object specific field.  This pattern could easily be
    // generalized to a template, but a one-off implementation for a specific object is
    // more illustrative here.
    template <typename T>
    struct version
    {
        // The accessor methods and constructors maintain the invariant version == 5

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

        // Note - this implementation does not use an underlying value type
    };
}

// Widen-value template - stores an underlying arithmetic value type as an integer if possible
template <typename T>
struct widen_value
{
    private:
        enum { widen = std::is_arithmetic <typename T::value_type>::value && sizeof(typename T::value_type) <= sizeof(int) };

        // Note - this implementation redefines the underlying value type
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
};

int main ()
{
    using ::framework::serializable::inline_object;
    using ::framework::serializable::value;
    
    // Define an object type
    using object = inline_object <
        value <NAME("Version"), uint8_t, object_impl::version>,
        value <NAME("Value"), short, widen_value>>;

    // Create an object
    object o1 {5, 2.0};

    o1.get <NAME("Value")> ();

    // Write the object to a file
    assert(write(o1, std::ofstream("filename")));

    // Read the object from a file
    object o2;
    assert(read(std::ifstream("filename"), o2));

    // Check the value
    assert(o1 == o2);

    std::cout << "Original bool size: " << sizeof(inline_object <value <NAME("x"), bool>>) << std::endl;
    std::cout << "Widened bool size: " << sizeof(inline_object <value <NAME("x"), bool, widen_value>>) << std::endl;

    return 0;
}
