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
            optional <Name, Number, Specification, Implementation>*,
            Input& in, Output& out, uint8_t format)
        {
            using number = std::integral_constant <uint32_t, Number>;
            using type = make_wire_type <Specification>;
            using wrapper = field_tag <number, type>;

            if (FRAMEWORK_EXPECT_FALSE(format != static_cast <uint8_t> (wire_type <type>::value)))
                return false;

            return serializable::dispatch_read <wrapper> (in, serializable::interface <Name> (out).mutate());
        }

        template <
            typename Name,
            uint32_t Number,
            typename Specification,
            template <typename> class Implementation,
            typename Input,
            typename Output>
        bool write_dispatch (
            optional <Name, Number, Specification, Implementation>*,
            Input const& in, Output& out)
        {
            using number = std::integral_constant <uint32_t, Number>;
            using type = make_wire_type <Specification>;
            using wrapper = field_tag <number, type>;
            using tag = fixed_tag <Number, static_cast <uint8_t> (wire_type <type>::value)>;

            if (FRAMEWORK_EXPECT_FALSE(!serializable::interface <Name> (in).check()))
                return true;
            if (FRAMEWORK_EXPECT_FALSE(!write_tag <tag> (out)))
                return false;

            return serializable::dispatch_write <wrapper> (serializable::interface <Name> (in).get(), out);
        }

        template <
            typename Name,
            uint32_t Number,
            typename Specification,
            template <typename> class Implementation,
            typename Input>
        FRAMEWORK_ALWAYS_INLINE
        bool write_dispatch (
            optional <Name, Number, Specification, Implementation>*,
            Input const& in, max_size_frame& out)
        {
            using number = std::integral_constant <uint32_t, Number>;
            using type = make_wire_type <Specification>;
            using wrapper = field_tag <number, type>;
            using tag = fixed_tag <Number, static_cast <uint8_t> (wire_type <type>::value)>;

            if (FRAMEWORK_EXPECT_FALSE(!write_tag <tag> (out)))
                return false;
            
            return serializable::dispatch_write <wrapper> (serializable::interface <Name> (in).get(), out);
        }
        
        template <
            typename Name,
            uint32_t Number,
            typename Specification,
            template <typename> class Implementation,
            typename Output>
        FRAMEWORK_ALWAYS_INLINE
        bool read_dispatch (
            optional <Name, Number, Specification, Implementation>*,
            char const* begin, char const* end, char const*& it,
            Output&& out, uint8_t format)
        {
            using number = std::integral_constant <uint32_t, Number>;
            using type = make_wire_type <Specification>;
            using wrapper = field_tag <number, type>;

            if (FRAMEWORK_EXPECT_FALSE(format != static_cast <uint8_t> (wire_type <type>::value)))
                return false;

            return serializable::dispatch_read <wrapper> (begin, end, it, serializable::interface <Name> (out).mutate());
        }

        template <
            typename Name,
            uint32_t Number,
            typename Specification,
            template <typename> class Implementation,
            typename Input>
        FRAMEWORK_ALWAYS_INLINE
        bool write_dispatch (
            optional <Name, Number, Specification, Implementation>*,
            Input&& in,
            char* begin, char* end, char*& it)
        {
            using number = std::integral_constant <uint32_t, Number>;
            using type = make_wire_type <Specification>;
            using wrapper = field_tag <number, type>;
            using tag = fixed_tag <Number, static_cast <uint8_t> (wire_type <type>::value)>;

            if (FRAMEWORK_EXPECT_FALSE(!serializable::interface <Name> (in).check()))
                return true;
            if (FRAMEWORK_EXPECT_FALSE(!write_tag <tag> (begin, end, begin)))
                return false;
            if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_write <wrapper> (serializable::interface <Name> (in).get(), begin, end, begin)))
                return false;

            it = begin;
            return true;
        }
    }
}
