// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#pragma once

#include <type_traits>
#include <algorithm>

namespace framework
{
    namespace serializable
    {
#ifndef FRAMEWORK_SERIALIZABLE_NO_FUNDAMENTAL
        namespace detail
        {
            template <typename Input, typename Output, typename Enabler = void>
            struct type_read_exists : std::false_type
            {
            };

            template <typename Input, typename Output>
            struct type_read_exists <Input, Output,
                typename std::enable_if <
                    std::is_same <
                        decltype(std::declval <Input> ().read(std::declval <Output&> ())),
                        decltype(std::declval <Input> ().read(std::declval <Output&> ()))
                    >::value
                >::type> : std::true_type
            {
            };

            template <typename Input, typename Output, typename Enabler = void>
            struct count_read_exists : std::false_type
            {
            };

            template <typename Input, typename Output>
            struct count_read_exists <Input, Output,
                typename std::enable_if <
                    std::is_same <
                        decltype(std::declval <Input> ().template write <sizeof (Output)> (std::declval <char*> ())),
                        decltype(std::declval <Input> ().template write <sizeof (Output)> (std::declval <char*> ()))
                    >::value
                >::type> : std::true_type
            {
            };

            template <typename Input, typename Output>
            typename std::enable_if <
                type_read_exists <Input, Output>::value,
                bool
            >::type read (Input& in, Output& out)
            {
                if (!in.read(out))
                    return false;

                return true;
            }
            
            template <typename Input, typename Output>
            typename std::enable_if <
                !type_read_exists <Input, Output>::value &&
                count_read_exists <Input, Output>::value,
                bool
            >::type read (Input& in, Output& out)
            {
                if (!in.template read <sizeof(Output)> (reinterpret_cast <char*> (&out)))
                    return false;

                return true;
            }
            
            template <typename Input, typename Output>
            typename std::enable_if <
                !type_read_exists <Input, Output>::value &&
                !count_read_exists <Input, Output>::value,
                bool
            >::type read (Input& in, Output& out)
            {
                if (!in.read(reinterpret_cast <char*> (&out), sizeof(Output)))
                    return false;

                return true;
            }

            template <typename Input, typename Output, typename Enabler = void>
            struct type_write_exists : std::false_type
            {
            };

            template <typename Input, typename Output>
            struct type_write_exists <Input, Output,
                typename std::enable_if <
                    std::is_same <
                        decltype(std::declval <Output> ().template write <Input> (std::declval <Input const&> ())),
                        decltype(std::declval <Output> ().template write <Input> (std::declval <Input const&> ()))
                    >::value
                >::type> : std::true_type
            {
            };

            template <typename Input, typename Output, typename Enabler = void>
            struct count_write_exists : std::false_type
            {
            };

            template <typename Input, typename Output>
            struct count_write_exists <Input, Output,
                typename std::enable_if <
                    std::is_same <
                        decltype(std::declval <Output> ().template write <sizeof (Input)> (std::declval <char const*> ())),
                        decltype(std::declval <Output> ().template write <sizeof (Input)> (std::declval <char const*> ()))
                    >::value
                >::type> : std::true_type
            {
            };

            template <typename Input, typename Output>
            typename std::enable_if <
                type_write_exists <Input, Output>::value,
                bool
            >::type write (Input const& in, Output& out)
            {
                if (!out.write(in))
                    return false;

                return true;
            }
            
            template <typename Input, typename Output>
            typename std::enable_if <
                !type_write_exists <Input, Output>::value &&
                count_write_exists <Input, Output>::value,
                bool
            >::type write (Input const& in, Output& out)
            {
                if (!out.template write <sizeof(Input)> (reinterpret_cast <char const*> (&in)))
                    return false;

                return true;
            }
            
            template <typename Input, typename Output>
            typename std::enable_if <
                !type_write_exists <Input, Output>::value &&
                !count_write_exists <Input, Output>::value,
                bool
            >::type write (Input const& in, Output& out)
            {
                if (!out.write(reinterpret_cast <char const*> (&in), sizeof(Input)))
                    return false;

                return true;
            }
        }

        template <typename T>
        template <typename Input>
        bool serializable_default_specification <T,
            typename std::enable_if <
                !is_default_serializable <T>::value &&
                std::is_fundamental <T>::value,
                void
            >::type
        >::read (Input& in, T& out)
        {
            return detail::read(in, out);
        }

        template <typename T>
        template <typename Output>
        bool serializable_default_specification <T,
            typename std::enable_if <
                !is_default_serializable <T>::value &&
                std::is_fundamental <T>::value,
                void
            >::type
        >::write (T const& in, Output& out)
        {
            return detail::write(in, out);
        }
#endif
    }
}

