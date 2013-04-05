// Copyright (C) 2013 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Optional field type.
*
* \copyright
* Copyright &copy; 2013 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <bitset>
#include <atomic>

#include <framework/common/common_macros.hpp>
#include <framework/serializable/common_macros.hpp>
#include <framework/serializable/containers/value.hpp>
#include <framework/serializable/containers/alias.hpp>
#include <framework/protocol_buffers/base_types.hpp>
#include <framework/protocol_buffers/fixed_tag.hpp>
#include <framework/protocol_buffers/wire_type.hpp>
#include <framework/protocol_buffers/field_type.hpp>
#include <framework/protocol_buffers/message_switch.hpp>

namespace framework
{
    namespace protocol_buffers
    {
        /**
        * \brief Message type.
        */
        template <typename... Types>
        class message : 
            public serializable::custom_serializable <message <Types...>, Types...>,
            public serializable::comparable <message <Types...>>,
            public wire_type_base <e_wire_type::length_delimited>
        {
            private:
                DEFINE_BASE_TEMPLATE(message <Types...>);
                using serializable_base = serializable::custom_serializable <message <Types...>, Types...>;

            public:
                message () = default;
                message (message const&) = default;
                message (message&&) = default;
                message& operator= (message const&) = default;
                message& operator= (message&&) = default;

                /**
                * \brief Base constructor forwarder.
                */
                template <typename... Args>
                message (Args&&... args)
                    : serializable_base(std::forward <Args> (args)...)
                {
                }

                /**
                * \brief Explicit default constructor overload.
                */
                message (std::tuple <>)
                    : message ()
                {
                }
                
                /**
                * \brief Tuple constructor.
                */
                template <typename... Args>
                message (std::tuple <Args...>&& args)
                    : message (
                        std::forward <std::tuple <Args...>> (args), 
                        static_cast <make_indices <sizeof... (Args)>*> (nullptr))
                {
                }

            private:
                template <typename... Args, typename... Indices>
                message (std::tuple <Args...> args, pack_container <Indices...>*)
                    : serializable_base(std::forward <Args> (std::get <Indices::value> (args))...)
                {
                }

           public:
                /**
                * \brief Get forwarder.
                */
                template <typename Name, typename... Args>
                auto get (Args&&... args) const ->
                decltype(std::declval <base <Name>> ().get(std::forward <Args> (args)...))
                {
                    return base <Name>::get(std::forward <Args> (args)...);
                }

                /**
                * \brief Set forwarder.
                */
                template <typename Name, typename... Args>
                auto set (Args&&... args) ->
                decltype(std::declval <base <Name>> ().set(std::forward <Args> (args)...))
                {
                    return base <Name>::set(std::forward <Args> (args)...);
                }

                /**
                * \brief Check forwarder.
                */
                template <typename Name, typename... Args>
                auto check (Args&&... args) const ->
                decltype(std::declval <base <Name>> ().check(std::forward <Args> (args)...))
                {
                    return base <Name>::check(std::forward <Args> (args)...);
                }

                /**
                * \brief Clear forwarder.
                */
                template <typename Name, typename... Args>
                auto clear (Args&&... args) ->
                decltype(std::declval <base <Name>> ().clear(std::forward <Args> (args)...))
                {
                    return base <Name>::clear(std::forward <Args> (args)...);
                }

                /**
                * \brief Mutate forwarder.
                */
                template <typename Name, typename... Args>
                auto mutate (Args&&... args) ->
                decltype(std::declval <base <Name>> ().mutate(std::forward <Args> (args)...))
                {
                    return base <Name>::mutate(std::forward <Args> (args)...);
                }
        };

