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
        class optional_field_input_wrapper
        {
            public:
                /**
                * \brief Wrapper constructor.
                *
                * \warn
                * The lifetime of stream and flags must exceed this class; behaviour when this
                * condition is not met is undefined.
                */
                optional_field_input_wrapper (Stream& stream, Type const& flags)
                    : p_tStream(stream),
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
                * \brief Read forwarder.
                */
                template <typename... Args>
                auto read (Args&&... args) ->
                decltype(std::declval <Stream> ().read(std::forward <Args> (args)...))
                {
                    return p_tStream.read(std::forward <Args> (args)...);
                }

            private:
                Stream& p_tStream;
                Type const& p_iFlags;
        };

        /**
        * \headerfile optional_field.hpp <framework/serializable/containers/optional_field.hpp>
        * \brief Optional field output stream wrapper.
        */
        template <typename Stream, typename Type>
        class optional_field_output_wrapper
        {
            public:
                /**
                * \brief Wrapper constructor.
                *
                * \warn
                * The lifetime of stream and flags must exceed this class; behaviour when this
                * condition is not met is undefined.
                */
                optional_field_output_wrapper (Stream& stream, Type& flags)
                    : p_tStream(stream),
                      p_iFlags(flags)
                {
                }
            
                /**
                * \brief Set flag.
                */
                void setFlag (Type const& flag) { p_iFlags |= flag; }

                /**
                * \brief Write forwarder.
                */
                template <typename... Args>
                auto write (Args&&... args) ->
                decltype(std::declval <Stream> ().write(std::forward <Args> (args)...))
                {
                    return p_tStream.write(std::forward <Args> (args)...);
                }

            private:
                Stream& p_tStream;
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
        * \brief Optional field serialization implementation.
        */
        template <typename Type, typename... Specification>
        struct serializable_specification <optional_field <Type, Specification...>>
        {
            /**
            * \brief Read container.
            * \param in input stream
            * \param out output object
            * \return true on success, false on failure
            */
            template <typename Input, typename Output>
            static bool read (Input& in, Output& out)
            {
                value_type value;
                if (!serializable_specification <Type>::read(in, value))
                    return false;
                
                optional_field_input_wrapper <Input, value_type> wrapper{in, value};
                return serializable_specification <alias <Specification...>>::read(wrapper, out);
            }

            /**
            * \brief Write container.
            * \param in input stream
            * \param out output object
            * \return true on success, false on failure
            */
            template <typename Input, typename Output>
            static bool write (Input const& in, Output& out)
            {
                value_type value{0};
                std::stringstream ss;
                
                optional_field_output_wrapper <decltype(ss), value_type> wrapper{ss, value};
                if (!serializable_specification <alias <Specification...>>::write(in, wrapper))
                    return false;
                if (!serializable_specification <Type>::write(value, out))
                    return false;
                
                auto const& buffer = ss.str();
                if (!out.write(&buffer[0], buffer.size()))
                    return false;

                return true;
            }
            
            private:
                using value_type = typename type_extractor <Type>::type;
        };
    }
}
