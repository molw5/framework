// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Fundamental serializable definitions.
*
* Provides the fundamental definitions extended by the remainder of this library to support specific
* types of serialization.  Marshalling of data is performed using a collection of read/write method
* overloads, invoked as follows:
*
* \code
* dispatch_read <Specification> (in, out);
* dispatch_write <Specification> (in, out);
* \endcode
*
* or through the following equivalent syntax:
*
* \code
* dispatch_read(in, out, (Specification*)nullptr);
* dispatch_write(in, out, (Specification*)nullptr);
* \endcode
* 
* It is important to note that the Specification provided to the above is used exclusively to define
* how the object is to be serialized.  Specifically, there is no fundamental requirement that 
* \c Specification match the associated input or output object above  This allows code to alter how 
* the serialization of a class takes place without altering it's definition - for example, the following 
* is well defined:
*
* \code
* struct S1 : serializable <S1,
*     value <NAME("Field 1"), int>,
*     value <NAME("Field 2"), float>,
*     value <NAME("Field 3"), double>,
*     value <NAME("Field 4"), short>>
* {
* } s1 {1, 2.0f, 3.0, 4};
* 
* struct S2 : serializable <S2,
*     value <NAME("Field 1"), int>,
*     value <NAME("Field 3"), double>>
* {
* } s2 {0, 0};
* 
* std::stringstream ss;
* dispatch_write <S2> (s1, ss);
* dispatch_read <S2> (ss, s2);
* \endcode
*
* More commonly the alias template may be used to define part of the object's specification for
* use cases similar to the above.  For example, the following computes a header-only checksum of
* the associated object:
*
* \code
* using Header = alias <
*     value <NAME("Field 1"), int>,
*     value <NAME("Field 2"), float>>;
*
* struct S1 : serializable <S1, Header,
*     value <NAME("Field 3"), double>,
*     value <NAME("Field 4"), short>>
* {
* } s1 {1, 2.0f, 3.0, 4};
*
* internet_checksum chk;
* dispatch_write <Header> (s1, chk);
* \endcode
*
* Note that template partial specialization was originally used here to discriminate between types - 
* this solution required great care in the placement of new definitions to avoid violating the one
* definition rule.  This effectively eliminated what was seen as one of the stronger side effects
* of this sort of serialization - the ability to incrementally improve marshalling functions based
* on the profiled performance of an application.  For example, consider the following type:
*
* \code
* ...
* value <NAME("x"), stl_vector <little_endian <uint32_t>, little_endian <uint32_t>>>,
* ...
* \encode
*
* For a sufficiently large vector, "x" could conceivably dominate the serialization time in a
* particular application as each data member is marshalled individually.  With tagged dispatch
* the solution is trivial; add an overload:
*
* \code
* template <typename Input, typename Output, typename Size>
* bool dispatch_read (Input& in, Output& out, stl_vector <Size, little_endian <uint32_t>>*)
* {
*     // apply optimizations based on the endianness of the host
*     ...
* }
* \endcode
*
* The above overload will now take precedence over the default stl_vector read definition in any
* translation unit where we define the above; the two definitions may coexist within an application.  
* Contrast this to template partial specialization where a solution similar to the above may still be 
* used with the caveat that it \em must always take precedence.  In practice, this produces highly
* fragile code and effectively eliminates this optimization path.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <type_traits>
#include <algorithm>

// Intended for experimentation only; should not be used.  Generally the compiler's optimizer should be 
// allowed to control which dispatch methods to inline.
#ifdef FRAMEWORK_DISPATCH_FORCE_INLINE
    #ifdef __GNUC__
        #define FRAMEWORK_DISPATCH_TAG __attribute__((always_inline))
    #else
        #error "Unknown compiler"
    #endif
#else
    #define FRAMEWORK_DISPATCH_TAG
#endif

namespace framework
{
    namespace serializable
    {
        namespace detail
        {
            template <typename Input, std::size_t N, typename Enabler = void>
            struct read_count_exists_impl;

            template <typename Output, std::size_t N, typename Enabler = void>
            struct write_count_exists_impl;
        }

        /**
        * Determines if a read method is present in Input with a signature compatible with the following:
        *
        * \code
        * std::declval <Input> ().read <N> (std::declval <char*> ())
        * \endcode
        */
        template <typename Input, std::size_t N>
        using read_count_exists = typename detail::read_count_exists_impl <Input, N>::type;

