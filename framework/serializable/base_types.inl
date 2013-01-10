// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

namespace framework
{
    namespace serializable
    {
        namespace detail
        {
            template <typename Input, std::size_t N, typename Enabler>
            struct read_count_exists_impl
            {
                using type = std::false_type;
            };

            template <typename Input, std::size_t N>
            struct read_count_exists_impl <Input, N, 
                decltype(
                    std::declval <Input> ().template read <N> (std::declval <char*> ()), 
                    void()
                )>
            {
                using type = std::true_type;
            };

            template <typename Output, std::size_t N, typename Enabler>
            struct write_count_exists_impl
            {
                using type = std::false_type;
            };

            template <typename Output, std::size_t N>
            struct write_count_exists_impl <Output, N, 
                decltype(
                    std::declval <Output> ().template write <N> (std::declval <char const*> ()), 
                    void()
                )> 
            {
                using type = std::true_type;
            };
        }

        template <typename Specification, typename Input, typename Output>
        bool dispatch_read (Input&& in, Output&& out)
        {
            return dispatch_read(in, out, (Specification*)nullptr);
        }

        template <typename Specification, typename Input, typename Output>
        bool dispatch_write (Input&& in, Output&& out)
        {
            return dispatch_write(in, out, (Specification*)nullptr);
        }

        template <typename Input, typename T>
        bool dispatch_read (Input& in, T& out, T*,
            decltype(custom_read(in, out), void())*)
        {
            return custom_read(in, out);
        }

        template <typename Output, typename T>
        bool dispatch_write (T const& in, Output& out, T*,
            decltype(custom_write(in, out), void())*)
        {
            return custom_write(in, out);
        }

        template <typename Input, typename Output>
        bool read (Input&& in, Output&& out)
        {
            using type = typename std::remove_cv <typename std::remove_reference <Output>::type>::type;
            return dispatch_read <type> (in, out);
        }

        template <typename Input, typename Output>
        bool write (Input&& in, Output&& out)
        {
            using type = typename std::remove_cv <typename std::remove_reference <Input>::type>::type;
            return dispatch_write <type> (in, out);
        }
        
        template <typename Input, typename T>
        bool custom_read (Input& in, T& out, 
            typename std::enable_if <
                std::is_arithmetic <T>::value &&
                read_count_exists <Input, sizeof(T)>::value,
                void
            >::type*)
        {
            if (!in.template read <sizeof(T)> (reinterpret_cast <char*> (&out)))
                return false;

            return true;
        }

        template <typename Input, typename T>
        bool custom_read (Input& in, T& out,
            typename std::enable_if <
                std::is_arithmetic <T>::value &&
                !read_count_exists <Input, sizeof(T)>::value,
                void
            >::type*)
        {
            if (!in.read(reinterpret_cast <char*> (&out), sizeof(out)))
                return false;

            return true;
        }

        template <typename Output, typename T>
        bool custom_write (T const& in, Output& out, 
            typename std::enable_if <
                std::is_arithmetic <T>::value &&
                write_count_exists <Output, sizeof(T)>::value,
                void
            >::type*)
        {
            if (!out.template write <sizeof(in)> (reinterpret_cast <char const*> (&in)))
                return false;

            return true;
        }

        template <typename Output, typename T>
        bool custom_write (T const& in, Output& out, 
            typename std::enable_if <
                std::is_arithmetic <T>::value &&
                !write_count_exists <Output, sizeof(T)>::value,
                void
            >::type*)
        {
            if (!out.write(reinterpret_cast <char const*> (&in), sizeof(in)))
                return false;

            return true;
        }
    }
}
