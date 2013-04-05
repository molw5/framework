// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

namespace framework
{
    namespace serializable
    {
        namespace detail
        {
            template <typename T, std::size_t N, typename Enabler = void>
            struct has_template_read : std::false_type {};

            template <typename T, std::size_t N>
            struct has_template_read <T, N,
                decltype(std::declval <T> ().template read <N> (std::declval <char*> ()), void())
                > : std::true_type {};

            template <typename T, std::size_t N, typename Enabler = void>
            struct has_template_write : std::false_type {};

            template <typename T, std::size_t N>
            struct has_template_write <T, N,
                decltype(std::declval <T> ().template write <N> (std::declval <char const*> ()), void())
                > : std::true_type {};

            struct direct 
            {
                template <std::size_t N, typename Stream>
                FRAMEWORK_ALWAYS_INLINE
                static typename std::enable_if <
                    has_template_read <Stream, N>::value, 
                    bool
                >::type read (Stream& stream, void* s)
                {
                    return !!(stream.template read <N> (reinterpret_cast <char*> (s)));
                }

                template <std::size_t N, typename Stream>
                FRAMEWORK_ALWAYS_INLINE
                static typename std::enable_if <
                    !has_template_read <Stream, N>::value, 
                    bool
                >::type read (Stream& stream, void* s)
                {
                    return read(stream, s, N);
                }

                template <typename Stream>
                FRAMEWORK_ALWAYS_INLINE
                static bool read (Stream& stream, void* s, std::size_t n)
                {
                    return !!(stream.read(reinterpret_cast <char*> (s), n));
                }

                template <std::size_t N, typename Stream>
                FRAMEWORK_ALWAYS_INLINE
                static typename std::enable_if <
                    has_template_write <Stream, N>::value, 
                    bool
                >::type write (Stream& stream, void const* s)
                {
                    return !!(stream.template write <N> (reinterpret_cast <char const*> (s)));
                }

                template <std::size_t N, typename Stream>
                FRAMEWORK_ALWAYS_INLINE
                static typename std::enable_if <
                    !has_template_write <Stream, N>::value, 
                    bool
                >::type write (Stream& stream, void const* s)
                {
                    return write(stream, s, N);
                }

                template <typename Stream>
                FRAMEWORK_ALWAYS_INLINE
                static bool write (Stream& stream, void const* s, std::size_t n)
                {
                    return !!(stream.write(reinterpret_cast <char const*> (s), n));
                }
            };
        }

        template <typename Input, typename T>
        typename std::enable_if <
            std::is_arithmetic <T>::value,
            bool
        >::type read_dispatch (T*, Input& in, T& out)
        {
            return detail::direct::read <sizeof(out)> (in, &out);
        }
        
        template <typename Output, typename T>
        typename std::enable_if <
            std::is_arithmetic <T>::value,
            bool
        >::type write_dispatch (T*, T in, Output& out)
        {
            return detail::direct::write <sizeof(in)> (out, &in);
        }

        template <std::size_t N, typename Stream>
        bool stream_read (Stream&& stream, void* s)
        {
            return detail::direct::read <N> (stream, s);
        }

        template <typename Stream>
        bool stream_read (Stream&& stream, void* s, std::size_t n)
        {
            return detail::direct::read(stream, s, n);
        }

        template <std::size_t N, typename Stream>
        bool stream_write (Stream&& stream, void const* s)
        {
            return detail::direct::write <N> (stream, s);
        }

        template <typename Stream>
        bool stream_write (Stream&& stream, void const* s, std::size_t n)
        {
            return detail::direct::write(stream, s, n);
        }
    }
}