        /**
        * \brief Group type.
        */
        template <typename... Types>
        class group : 
            public serializable::custom_serializable <group <Types...>, Types...>,
            public serializable::comparable <group <Types...>>,
            public wire_type_base <e_wire_type::start_group>
        {
            private:
                DEFINE_BASE_TEMPLATE(group <Types...>);
                using serializable_base = serializable::custom_serializable <group <Types...>, Types...>;

            public:
                group () = default;
                group (group const&) = default;
                group (group&&) = default;
                group& operator= (group const&) = default;
                group& operator= (group&&) = default;

                /**
                * \brief Base constructor forwarder.
                */
                template <typename... Args>
                group (Args&&... args)
                    : serializable_base(std::forward <Args> (args)...)
                {
                }

                /**
                * \brief Explicit default constructor overload.
                */
                group (std::tuple <>)
                    : group ()
                {
                }
                
                /**
                * \brief Tuple constructor.
                */
                template <typename... Args>
                group (std::tuple <Args...>&& args)
                    : group (
                        std::forward <std::tuple <Args...>> (args), 
                        static_cast <make_indices <sizeof... (Args)>*> (nullptr))
                {
                }

            private:
                template <typename... Args, typename... Indices>
                group (std::tuple <Args...> args, pack_container <Indices...>*)
                    : serializable_base(std::forward <Args> (std::get <Indices::value> (args))...)
                {
                }

           public:
                /**
                * \brief Get forwarder.
                */
                template <typename Name, typename... Args>
                auto get (Args&&... args) const ->
                decltype(std::declval <base <Name>> ().get(std::forward <Args> (args)...))
                {
                    return base <Name>::get(std::forward <Args> (args)...);
                }

                /**
                * \brief Set forwarder.
                */
                template <typename Name, typename... Args>
                auto set (Args&&... args) ->
                decltype(std::declval <base <Name>> ().set(std::forward <Args> (args)...))
                {
                    return base <Name>::set(std::forward <Args> (args)...);
                }

                /**
                * \brief Check forwarder.
                */
                template <typename Name, typename... Args>
                auto check (Args&&... args) const ->
                decltype(std::declval <base <Name>> ().check(std::forward <Args> (args)...))
                {
                    return base <Name>::check(std::forward <Args> (args)...);
                }

                /**
                * \brief Clear forwarder.
                */
                template <typename Name, typename... Args>
                auto clear (Args&&... args) ->
                decltype(std::declval <base <Name>> ().clear(std::forward <Args> (args)...))
                {
                    return base <Name>::clear(std::forward <Args> (args)...);
                }
                
                /**
                * \brief Mutate forwarder.
                */
                template <typename Name, typename... Args>
                auto mutate (Args&&... args) ->
                decltype(std::declval <base <Name>> ().mutate(std::forward <Args> (args)...))
                {
                    return base <Name>::mutate(std::forward <Args> (args)...);
                }
        };

        /**
        * \brief Write to string.
        *
        * Serializes the object to a string using a SizeFrame object to asses the object's size.
        *
        * \return true on success, false on failure
        * \tparam SizeFrame used to assess the object's size
        * \param in input object
        * \param out output string
        */
        template <typename SizeFrame, typename Input, typename Output>
        bool write_to_string (Input&& in, Output&& out)
        {
            using type = typename std::remove_cv <typename std::remove_reference <Input>::type>::type;

            SizeFrame size;
            if (FRAMEWORK_EXPECT_FALSE(!serializable::write(in, size)))
                return false;

            out.resize(static_cast <std::size_t> (size));
            auto const begin = &out[0];
            auto const end = &out[out.size()];
            auto it = begin;
            if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_write <type> (in, begin, end, it)))
                return false;

