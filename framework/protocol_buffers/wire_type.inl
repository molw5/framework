// Copyright (C) 2013 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

namespace framework
{
    namespace protocol_buffers
    {
        namespace detail
        {
            template <typename T, typename Enabler>
            struct is_wire_type_impl
            {
                using type = std::false_type;
            };

            template <typename T>
            struct is_wire_type_impl <T, typename T::protocol_buffers_wire_enabler>
            {
                using type = std::true_type;
            };

            template <typename T>
            struct is_integer_type_impl
            {
                using type =
                    typename std::enable_if <
                        is_wire_type <T>::value,
                        typename std::conditional <
                            wire_type <T>::value == e_wire_type::varint ||
                            wire_type <T>::value == e_wire_type::fixed_32 ||
                            wire_type <T>::value == e_wire_type::fixed_64,
                            std::true_type,
                            std::false_type
                        >::type
                    >::type;
            };

            template <typename T, typename Enabler>
            struct wire_type_impl
            {
                using type = std::integral_constant <e_wire_type, e_wire_type::length_delimited>;
            };

            template <typename T>
            struct wire_type_impl <T, 
                typename std::enable_if <
                    is_wire_type <T>::value, 
                    void
                >::type>
            {
                using type = typename T::protocol_buffers_wire_type;
            };

            template <typename T>
            struct wire_type_impl <T, 
                typename std::enable_if <
                    serializable::is_value_type <T>::value,
                    void
                >::type>
            {
                using type = wire_type <serializable::value_specification <T>>;
            };

            template <typename T>
            struct make_wire_type_impl
            {
                using type =
                    typename std::conditional <
                        is_wire_type <T>::value,
                        T,
                        length_delimited <T>
                    >::type;
            };
        }

        //-----------------------------------------------------------------------------------------
        // Decay paths:
        // These dispatch methods are provided to define the transition from a wrapped protocol 
        // buffers dispatch call to a general serializable dispatch call.
        //-----------------------------------------------------------------------------------------

        template <
            typename Number,
            typename T,
            typename... Args>
        FRAMEWORK_ALWAYS_INLINE
        typename std::enable_if <
            std::is_same <T, float_>::value ||
            std::is_same <T, double_>::value ||
            std::is_same <T, bool_>::value ||
            std::is_same <T, fixed32>::value ||
            std::is_same <T, fixed64>::value ||
            std::is_same <T, sfixed32>::value ||
            std::is_same <T, sfixed64>::value ||
            std::is_same <T, string_>::value ||
            std::is_same <T, bytes>::value ||
            std::is_same <T, int32>::value ||
            std::is_same <T, int64>::value ||
            std::is_same <T, uint32>::value ||
            std::is_same <T, uint64>::value ||
            std::is_same <T, sint32>::value ||
            std::is_same <T, sint64>::value,
            bool
        >::type read_dispatch (
            field_tag <Number, T>*,
            Args&&... args)
        {
            return serializable::dispatch_read <T> (std::forward <Args> (args)...);
        }

        template <
            typename Number,
            typename T,
            typename... Args>
        FRAMEWORK_ALWAYS_INLINE
        typename std::enable_if <
            std::is_same <T, float_>::value ||
            std::is_same <T, double_>::value ||
            std::is_same <T, bool_>::value ||
            std::is_same <T, fixed32>::value ||
            std::is_same <T, fixed64>::value ||
            std::is_same <T, sfixed32>::value ||
            std::is_same <T, sfixed64>::value ||
            std::is_same <T, string_>::value ||
            std::is_same <T, bytes>::value ||
            std::is_same <T, int32>::value ||
            std::is_same <T, int64>::value ||
            std::is_same <T, uint32>::value ||
            std::is_same <T, uint64>::value ||
            std::is_same <T, sint32>::value ||
            std::is_same <T, sint64>::value,
            bool
        >::type write_dispatch (
            field_tag <Number, T>*,
            Args&&... args)
        {
            return serializable::dispatch_write <T> (std::forward <Args> (args)...);
        }

        template <
            typename Number,
            typename T,
            typename... Args>
        FRAMEWORK_ALWAYS_INLINE
        bool read_dispatch (
            field_tag <Number, length_delimited <T>>*,
            Args&&... args)
        {
            return serializable::dispatch_read <length_delimited <T>> (std::forward <Args> (args)...);
        }

        template <
            typename Number,
            typename T,
            typename... Args>
        FRAMEWORK_ALWAYS_INLINE
        bool write_dispatch (
            field_tag <Number, length_delimited <T>>*,
            Args&&... args)
        {
            return serializable::dispatch_write <length_delimited <T>> (std::forward <Args> (args)...);
        }

        //-----------------------------------------------------------------------------------------
        // Raw little endian overrides:
        // The various little-endian protocol buffer types (float_, double_, fixed32, ...) are
        // defined in terms of serializable's little_endian mutator which does not provide 
        // appropriate raw dispatch methods.  Ideally a thin wrapper around the buffer could be
        // used to avoid this duplication, however this approach was found to perform extremely
        // poorly under GCC/clang (4.8.0 20130216/3.3 trunk 174855 respectively).  Neither compiler 
        // was able to effectively remove the abstraction when the dispatch call was effectively
        // flattened - in fact performance was found to differ by an order of magnitude or more.
        //-----------------------------------------------------------------------------------------