        /**
        * Determines if a write method is present in Output with a signature compatible with the following:
        *
        * \code
        * std::declval <Output> ().read <N> (std::declval <char const*> ())
        * \endcode
        */
        template <typename Output, std::size_t N>
        using write_count_exists= typename detail::write_count_exists_impl <Output, N>::type;

        /**
        * \brief Arithmetic read overload.
        */
        template <typename Input, typename T>
        FRAMEWORK_DISPATCH_TAG
        bool custom_read (Input& in, T& out, 
            typename std::enable_if <
                std::is_arithmetic <T>::value &&
                read_count_exists <Input, sizeof(T)>::value,
                void
            >::type* = nullptr);

        /**
        * \brief Arithmetic read overload.
        */
        template <typename Input, typename T>
        FRAMEWORK_DISPATCH_TAG
        bool custom_read (Input& in, T& out,
            typename std::enable_if <
                std::is_arithmetic <T>::value &&
                !read_count_exists <Input, sizeof(T)>::value,
                void
            >::type* = nullptr);

        /**
        * \brief Arithmetic write overload.
        */
        template <typename Output, typename T>
        FRAMEWORK_DISPATCH_TAG
        bool custom_write (T const& in, Output& out,
            typename std::enable_if <
                std::is_arithmetic <T>::value &&
                write_count_exists <Output, sizeof(T)>::value,
                void
            >::type* = nullptr);

        /**
        * \brief Arithmetic write overload.
        */
        template <typename Output, typename T>
        FRAMEWORK_DISPATCH_TAG
        bool custom_write (T const& in, Output& out,
            typename std::enable_if <
                std::is_arithmetic <T>::value &&
                !write_count_exists <Output, sizeof(T)>::value,
                void
            >::type* = nullptr);

        /**
        * \headerfile base_types.hpp <framework/serializable/containers/base_types.hpp>
        * \brief Dispatched read forwarder.
        *
        * Uses the explicitly specified specification to serialize the object, equivalent to:
        *
        * \code
        * dispatch_read(in, out, (Specification*)nullptr);
        * \endcode
        */
        template <typename Specification, typename Input, typename Output>
        FRAMEWORK_DISPATCH_TAG
        bool dispatch_read (Input&& in, Output&& out);

        /**
        * \headerfile base_types.hpp <framework/serializable/containers/base_types.hpp>
        * \brief Dispatched write forwarder.
        *
        * Uses the explicitly specified specification to serialize the object, equivalent to:
        *
        * \code
        * dispatch_write(in, out, (Specification*)nullptr);
        * \endcode
        */
        template <typename Specification, typename Input, typename Output>
        FRAMEWORK_DISPATCH_TAG
        bool dispatch_write (Input&& in, Output&& out);

        /**
        * \headerfile base_types.hpp <framework/serializable/containers/base_types.hpp>
        * \brief Dispatched read terminator.
        *
        * Transfers control to an appropriate \c custom_read overload, if one exists.
        */
        template <typename Input, typename T>
        FRAMEWORK_DISPATCH_TAG
        bool dispatch_read (Input& in, T& out, T*,
            decltype(custom_read(in, out), void())* = nullptr);

        /**
        * \headerfile base_types.hpp <framework/serializable/containers/base_types.hpp>
        * \brief Dispatched write terminator.
        *
        * Transfers control to an appropriate \c custom_write overload, if one exists.
        */
        template <typename Output, typename T>
        FRAMEWORK_DISPATCH_TAG
        bool dispatch_write (T const& in, Output& out, T*,
            decltype(custom_write(in, out), void())* = nullptr);

        /**
        * \headerfile base_types.hpp <framework/serializable/containers/base_types.hpp>
        * \brief Read forwarder.
        *
        * Uses the output object's type as it's specification, generally equivalent to:
        *
        * \code
        * read <decltype(out)> (in, out);
        * \endcode
        */
        template <typename Input, typename Output>
        FRAMEWORK_DISPATCH_TAG
        bool read (Input&& in, Output&& out);

        /**
        * \headerfile base_types.hpp <framework/serializable/containers/base_types.hpp>
        * \brief Write forwarder.
        *
        * Uses the input object's type as it's specification.
        */
        template <typename Input, typename Output>
        FRAMEWORK_DISPATCH_TAG
        bool write (Input&& in, Output&& out);
    }
}

#include <framework/serializable/base_types.inl>
