// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Fundamental serializable definitions.
*
* Provides the fundamental structures extended by the remainder of this library to support specific 
* types of serialization.  Serialization using this library is provided by the 
* \c serializable_specification structure defined here:
* \code
* serializable_specification <Specification>::read(in, out);
* \endcode
* or through one of the associated helpers:
* \code
* read(in, out);
* \endcode
* It is important to note that the specification provided \c serializable_specification is used
* exclusively to define how the object is to be serialized.  Specifically, there is no fundamental 
* requirement that \c Specification match \c decltype(out) above or similar for \c read.  This 
* allows code to alter how the serialization of a class takes place without altering it's definition - 
* for example, the following is well defined:
*
* \code
struct S1 : serializable <S1,
    value <NAME("Field 1"), int>,
    value <NAME("Field 2"), float>,
    value <NAME("Field 3"), double>,
    value <NAME("Field 4"), short>>
{
} s1 {1, 2.0f, 3.0, 4};

struct S2 : serializable <S2,
    value <NAME("Field 1"), int>,
    value <NAME("Field 3"), double>>
{
} s2 {0, 0};

std::stringstream ss;
serializable_specification <S2>::write(s1, ss);
serializable_specification <S2>::read(ss, s2);
* \endcode
*
* More commonly the alias template may be used to define part of the object's specification for
* use cases similar to the above.  For example, the following code could be used together with
* a checksum stream to compute a header-only checksum.
*
* \code
using Header = alias <
    value <NAME("Field 1"), int>,
    value <NAME("Field 2"), float>>;

struct S1 : serializable <S1, Header,
    value <NAME("Field 3"), double>,
    value <NAME("Field 4"), short>>
{
} s1 {1, 2.0f, 3.0, 4};

checksum_stream chk;
serializable_specification <Header>::write(s1, chk);
* \endcode
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <type_traits>
#include <algorithm>

namespace framework
{
    namespace serializable
    {
        /**
        * \headerfile  base_types.hpp <framework/serializable/containers/base_types.hpp>
        * \brief Default container serializer.
        *
        * Provides default base container type serializers.  The default base container
        * serializers tests must be mutually exclusive - new base container types should wrap
        * this test in a unique \c is_default_NAME_serializable trait.  In addition, an
        * is_default_serializable override must be provided to disable the missing
        * specification/fundamental type specializations provided here.
        *
        * Example: \c container_type enables it's specialization of this template only if
        * \code
        * is_default_container_serializable <T>::value == true
        * \endcode
        * and extends \c is_default_serializable so that
        * \code
        * !is_default_container_serializable <T>::value || is_default_serializable <T>::value
        * \endcode
        *
        * \note
        * No default is provided here; a missing \c serializable_default_specification announced
        * by \c is_default_serializable will trigger an undefined template error.  The implementation
        * below is provided for documentation purposes only.
        *
        * \tparam T container type
        * \tparam Enabler specialization enabler
        */
        template <typename T, typename Enabler = void>
#ifndef DOXYGEN
        struct serializable_default_specification;
#else
        struct serializable_default_specification
        {
            /**
            * Reads the output object from the input stream.  The value returned by this method
            * must be boolean comparable.  Every \c serializable_default_specification must
            * define a read method compatible with this signature to allow proper instantiation
            * from \c serializable_specification's default implementation.
            *
            * \param in input stream
            * \param out output object
            * \return true on success, false on failure
            */
            template <typename Input, typename Output>
            static bool read (Input& in, Output& out);

            /**
            * Writes the input object to the output stream.  The value returned by this method
            * must be boolean comparable.  Every \c serializable_default_specification must
            * define a write method compatible with this signature to allow proper instantiation
            * from \c serializable_specification's default implementation.
            *
            * \param in input object
            * \param out output stream
            * \return true on success, false on failure
            */
            template <typename Input, typename Output>
            static bool write (Input const& in, Output& out);
        };
#endif

        /**
        * \headerfile  base_types.hpp <framework/serializable/containers/base_types.hpp>
        * \brief Serializable specification.
        *
        * Provides the underlying operations required to read/write serializable types.
        * Container types are expected to partially specialize this this template to
        * support additional container types.  Any such specializations are expected
        * to expose a static read and write functions compatible with the definitions
        * given below.
        *
        * This template allows for default handling of fundamental types, container types,
        * value types, and derived types used to provide each with the most common generic 
        * implementation.  See documentation in \c container_type, \c value_type, and 
        * \c serializable_implementation for more information.
        * 
        * \tparam T container type
        */
        template <typename T>
        struct serializable_specification
        {
            /**
            * Performs the actual deserialization of a container type - specializations
            * of serializable_specification are expected to provide a read method
            * compatible with this definition.  The default implementation of this 
            * compatible with this definition.  The implementation provided here forwards
            *
            * \param in input stream
            * \param out output object
            * \return true on success, false on failure
            */
            template <typename Input, typename Output>
            static bool read (Input& in, Output& out)
            {
                return serializable_default_specification <T>::read(in, out);
            }

