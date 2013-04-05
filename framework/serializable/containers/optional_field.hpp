// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Optional field container.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <framework/serializable/base_types.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * \brief Flags frame.
        *
        * Used to write the current flags field state to the output stream.
        */
        template <typename Specification>
        class output_flags_frame
        {
            template <typename Output>
            friend bool write_dispatch (
                output_flags_frame <Specification>*,
                output_flags_frame <Specification> const& in, Output& out)
            {
                return dispatch_write <Specification> (in.p_iFlags, out);
            }

            private:
                using type = type_extractor <Specification>;

            public:
                /**
                * \brief Set flag.
                */
                void set_flag (type const flag)
                {
                    p_iFlags |= flag;
                }

                /**
                * \brief Write bytes.
                */
                bool write (char const*, std::size_t)
                {
                    return true;
                }

            private:
                type p_iFlags {};
        };

        template <typename Specification, typename Stream>
        class input_flags_frame : std::reference_wrapper <Stream>
        {
            private:
                using type = type_extractor <Specification>;

            public:
                /**
                * \brief Frame constructor.
                * \warn
                * The lifetime of stream and flags must exceed this class; behaviour when this
                * condition is not met is undefined.
                */
                input_flags_frame (Stream& stream, type flags)
                    : std::reference_wrapper <Stream> (stream),
                      p_iFlags(flags)
                {
                }

                /**
                * \brief Check flag.
                */
                bool check_flag (type const flag) const
                {
                    return p_iFlags & flag;
                }

                /**
                * \brief Read bytes.
                */
                template <std::size_t N>
                bool read (char* s)
                {
                    return stream_read <N> (this->get(), s);
                }

                /**
                * \brief Read bytes.
                */
                bool read (char* s, std::size_t n)
                {
                    return stream_read(this->get(), s, n);
                }

            private:
                type const p_iFlags;
        };

        /**
        * \headerfile optional_field.hpp <framework/serializable/containers/optional_field.hpp>
        * \brief Optional field container.
        */
        template <typename Type, typename... Specification>
        struct optional_field : public container_type <
            pack_container <Specification...>,
            pack_container <Specification...>,
            false>
        {
        };

        /**
        * \headerfile optional_field.hpp <framework/serializable/containers/optional_field.hpp>
        * \brief Read overload.
        * \param in input stream
        * \param out output object
        * \return true on success, false on failure
        */
        template <
            typename Type, 
            typename... Specification,
            typename Input,
            typename Output>
        bool read_dispatch (
            optional_field <Type, Specification...>*,
            Input& in, Output& out)
        {
            type_extractor <Type> flags;
            if (!dispatch_read <Type> (in, flags))
                return false;

            input_flags_frame <Type, Input> frame {in, flags};
            return dispatch_read <alias <Specification...>> (frame, out);
        }

        /**
        * \headerfile optional_field.hpp <framework/serializable/containers/optional_field.hpp>
        * \brief Write overload.
        * \param in input object
        * \param out output stream
        * \return true on success, false on failure
        */
        template <
            typename Type, 
            typename... Specification,
            typename Input,
            typename Output>
        bool write_dispatch (
            optional_field <Type, Specification...>*,
            Input const& in, Output& out)
        {
            // Write the object state to the flags stream
            output_flags_frame <Type> frame;
            if (!dispatch_write <alias <Specification...>> (in, frame))
                return false;

            // Write the flags stream to the output stream
            if (!write(frame, out))
                return false;

            // Write the object state to the output stream
            if (!dispatch_write <alias <Specification...>> (in, out))
                return false;

            return true;
        }
    }
}
