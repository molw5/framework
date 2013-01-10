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

#include <sstream>

#include <framework/serializable/streams/stream_wrapper.hpp>
#include <framework/serializable/base_types.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * \headerfile optional_field.hpp <framework/serializable/containers/optional_field.hpp>
        * \brief Optional field input stream wrapper.
        */
        template <typename Stream, typename Type>
        class optional_field_input_wrapper : private stream_wrapper <optional_field_input_wrapper <Stream, Type>, Stream>
        {
            private:
                using base = stream_wrapper <optional_field_input_wrapper <Stream, Type>, Stream>;
                friend base;

            public:
                /**
                * \brief Wrapper constructor.
                *
                * \warn
                * The lifetime of stream and flags must exceed this class; behaviour when this
                * condition is not met is undefined.
                */
                optional_field_input_wrapper (Stream& stream, Type const& flags)
                    : base(stream),
                      p_iFlags(flags)
                {
                }
                
                /**
                * \brief Check flag.
                */
                bool checkFlag (Type const& flag) const 
                { 
                    return p_iFlags & flag; 
                }

                /**
                * \brief Read forwarders.
                */
                using base::read;

            private:
                Type const& p_iFlags;
        };

        /**
        * \headerfile optional_field.hpp <framework/serializable/containers/optional_field.hpp>
        * \brief Optional field output stream wrapper.
        */
        template <typename Stream, typename Type>
        class optional_field_output_wrapper : private stream_wrapper <optional_field_output_wrapper <Stream, Type>, Stream>
        {
            private:
                using base = stream_wrapper <optional_field_output_wrapper <Stream, Type>, Stream>;
                friend base;

            public:
                /**
                * \brief Wrapper constructor.
                *
                * \warn
                * The lifetime of stream and flags must exceed this class; behaviour when this
                * condition is not met is undefined.
                */
                optional_field_output_wrapper (Stream& stream, Type& flags)
                    : base(stream),
                      p_iFlags(flags)
                {
                }
            
                /**
                * \brief Set flag.
                */
                void setFlag (Type const& flag) { p_iFlags |= flag; }

                /**
                * \brief Write forwarders.
                */
                using base::write;

            private:
                Type& p_iFlags;
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
            typename Input,
            typename Output,
            typename Type, 
            typename... Specification>
        bool dispatch_read (Input& in, Output& out, optional_field <Type, Specification...>*)
        {
            using value_type = type_extractor <Type>;

            value_type value;
            if (!dispatch_read <Type> (in, value))
                return false;

            optional_field_input_wrapper <Input, value_type> wrapper{in, value};
            return dispatch_read <alias <Specification...>> (wrapper, out);
        }

        /**
        * \headerfile optional_field.hpp <framework/serializable/containers/optional_field.hpp>
        * \brief Write overload.
        * \param in input object
        * \param out output stream
        * \return true on success, false on failure
        */
        template <
            typename Input,
            typename Output,
            typename Type, 
            typename... Specification>
        bool dispatch_write (Input const& in, Output& out, optional_field <Type, Specification...>*)
        {
            using value_type = type_extractor <Type>;

            value_type value{0};
            std::stringstream ss;

            optional_field_output_wrapper <decltype(ss), value_type> wrapper{ss, value};
            if (!dispatch_write <alias <Specification...>> (in, wrapper))
                return false;

            if (!dispatch_write <Type> (value, out))
                return false;

            auto const& buffer = ss.str();
            if (!out.write(&buffer[0], buffer.size()))
                return false;

            return true;
        }
    }
}