            out.resize(static_cast <std::size_t> (it - begin));
            return true;
        }

        /**
        * \brief Write to frame.
        *
        * Serializes the object to a string though a raw frame, using a SizeFrame object to asses the object's size.
        *
        * \return true on success, false on failure
        * \tparam SizeFrame used to assess the object's size
        * \param in input object
        * \param out output string
        */
        template <typename SizeFrame, typename Input, typename Output>
        bool write_to_frame (Input&& in, Output&& out)
        {
            SizeFrame size;
            if (FRAMEWORK_EXPECT_FALSE(!serializable::write(in, size)))
                return false;

            std::size_t serialized_size;
            out.resize(static_cast <std::size_t> (size));

            // Destroy the frame before invalidating the iterators
            {
#ifdef NDEBUG
                raw_output_frame <false> frame {&out[0], &out[out.size()]};
#else
                raw_output_frame <true> frame {&out[0], &out[out.size()]};
#endif
                auto const begin = frame.tellp();
                if (FRAMEWORK_EXPECT_FALSE(!serializable::write(in, frame)))
                    return false;
                auto const end = frame.tellp();
                serialized_size = static_cast <std::size_t> (end - begin);
            }

            out.resize(serialized_size);
            return true;
        }

        /**
        * \brief Fast write to string.
        *
        * Serializes the protocol buffer object to a string, using max_size_frame to asses the object's size.
        * This method can be significantly faster than write_to_array_fast, however the amount of memory allocated
        * to the string may be as much as an order of magnitude more than write_to_array_tight.
        * 
        * \return true on success, false on failure
        * \param in input object
        * \param out output string
        */
        template <typename Input, typename Output>
        bool write_to_string_fast (Input&& in, Output&& out)
        {
            return write_to_string <max_size_frame> (std::forward <Input> (in), std::forward <Output> (out));
        }

        /**
        * \brief Tight write to string.
        *
        * Serializes the protocol buffer object to a string, using size_frame to assess the object's size.
        * This method uses significantly less memory than write_to_array_fast, however the serialization time may
        * be as much as twice that of write_to_string_fast.
        *
        * \return true on success, false on failure
        * \param in input object
        * \param out output string
        */
        template <typename Input, typename Output>
        bool write_to_string_tight (Input&& in, Output&& out)
        {
            return write_to_string <size_frame> (std::forward <Input> (in), std::forward <Output> (out));
        }

        /**
        * \brief Fast write to frame.
        *
        * As write_to_string_fast but writes to the string through a raw_input_frame object.  When mixing arbitrary
        * serializable types with protocol buffer specifications the overhead associated with creating an intermediate
        * frame object at each of these transition points was found to be substantial on the compilers benchmarked
        * (clang++ 3.3 - 178146 / g++ 4.7.2 - 20121109).  This variant uses a single global frame object for the
        * write rather than a series of local objects - performance implications will be highly workload dependent.
        *
        * \return true on success, false on failure
        * \param in input object
        * \param out output string
        */
        template <typename Input, typename Output>
        bool write_to_frame_fast (Input&& in, Output&& out)
        {
            return write_to_frame <max_size_frame> (std::forward <Input> (in), std::forward <Output> (out));
        }

        /**
        * \brief Fast write to tight.
        *
        * As write_to_string_tight but writes to the string through a raw_input_frame object.  When mixing arbitrary
        * serializable types with protocol buffer specifications the overhead associated with creating an intermediate
        * frame object at each of these transition points was found to be substantial on the compilers benchmarked
        * (clang++ 3.3 - 178146 / g++ 4.7.2 - 20121109).  This variant uses a single global frame object for the
        * write rather than a series of local objects - performance implications will be highly workload dependent.
        *
        * \return true on success, false on failure
        * \param in input object
        * \param out output string
        */
        template <typename Input, typename Output>
        bool write_to_frame_tight (Input&& in, Output&& out)
        {
            return write_to_frame <size_frame> (std::forward <Input> (in), std::forward <Output> (out));
        }

        /**
        * \brief Write to array.
        *
        * Serializes the object to a fixed-length byte array.  The size of the serialized object is stored in n on success.
        *
        * \return true on success, false on failure
        * \param in input object
        * \param s output byte array
        * \param n byte array size 
        */
        template <typename Input>
        bool write_to_array (Input&& in, char* s, std::size_t& n)
        {
            using type = typename std::remove_cv <typename std::remove_reference <Input>::type>::type;

            max_size_frame max_size;
            if (FRAMEWORK_EXPECT_FALSE(!write(in, max_size)))
                return false;
            if (FRAMEWORK_EXPECT_FALSE(static_cast <std::size_t> (max_size) > n))
            {
                size_frame size;
                if (FRAMEWORK_EXPECT_FALSE(!write(in, size)))
                    return false;
                if (FRAMEWORK_EXPECT_FALSE(static_cast <std::size_t> (size) > n))
                    return false;
            }

            char* const begin = &s[0];
            char* const end = &s[n];
            char* it = begin;
            if (FRAMEWORK_EXPECT_FALSE(!serializable::dispatch_write <type> (in, begin, end, it)))
                return false;

            n = static_cast <std::size_t> (it - begin);
            return true;
        }
        
        /**
        * \brief Write to ostream.
        *
        * Serializes the object to a std::ostream.  This method may be preferred over a serializable::write operation
        * to avoid instantiating multiple templates for ostream types.
        *
        * \return true on success, false on failure
        * \param in input object
        * \param out output ostream
        */
        template <typename Input>
        bool write_to_ostream (Input&& in, std::ostream& out)
        {
            return serializable::write(std::forward <Input> (in), out);
        }

        /**
        * \brief Read from array.
        *
        * Deserializes the object from a byte array.
        *
        * \return true on success, false on failure
        * \param s input byte array
        * \param n byte array size
        * \param out output object
        */
        template <typename Output>
        bool read_from_array (char const* s, std::size_t n, Output&& out)
        {
            using type = typename std::remove_cv <typename std::remove_reference <Output>::type>::type;

            char const* const begin = s;
            char const* const end = s+n;
            char const* it = begin;
            
            return serializable::dispatch_read <type> (begin, end, it, std::forward <Output> (out));
        }

        /**
        * \brief Read from string.
        *
        * Deserializes the object from a string.
        *
        * \return true on success, false on failure
        * \param in input string
        * \param out output object
        */
        template <typename Input, typename Output>
        bool read_from_string (Input&& in, Output&& out)
        {
            return read_from_array(&in[0], in.size(), std::forward <Output> (out));
        }

        /**
        * \brief Read from frame.
        *
        * Deserializes the object from a string through a raw_input_frame.  Similar performance considerations as
        * described in write_to_frame_fast hold here.
        *
        * \return true on success, false on failure
        * \param in input string
        * \param out output object
        */
        template <typename Input, typename Output>
        bool read_from_frame (Input&& in, Output&& out)
        {
            raw_input_frame frame {&in[0], &in[in.size()]};
            return serializable::read(frame, out);
        }
        
        /**
        * \brief Read from istream.
        *
        * Deserializes the object from a std::istream.  This method may be preferred over a serializable::read operation
        * to avoid instantiating multiple templates for istream types.
        *
        * \return true on success, false on failure
        * \param in input istream
        * \param out output object
        */
        template <typename Output>
        bool read_from_istream (std::istream& in, Output&& out)
        {
            return serializable::read(in, std::forward <Output> (out));
        }
    }
}

#include <framework/protocol_buffers/protocol_buffers.inl>
