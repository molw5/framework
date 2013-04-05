// Copyright (C) 2013 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

namespace framework
{
    namespace protocol_buffers
    {
        //-----------------------------------------------------------------------------------------
        // Write dispatch methods:
        // Valid paths:
        //  <none/general> -> message <...>
        //  <required/repeated/optional> -> field_tag <..., message <...>>
        //  <required/repeated/optional> -> field_tag <..., group <...>>
        //-----------------------------------------------------------------------------------------

        // <none/general> -> message <...> (stream)
        template <
            typename... Types,
            typename Input,
            typename Output>
        bool write_dispatch (
            message <Types...>*,
            Input&& in, Output&& out)
        {
            using child = serializable::make_alias <sort_pack <pack_container <Types...>, field_less_than>>;
            if (!serializable::dispatch_write <child> (in, out))
                return false;

            return true;
        }

        // <none/general> -> message <...> (raw)
        template <
            typename... Types,
            typename Input>
        bool write_dispatch (
            message <Types...>*,
            Input&& in,
            char* begin, char* end, char*& it)
        {
            using child = serializable::make_alias <sort_pack <pack_container <Types...>, field_less_than>>;
            if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_write <child> (in, begin, end, begin)))
                return false;

            it = begin;
            return true;
        }

        // <required/repeated/optional> -> field_tag <..., message <...>> (stream)
        template <
            typename Number,
            typename... Types,
            typename Input,
            typename Output>
        bool write_dispatch (
            field_tag <Number, message <Types...>>*,
            Input&& in, Output&& out)
        {
            auto const tag = out.tellp();
            if (FRAMEWORK_EXPECT_FALSE(!fixed_length <5> (0, out)))
                return false;
            auto const begin = out.tellp();
            if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_write <message <Types...>> (in, out)))
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

        // <required/repeated/optional> -> field_tag <..., message <...>> (raw)
        template <
            typename Number,
            typename... Types,
            typename Input>
        FRAMEWORK_ALWAYS_INLINE
        bool write_dispatch (
            field_tag <Number, message <Types...>>*,
            Input&& in,
            char* begin, char* end, char*& it)
        {
            enum{ length = static_cast <int> (e_constants::fixed_varint_length) };

            auto tag_begin = begin;
            auto tag_end = begin += length;
            if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_write <message <Types...>> (in, begin, end, begin)))
                return false;

            auto size = static_cast <std::size_t> (begin - tag_end);
            if (FRAMEWORK_EXPECT_FALSE(!fixed_length <length> (size, tag_begin, tag_end, tag_begin)))
                return false;

            it = begin;
            return true;
        }

        // <required/repeated/optional> -> field_tag <..., group <...>> (stream)
        template <
            typename Number,
            typename... Types,
            typename Input,
            typename Output>
        bool write_dispatch (
            field_tag <Number, group <Types...>>*,
            Input&& in, Output&& out)
        {
            using tag = fixed_tag <Number::value, static_cast <uint8_t> (e_wire_type::end_group)>;

            if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_write <message <Types...>> (in, out)))
                return false;
            if (FRAMEWORK_EXPECT_FALSE(!write_tag <tag> (out)))
                return false;

            return true;
        }

        // <required/repeated/optional> -> field_tag <..., group <...>> (raw)
        template <
            typename Number,
            typename... Types,
            typename Input>
        FRAMEWORK_ALWAYS_INLINE
        bool write_dispatch (
            field_tag <Number, group <Types...>>*,
            Input&& in,
            char* begin, char* end, char*& it)
        {
            using tag = fixed_tag <Number::value, static_cast <uint8_t> (e_wire_type::end_group)>;

            if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_write <message <Types...>> (in, begin, end, begin)))
                return false;
            if (FRAMEWORK_EXPECT_FALSE(!write_tag <tag> (begin, end, begin)))
                return false;

            it = begin;
            return true;
        }

        //-----------------------------------------------------------------------------------------
        // Read dispatch methods:
        // Valid paths:
        //  <none/general> -> message <...>
        //  <required/repeated/optional> -> field_tag <..., message <...>>
        //  <required/repeated/optional> -> field_tag <..., group <...>>
        //-----------------------------------------------------------------------------------------

        // <none/general> -> message <...> (stream)
        template <
            typename... Types,
            typename Input,
            typename Output>
        bool read_dispatch (
            message <Types...>*,
            Input&& in, Output&& out)
        {
            using fields = sort_pack <pack_container <Types...>, field_less_than>;
            using required_fields = filter_pack <fields, is_required>;

            uint64_t field_tag;
            std::bitset <required_fields::size> required;
            while (serializable::dispatch_read <uint64> (in, field_tag))
                if (FRAMEWORK_EXPECT_FALSE(!detail::message_switch <required_fields, fields>::run(
                        field_tag >> 3, field_tag & 0x7, 
                        required, 
                        in, out)))
                    return false;

            return required.all();
        }

        // <none/general> -> message <...> (raw)
        template <
            typename... Types,
            typename Output>
        bool read_dispatch (
            message <Types...>*,
            char const* begin, char const* end, char const*& it,
            Output&& out)
        {
            using fields = sort_pack <pack_container <Types...>, field_less_than>;
            using required_fields = filter_pack <fields, is_required>;

            std::bitset <required_fields::size> required;
            while (begin != end)
            {
                uint64_t field_tag;
                if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_read <uint64> (begin, end, begin, field_tag)))
                    return false;
                if (FRAMEWORK_EXPECT_FALSE(!detail::message_switch <required_fields, fields>::run(
                        field_tag >> 3, field_tag & 0x7, 
                        required, 
                        begin, end, begin, 
                        out)))
                    return false;
            }

            it = begin;
            return required.all();
        }

        // <required/repeated/optional> -> field_tag <..., message <...>> (stream)
        template <
            typename Number,
            typename... Types,
            typename Input,
            typename Output>
        bool read_dispatch (
            field_tag <Number, message <Types...>>*,
            Input&& in, Output&& out)
        {
            using fields = sort_pack <pack_container <Types...>, field_less_than>;
            using required_fields = filter_pack <fields, is_required>;

            uint64_t size;
            if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_read <uint64> (in, size)))
                return false;

            std::bitset <required_fields::size> required;
            auto const begin = in.tellg();
            while (in.tellg() - begin < size)
            {
                uint64_t field_tag;
                if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_read <uint64> (in, field_tag)))
                    return false;
                if (FRAMEWORK_EXPECT_FALSE(!detail::message_switch <required_fields, fields>::run(
                        field_tag >> 3, field_tag & 0x7,
                        required, 
                        in, out)))
                    return false;
            }

            if (FRAMEWORK_EXPECT_FALSE(in.tellg() - begin != size))
                return false;

            return required.all();
        }

        // <required/repeated/optional> -> field_tag <..., message <...>> (raw)
        template <
            typename Number,
            typename... Types,
            typename Output>
        bool read_dispatch (
            field_tag <Number, message <Types...>>*,
            char const* begin, char const* end, char const*& it,
            Output&& out)
        {
            using fields = sort_pack <pack_container <Types...>, field_less_than>;
            using required_fields = filter_pack <fields, is_required>;

            uint64_t size;
            if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_read <uint64> (begin, end, begin, size)))
                return false;

            std::bitset <required_fields::size> required;
            auto const last = begin + size;
            while (begin < last)
            {
                uint64_t field_tag;
                if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_read <uint64> (begin, last, begin, field_tag)))
                    return false;
                if (FRAMEWORK_EXPECT_FALSE(!detail::message_switch <required_fields, fields>::run(
                        field_tag >> 3, field_tag & 0x7,
                        required,
                        begin, last, begin,
                        out)))
                    return false;
            }

            if (FRAMEWORK_EXPECT_FALSE(begin != last))
                return false;

            it = begin;
            return required.all();
        }

        // <required/repeated/optional> -> field_tag <..., group <...>> (stream)
        template <
            typename Number,
            typename... Types,
            typename Input,
            typename Output>
        bool read_dispatch (
            field_tag <Number, group <Types...>>*,
            Input&& in, Output&& out)
        {
            using end_tag = fixed_tag <Number::value, static_cast <uint8_t> (e_wire_type::end_group)>;
            using fields = sort_pack <pack_container <Types...>, field_less_than>;
            using required_fields = filter_pack <fields, is_required>;

            uint64_t field_tag;
            std::bitset <required_fields::size> required;
            while (serializable::dispatch_read <uint64> (in, field_tag))
            {
                if (field_tag == end_tag::buffer)
                    return required.all();
                if (FRAMEWORK_EXPECT_FALSE(!detail::message_switch <required_fields, fields>::run(
                        field_tag >> 3, field_tag & 0x7,
                        required,
                        in, out)))
                    return false;
            }

            return false;
        }

        // <required/repeated/optional> -> field_tag <..., group <...>> (raw)
        template <
            typename Number,
            typename... Types,
            typename Output>
        bool read_dispatch (
            field_tag <Number, group <Types...>>*,
            char const* begin, char const* end, char const*& it,
            Output&& out)
        {
            using end_tag = fixed_tag <Number::value, static_cast <uint8_t> (e_wire_type::end_group)>;
            using fields = sort_pack <pack_container <Types...>, field_less_than>;
            using required_fields = filter_pack <fields, is_required>;

            std::bitset <required_fields::size> required;
            while (!expect_tag <end_tag> (begin, end, begin))
            {
                uint64_t field_tag;
                if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_read <uint64> (begin, end, begin, field_tag)))
                    return false;
                if (FRAMEWORK_EXPECT_FALSE(!detail::message_switch <required_fields, fields>::run(
                        field_tag >> 3, field_tag & 0x7, 
                        required, 
                        begin, end, begin,
                        out)))
                    return false;
            }

            it = begin;
            return required.all();
        }
    }
}