            /**
            * Performs the actual serialization of a container type - specializations
            * of serializable_specification are expected to provide a write method
            * compatible with this definition.  The implementation provided here forwards
            * the call to \c serializable_default_specification.
            *
            * \param in input object
            * \param out output stream
            * \return true on success, false on failure
            */
            template <typename Input, typename Output>
            static bool write (Input const& in, Output& out)
            {
                return serializable_default_specification <T>::write(in, out);
            }
        };

        /**
        * \headerfile base_types.hpp <framework/serializable/containers/base_types.hpp>
        * \brief Read fowarder.
        *
        * Equivalent to
        * \code
        * serializable_specification <Output>::read(in, out)
        * \endcode
        */
        template <typename Input, typename Output>
        bool read (Input&& in, Output&& out)
        {
            using type = typename std::remove_const <typename std::remove_reference <Output>::type>::type;
            return serializable_specification <type>::read(in, out);
        }

        /**
        * \headerfile base_types.hpp <framework/serializable/containers/base_types.hpp>
        * \brief Write forwarder.
        *
        * Equivalent to
        * \code
        * serializable_specification <Input>::write(in, out)
        * \endcode
        */
        template <typename Input, typename Output>
        bool write (Input&& in, Output&& out)
        {
            using type = typename std::remove_const <typename std::remove_reference <Input>::type>::type;
            return serializable_specification <type>::write(in, out);
        }
        
        /**
        * \headerfile base_types.hpp <framework/serializable/containers/base_types.hpp>
        * \brief Default serializable type trait.
        *
        * Provides a common default serializable check.  The specializations of this trait
        * must be mutually exclusive - new base container types should wrap this test in a 
        * unique \c is_default_NAME_serializable trait.
        *
        * Example: \c container_type enables it's specialization of this template only if
        * \code
        * is_default_container_serializable <T>::value == true
        * \endcode
        * extending \c is_default_serializable so that
        * \code
        * !is_default_container_serializable <T>::value || is_default_serializable <T>::value
        * \endcode
        *
        * \param T container type
        * \tparam Enabler specialization enabler
        */
        template <typename T, typename Enabler = void>
        struct is_default_serializable : std::false_type
        {
        };

#ifndef FRAMEWORK_SERIALIZABLE_NO_FUNDAMENTAL
        /**
        * \brief Default specification for fundamental types.
        */
        template <typename T>
        struct serializable_default_specification <T,
            typename std::enable_if <
                !is_default_serializable <T>::value &&
                std::is_fundamental <T>::value,
                void
            >::type>
        {
            /**
            * Reads the output value as a raw stream of \c sizeof(out) bytes.  Allows for the presence of
            * specialized read functions in the input stream, with the following precedence:
            *   - in.read(out)
            *   - in.read <sizeof(T)> (reinterpret_cast <char*> (out))
            *   - in.read(reinterpret_cast <char*> (out), sizeof (T))
            */
            template <typename Input>
            static bool read (Input& in, T& out);

            /**
            * Writes the input value as a raw stream of \c sizeof(out) bytes.  Allows for the presence of
            * specialized write functions in the output stream, with the following precedence:
            *   - out.write(in)
            *   - out.write <sizeof(T)> (reinterpret_cast <char const*> (in)
            *   - out.write(reinterpret_cast <char const*> (in), sizeof(T))
            */
            template <typename Output>
            static bool write (T const& in, Output& out);
        };
#endif
        
        /**
        * \brief Default specialization for types missing a \em required specification.
        */
        template <typename T>
        struct serializable_default_specification <T, 
            typename std::enable_if <
#ifndef FRAMEWORK_SERIALIZABLE_NO_FUNDAMENTAL
                !std::is_fundamental <T>::value &&
#endif
                !is_default_serializable <T>::value,
                void
            >::type>
        {
            /**
            * \brief Provides a compile time failure when instantiated.
            */
            template <typename Input, typename Output>
            static bool read (Input&, Output&)
            {
                static_assert(!std::is_same <T, T>::value, "Missing serializable_specification definition");
                return false; // suppress warnings
            }

            /**
            * \brief Provides a compile time failure when instantiated.
            */
            template <typename Input, typename Output>
            static bool write (Input&, Output&)
            {
                static_assert(!std::is_same <T, T>::value, "Missing serializable_specification definition");
                return false; // suppress warnings
            }
        };
    }
}

#include <framework/serializable/base_types.inl>
