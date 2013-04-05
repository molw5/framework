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
* read_dispatch((Specification*)nullptr, in, out);
* write_dispatch((Specification*)nullptr, in, out);
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
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <type_traits>
#include <algorithm>

#include <framework/common/common_macros.hpp>

namespace framework
{
    namespace serializable
    {
        template <typename T>
        using reference_base = typename std::remove_cv <typename std::remove_reference <T>::type>::type;

        /**
        * \brief Arithmetic read overload.
        */
        template <typename Input, typename T>
        typename std::enable_if <
            std::is_arithmetic <T>::value,
            bool
        >::type read_dispatch (T*, Input& in, T& out);

        /**
        * \brief Arithmetic write overload.
        */
        template <typename Output, typename T>
        typename std::enable_if <
            std::is_arithmetic <T>::value,
            bool
        >::type write_dispatch (T*, T in, Output& out);

        /**
        * \headerfile base_types.hpp <framework/serializable/containers/base_types.hpp>
        * \brief Dispatched read forwarder.
        *
        * Uses the explicitly specified specification to serialize the object.
        */
        template <typename Specification, typename... Args>
        FRAMEWORK_ALWAYS_INLINE
        auto dispatch_read (Args&&... args) ->
        decltype(read_dispatch(
            static_cast <Specification*> (nullptr), 
            std::forward <Args> (args)...))
        {
            return read_dispatch(
                static_cast <Specification*> (nullptr),
                std::forward <Args> (args)...);
        }
        
        /**
        * \headerfile base_types.hpp <framework/serializable/containers/base_types.hpp>
        * \brief Dispatched write forwarder.
        *
        * Uses the explicitly specified specification to serialize the object.
        */
        template <typename Specification, typename... Args>
        FRAMEWORK_ALWAYS_INLINE
        auto dispatch_write (Args&&... args) ->
        decltype(write_dispatch(
            static_cast <Specification*> (nullptr),
            std::forward <Args> (args)...))
        {
            return write_dispatch(
                static_cast <Specification*> (nullptr),
                std::forward <Args> (args)...);
        }

        /**
        * \headerfile base_types.hpp <framework/serializable/containers/base_types.hpp>
        * \brief Stream read forwarder.
        *
        * Used to forward a fixed-byte read call to the underlying stream object.  Equivalent to one
        * of the following two calls, in order of precedence:
        *
        * \code
        * s.read <N> (reinterpret_cast <char*> (out));
        * s.read(reinterpret_cast <char*> (out), N);
        * \endcode
        */
        template <std::size_t N, typename Stream>
        bool stream_read (Stream&& stream, void* s);

        /**
        * \headerfile base_types.hpp <framework/serializable/containers/base_types.hpp>
        * \brief Stream read forwarder.
        *
        * Used to forward a fixed-byte read call to the underlying stream object.  Equivalent to the following:
        *
        * \code
        * s.read(reinterpret_cast <char*> (out), N);
        * \endcode
        */
        template <typename Stream>
        bool stream_read (Stream&& stream, void* s, std::size_t n);

        /**
        * \headerfile base_types.hpp <framework/serializable/containers/base_types.hpp>
        * \brief Stream write forwarder.
        *
        * Used to forward a fixed-byte write call to the underlying stream object.  Equivalent to one
        * of the following two calls, in order of precedence:
        *
        * \code
        * s.write <N> (reinterpret_cast <char const*> (out));
        * s.write(reinterpret_cast <char const*> (out), N);
        * \endcode
        */
        template <std::size_t N, typename Stream>
        bool stream_write (Stream&& stream, void const* s);

        /**
        * \headerfile base_types.hpp <framework/serializable/containers/base_types.hpp>
        * \brief Stream write forwarder.
        *
        * Used to forward a fixed-byte write call to the underlying stream object.  Equivalent to the following:
        *
        * \code
        * s.write <N> (reinterpret_cast <char const*> (out));
        * \endcode
        */
        template <typename Stream>
        bool stream_write (Stream&& stream, void const* s, std::size_t n);

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
        FRAMEWORK_ALWAYS_INLINE
        auto read (Input&& in, Output&& out) ->
        decltype(dispatch_read <reference_base <Output>> (
            std::forward <Input> (in),
            std::forward <Output> (out)))
        {
            return dispatch_read <reference_base <Output>> (
                std::forward <Input> (in),
                std::forward <Output> (out));
        }

        /**
        * \headerfile base_types.hpp <framework/serializable/containers/base_types.hpp>
        * \brief Write forwarder.
        *
        * Uses the input object's type as it's specification.
        */
        template <typename Input, typename Output>
        FRAMEWORK_ALWAYS_INLINE
        auto write (Input&& in, Output&& out) ->
        decltype(dispatch_write <reference_base <Input>> (
            std::forward <Input> (in),
            std::forward <Output> (out)))
        {
            return dispatch_write <reference_base <Input>> (
                std::forward <Input> (in),
                std::forward <Output> (out));
        }
    }
}

#include <framework/serializable/base_types.inl>
