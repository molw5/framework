// Copyright (C) 2013 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

namespace framework
{
    namespace protocol_buffers
    {
        template <
            typename Name,
            uint32_t Number,
            typename Specification,
            template <typename> class Implementation,
            typename Input,
            typename Output>
        bool read_dispatch (
            repeated <Name, Number, Specification, Implementation>*,
            Input& in, Output& out, uint8_t format)
        {
            using number = std::integral_constant <uint32_t, Number>;
            using type = make_wire_type <Specification>;
            using wrapper = field_tag <number, type>;
            using value = remove_reference_cv <decltype(serializable::interface <Name> (out).get())>;

            /*static*/ if (is_integer_type <type>::value)
            {
                if (FRAMEWORK_EXPECT_TRUE(format == static_cast <uint8_t> (e_wire_type::length_delimited)))
                {
                    std::size_t size;
                    if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_read <varint <std::size_t>> (in, size)))
                        return false;

                    auto& container = serializable::interface <Name> (out).mutate();
                    auto const begin = in.tellg();
                    while (static_cast <std::size_t> (in.tellg() - begin) < size)
                    {
                        serializable::type_extractor <type> x;
                        if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_read <wrapper> (in, x)))
                            return false;

                        container.push_back(std::move(x));
                    }

                    return static_cast <std::size_t> (in.tellg() - begin) == size;
                }
            }

            if (FRAMEWORK_EXPECT_FALSE(format != static_cast <uint8_t> (wire_type <type>::value)))
                return false;

            serializable::type_extractor <type> x;
            if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_read <wrapper> (in, x)))
                return false;

            serializable::interface <Name> (out).mutate().push_back(std::move(x));
            return true;
        }

        template <
            typename Name,
            uint32_t Number,
            typename Specification,
            template <typename> class Implementation,
            typename Input,
            typename Output>
        bool write_dispatch (
            repeated <Name, Number, Specification, Implementation>*,
            Input const& in, Output& out)
        {
            using number = std::integral_constant <uint32_t, Number>;
            using type = make_wire_type <Specification>;
            using wrapper = field_tag <number, type>;

            auto const& container = serializable::interface <Name> (in).get();
            
            /*static*/ if (is_integer_type <type>::value)
            {
                if (container.size() > 1)
                {
                    using field_tag = fixed_tag <Number, static_cast <uint8_t> (e_wire_type::length_delimited)>;
                    if (FRAMEWORK_EXPECT_FALSE(!write_tag <field_tag> (out)))
                        return false;
            
                    auto const tag = out.tellp();
                    if (FRAMEWORK_EXPECT_FALSE(!fixed_length <5> (0, out)))
                        return false;
                    auto const begin = out.tellp();
                    for (auto const& x : container)
                        if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_write <wrapper> (x, out)))
                            return false;
                    auto const end = out.tellp();

                    auto size = static_cast <std::size_t> (end - begin);
                    if (FRAMEWORK_EXPECT_FALSE(!out.seekp(tag)))
                        return false;
                    if (FRAMEWORK_EXPECT_FALSE(!fixed_length <5> (size, out)))
                        return false;
                    if (FRAMEWORK_EXPECT_FALSE(!out.seekp(end)))
                        return false;

                    return true;
                }
            }

            for (auto const& x : container)
            {
                using field_tag = fixed_tag <Number, static_cast <uint8_t> (wire_type <type>::value)>;
                if (FRAMEWORK_EXPECT_FALSE(!write_tag <field_tag> (out)))
                    return false;
                if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_write <wrapper> (x, out)))
                    return false;
            }

            return true;
        }

        template <
            typename Name,
            uint32_t Number,
            typename Specification,
            template <typename> class Implementation,
            typename Output>
        FRAMEWORK_ALWAYS_INLINE
        bool read_dispatch (
            repeated <Name, Number, Specification, Implementation>*,
            char const* begin, char const* end, char const*& it,
            Output& out, uint8_t format)
        {
            using number = std::integral_constant <uint32_t, Number>;
            using type = make_wire_type <Specification>;
            using wrapper = field_tag <number, type>;
            using value = remove_reference_cv <decltype(serializable::interface <Name> (out).get())>;

            /*static*/ if (is_integer_type <type>::value)
            {
                if (FRAMEWORK_EXPECT_TRUE(format == static_cast <uint8_t> (e_wire_type::length_delimited)))
                {
                    std::size_t size;
                    if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_read <varint <std::size_t>> (begin, end, begin, size)))
                        return false;
                    if (FRAMEWORK_EXPECT_FALSE(end - begin < static_cast <long> (size)))
                        return false;

                    auto& container = serializable::interface <Name> (out).mutate();
                    auto const last = begin + size;
                    while (begin < last)
                    {
                        serializable::type_extractor <type> x;
                        if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_read <wrapper> (begin, end, begin, x)))
                            return false;

                        container.push_back(std::move(x));
                    }

                    if (FRAMEWORK_EXPECT_FALSE(begin != last))
                        return false;

                    it = begin;
                    return true;
                }
            }

            if (FRAMEWORK_EXPECT_FALSE(format != static_cast <uint8_t> (wire_type <type>::value)))
                return false;

            serializable::type_extractor <type> x;
            if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_read <wrapper> (begin, end, begin, x)))
                return false;

            serializable::interface <Name> (out).mutate().push_back(std::move(x));
            it = begin;
            return true;
        }

        template <
            typename Name,
            uint32_t Number,
            typename Specification,
            template <typename> class Implementation,
            typename Input>
        FRAMEWORK_ALWAYS_INLINE
        bool write_dispatch (
            repeated <Name, Number, Specification, Implementation>*,
            Input const& in, 
            char* begin, char* end, char*& it)
        {
            using number = std::integral_constant <uint32_t, Number>;
            using type = make_wire_type <Specification>;
            using wrapper = field_tag <number, type>;

            auto const& container = serializable::interface <Name> (in).get();

            /*static*/ if (is_integer_type <type>::value)
            {
                if (container.size() > 1)
                {
                    enum{ length = static_cast <int> (e_constants::fixed_varint_length) };
                    using tag = fixed_tag <Number, static_cast <uint8_t> (e_wire_type::length_delimited)>;
                    if (FRAMEWORK_EXPECT_FALSE(!write_tag <tag> (begin, end, begin)))
                        return false;
                    
                    auto size_begin = begin;
                    auto const raw_begin = begin = size_begin + length;
                    for (auto const& x : container)
                        if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_write <wrapper> (x, begin, end, begin)))
                            return false;
                    auto const raw_end = begin;

                    if (FRAMEWORK_EXPECT_FALSE(!fixed_length <length> (raw_end - raw_begin, size_begin, raw_begin, size_begin)))
                        return false;

                    it = begin;
                    return true;
                }
            }

            for (auto const& x : container)
            {
                using tag = fixed_tag <Number, static_cast <uint8_t> (wire_type <type>::value)>;
                if (FRAMEWORK_EXPECT_FALSE(!write_tag <tag> (begin, end, begin)))
                    return false;
                if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_write <wrapper> (x, begin, end, begin)))
                    return false;
            }

            it = begin;
            return true;
        }
    }
}