        template <
            typename T,
            typename Output>
        FRAMEWORK_ALWAYS_INLINE
        typename std::enable_if <
            std::is_same <T, float_>::value ||
            std::is_same <T, double_>::value ||
            std::is_same <T, fixed32>::value ||
            std::is_same <T, fixed64>::value ||
            std::is_same <T, sfixed32>::value ||
            std::is_same <T, sfixed64>::value,
            bool
        >::type read_dispatch (
            T*,
            char const* begin, char const* end, char const*& it,
            Output&& out)
        {
            enum{ size = sizeof(out) };
            if (FRAMEWORK_EXPECT_FALSE(end - begin < size))
                return false;

            serializable::type_extractor <T> value;
            memcpy(&value, begin, size);
            out = little_to_host(value);
            it = begin + size;
            return true;
        }

        template <
            typename T,
            typename Input>
        FRAMEWORK_ALWAYS_INLINE
        typename std::enable_if <
            std::is_same <T, float_>::value ||
            std::is_same <T, double_>::value ||
            std::is_same <T, fixed32>::value ||
            std::is_same <T, fixed64>::value ||
            std::is_same <T, sfixed32>::value ||
            std::is_same <T, sfixed64>::value,
            bool
        >::type write_dispatch (
            T*,
            Input in,
            char* begin, char* end, char*& it)
        {
            enum{ size = sizeof(in) };

            assert(end - begin >= size);
            in = host_to_little(in);
            memcpy(begin, &in, size);
            it = begin + size;
            return true;
        }

        //-----------------------------------------------------------------------------------------
        // String type overrides:
        // Again, these methods must be duplicated to support raw methods - see raw little endian
        // overrides above.
        //-----------------------------------------------------------------------------------------
        
        template <typename Output>
        FRAMEWORK_ALWAYS_INLINE
        bool read_dispatch (
            string_*,
            char const* begin, char const* end, char const*& it,
            Output&& out)
        {
            std::size_t size;
            if (!serializable::dispatch_read <varint <std::size_t>> (begin, end, begin, size))
                return false;
            if (FRAMEWORK_EXPECT_FALSE(end - begin < static_cast <long> (size)))
                return false;
            
            out.assign(begin, size);
            it = begin + size;
            return true;
        }

        template <typename Input>
        FRAMEWORK_ALWAYS_INLINE
        bool write_dispatch (
            string_*,
            Input&& in,
            char* begin, char* end, char*& it)
        {
            if (!serializable::dispatch_write <varint <std::size_t>> (in.size(), begin, end, begin))
                return false;

            assert(static_cast <std::size_t> (end - begin) >= in.size());
            memcpy(begin, &in[0], in.size());
            it = begin + in.size();
            return true;
        }

        //-----------------------------------------------------------------------------------------
        // Generic length-delimited wrappers:
        // These dispatch methods provide allow serializable definitions to be naturally embedded
        // inside protocol buffer definitions as length-delimited fields.  Note that effective
        // cost of these methods may be prohibitively high depending on the application - see
        // raw little endian overrides above.
        //-----------------------------------------------------------------------------------------

        template <
            typename T, 
            typename Output>
        FRAMEWORK_ALWAYS_INLINE
        bool read_dispatch (
            length_delimited <T>*,
            char const* begin, char const* end, char const*& it,
            Output& out)
        {
            std::size_t size;
            if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_read <varint <std::size_t>> (begin, end, begin, size)))
                return false;
            if (FRAMEWORK_EXPECT_FALSE(end - begin < static_cast <long> (size)))
                return false;

            raw_input_frame frame {begin, begin + size};
            auto const frame_begin = frame.tellg();
            if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_read <T> (frame, out)))
                return false;
            auto const frame_end = frame.tellg();

            if (FRAMEWORK_EXPECT_FALSE(frame_end - frame_begin != size))
                return false;

            it = begin + size;
            return true;
        }

        template <
            typename T, 
            typename Input>
        FRAMEWORK_ALWAYS_INLINE
        bool write_dispatch (
            length_delimited <T>*,
            Input const& in, 
            char* begin, char* end, char*& it)
        {
            enum{ length = static_cast <int> (e_constants::fixed_varint_length) };

#ifdef NDEBUG
            raw_output_frame <false> frame {begin + length, end};
#else
            raw_output_frame <true> frame {begin + length, end};
#endif

            auto frame_begin = frame.tellp();
            if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_write <T> (in, frame)))
                return false;
            auto frame_end = frame.tellp();

            auto const size = frame_end - frame_begin;
            if (FRAMEWORK_EXPECT_FALSE(!fixed_length <length> (size, begin, end, begin)))
                return false;

            it = begin + size;
            return true;
        }

        template <
            typename T, 
            typename Input, 
            typename Output>
        bool read_dispatch (
            length_delimited <T>*,
            Input& in, Output&& out)
        {
            std::size_t size;
            if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_read <varint <std::size_t>> (in, size)))
                return false;

            length_delimited_input_frame <Input> frame {in, size};
            if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_read <T> (frame, out)))
                return false;
            if (FRAMEWORK_EXPECT_FALSE(frame.size() != 0))
                return false;

            return true;
        }

        template <
            typename T, 
            typename Input, 
            typename Output>
        bool write_dispatch (
            length_delimited <T>*,
            Input&& in, Output&& out)
        {
            using serializable::dispatch_write;
            enum{ length = static_cast <int> (e_constants::fixed_varint_length) };

            auto const tag = out.tellp();
            if (FRAMEWORK_EXPECT_FALSE(!fixed_length <length> (0, out)))
                return false;
            auto const begin = out.tellp();
            if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_write <T> (in, out)))
                return false;
            auto const end = out.tellp();

            auto size = static_cast <std::size_t> (end - begin);
            if (FRAMEWORK_EXPECT_FALSE(!out.seekp(tag)))
                return false;
            if (FRAMEWORK_EXPECT_FALSE(!fixed_length <length> (size, out)))
                return false;
            if (FRAMEWORK_EXPECT_FALSE(!out.seekp(end)))
                return false;

            return true;
        }
    }
}
