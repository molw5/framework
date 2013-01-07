// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

namespace framework
{
    namespace serializable
    {
        namespace detail
        {
            template <typename Type, Type Address>
            struct signature_exists : std::true_type
            {
            };

            template <typename Stream, typename Type, typename Enabler = void>
            struct read_signature_exists : std::false_type
            {
            };

            template <typename Stream, typename Type>
            struct read_signature_exists <Stream, Type,
                typename std::enable_if <
                    signature_exists <Type, &Stream::read>::value,
                    void
                >::type>
            {
            };
            
            template <typename Input, typename Output, typename Enabler>
            struct read_type_exists_impl : std::false_type
            {
            };

            template <typename Input, typename Output>
            struct read_type_exists_impl <Input, Output,
                typename std::enable_if <
                    read_signature_exists <Input, bool (Input::*)(Output&)>::value,
                    void
                >::type> : std::true_type
            {
            };

            template <typename Input, std::size_t N, typename Enabler>
            struct read_count_exists_impl : std::false_type
            {
            };

            template <typename Input, std::size_t N>
            struct read_count_exists_impl <Input, N, 
                decltype(
                    std::declval <Input> ().template read <N> (std::declval <char*> ()), 
                    void()
                )> : std::true_type
            {
            };

            template <typename Stream, typename Type, typename Enabler = void>
            struct write_signature_exists : std::false_type
            {
            };

            template <typename Stream, typename Type>
            struct write_signature_exists <Stream, Type,
                typename std::enable_if <
                    signature_exists <Type, &Stream::write>::value,
                    void
                >::type> : std::true_type
            {
            };
            
            template <typename Input, typename Output, typename Enabler>
            struct write_type_exists_impl : std::false_type
            {
            };

            template <typename Input, typename Output>
            struct write_type_exists_impl <Input, Output,
                typename std::enable_if <
                    write_signature_exists <Output, bool (Output::*)(Input const&)>::value ||
                    write_signature_exists <Output, bool (Output::*)(Input)>::value,
                    void
                >::type> : std::true_type
            {
            };

            template <std::size_t N, typename Output, typename Enabler>
            struct write_count_exists_impl : std::false_type
            {
            };

            template <std::size_t N, typename Output>
            struct write_count_exists_impl <N, Output, 
                decltype(
                    std::declval <Output> ().template write <N> (std::declval <char const*> ()), 
                    void()
                )> : std::true_type
            {
            };

#ifndef FRAMEWORK_SERIALIZABLE_NO_FUNDAMENTAL
            template <typename Input, typename Output>
            typename std::enable_if <
                read_type_exists <Input, Output>::value,
                bool
            >::type read (Input& in, Output& out)
            {
                if (!in.read(out))
                    return false;

                return true;
            }
            
            template <typename Input, typename Output>
            typename std::enable_if <
                !read_type_exists <Input, Output>::value &&
                read_count_exists <Input, sizeof(Output)>::value,
                bool
            >::type read (Input& in, Output& out)
            {
                if (!in.template read <sizeof(Output)> (reinterpret_cast <char*> (&out)))
                    return false;

                return true;
            }
            
            template <typename Input, typename Output>
            typename std::enable_if <
                !read_type_exists <Input, Output>::value &&
                !read_count_exists <Input, sizeof(Output)>::value,
                bool
            >::type read (Input& in, Output& out)
            {
                if (!in.read(reinterpret_cast <char*> (&out), sizeof(Output)))
                    return false;

                return true;
            }

            template <typename Input, typename Output>
            typename std::enable_if <
                write_type_exists <Input, Output>::value,
                bool
            >::type write (Input const& in, Output& out)
            {
                if (!out.write(in))
                    return false;

                return true;
            }
            
            template <typename Input, typename Output>
            typename std::enable_if <
                !write_type_exists <Input, Output>::value &&
                write_count_exists <sizeof(Input), Output>::value,
                bool
            >::type write (Input const& in, Output& out)
            {
                if (!out.template write <sizeof(Input)> (reinterpret_cast <char const*> (&in)))
                    return false;

                return true;
            }
            
            template <typename Input, typename Output>
            typename std::enable_if <
                !write_type_exists <Input, Output>::value &&
                !write_count_exists <sizeof(Input), Output>::value,
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
