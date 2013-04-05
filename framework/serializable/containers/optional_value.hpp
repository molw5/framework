// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Optional value.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <tuple>
#include <cassert>

#include <framework/serializable/base_types.hpp>
#include <framework/serializable/mutator_type.hpp>
#include <framework/serializable/utility/interface.hpp>
#include <framework/common/pack_container.hpp>
#include <framework/serializable/containers/optional_field.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * \headerfile optional_value.hpp <framework/serializable/containers/optional_value.hpp>
        * \brief Optional value implementation.
        */
        template <typename T>
        struct default_optional_value
        {
            private:
                using implementation = typename T::implementation;
                using value_type = typename T::value_type;

            public:
                /**
                * \brief Get value.
                * \pre check() == true
                */
                value_type const& get () const
                {
                    assert(static_cast <implementation const*> (this)->check());
                    return p_tValue;
                }

                /**
                * \brief Set value.
                * \post check() == true
                */
                void set (value_type value)
                {
                    p_tValue = std::move(value);
                    p_bIsSet = true;
                    assert(static_cast <implementation const*> (this)->check());
                }

                /**
                * \brief Check value.
                */
                bool check () const
                {
                    return p_bIsSet;
                }

                /**
                * \brief Clear value.
                * \post check() == false
                */
                void clear ()
                {
                    p_tValue = value_type{};
                    p_bIsSet = false;
                    assert(!static_cast <implementation const*> (this)->check());
                }

            protected:
                /**
                * \brief Default destructor.
                */
                ~default_optional_value () = default;
                
                /**
                * \brief Default constructor.
                * \post check() == false
                */
                default_optional_value ()
                    : p_tValue()
                {
                    assert(!static_cast <implementation const*> (this)->check());
                }

                /**
                * \brief Value constructor.
                * \post check() == true
                */
                default_optional_value (value_type value)
                    : p_tValue(std::move(value)),
                      p_bIsSet{true}
                {
                    assert(static_cast <implementation const*> (this)->check());
                }

                /**
                * \brief Default constructor alias.
                */
                default_optional_value (std::tuple <>)
                    : default_optional_value ()
                {
                }

                /**
                * \brief In-place constructor.
                * \post check() == true
                */
                template <typename... Args>
                default_optional_value (std::tuple <Args&&...>&& args)
                    : default_optional_value (
                        std::forward <std::tuple <Args&&...>> (args), 
                        static_cast <typename make_indices <sizeof... (Args)>::type*> (nullptr))
                {
                    assert(static_cast <implementation const*> (this)->check());
                }

            private:
                template <typename... Args, typename... Indices>
                default_optional_value (std::tuple <Args&&...> args, pack_container <Indices...>*)
                    : p_tValue{std::forward <Args> (std::get <Indices::value> (args))...},
                      p_bIsSet{true}
                {
                }

            private:
                value_type p_tValue {};
                bool p_bIsSet {false};
        };

        /**
        * \headerfile optional_value.hpp <framework/serializable/containers/optional_value.hpp>
        * \brief Optional value container.
        */
        template <
            int64_t Flag,
            typename Name,
            typename Specification,
            template <typename> class Implementation = default_optional_value>
        struct optional_value : public value_type <
            Name,
            Specification,
            value_implementation_wrapper <
                Name,
                Specification,
                Implementation
            >::template type,
            false>
        {
        };

        /**
        * \headerfile optional_value.hpp <framework/serializable/containers/optional_value.hpp>
        * \brief Read overload.
        *
        * Conditionally reads the underlying value.
        *
        * \param in input stream
        * \param out output object
        * \return true on success, false on failure
        */
        template <
            int64_t Flag,
            typename Name,
            typename Specification,
            template <typename> class Implementation,
            typename Input,
            typename Output>
        bool read_dispatch (
            optional_value <Flag, Name, Specification, Implementation>*,
            Input& in, Output& out)
        {
            if (!in.check_flag(Flag))
            {
                interface <Name> (out).clear();
                return true;
            }

            using value_alias = value_type <Name, Specification, Implementation>;
            return dispatch_read <value_alias> (in, out);
        }

        /**
        * \headerfile optional_value.hpp <framework/serializable/containers/optional_value.hpp>
        * \brief Write overload.
        *
        * Conditionally writes the underlying value.
        *
        * \param in input stream
        * \param out output object
        * \return true on success, false on failure
        */
        template <
            int64_t Flag,
            typename Name,
            typename Specification,
            template <typename> class Implementation,
            typename Input,
            typename Output>
        bool write_dispatch (
            optional_value <Flag, Name, Specification, Implementation>*,
            Input const& in, Output& out)
        {
            if (!interface <Name> (in).check())
                return true;

            using value_alias = value_type <Name, Specification, Implementation>;
            return dispatch_write <value_alias> (in, out);
        }

        /**
        * \headerfile optional_value.hpp <framework/serializable/containers/optional_value.hpp>
        * \brief Write overload.
        *
        * Writes the object state to a flags stream.
        *
        * \param in input stream
        * \param out output object
        * \return true on success, false on failure
        */
        template <
            int64_t Flag,
            typename Name,
            typename Specification,
            template <typename> class Implementation,
            typename Input,
            typename Type>
        bool write_dispatch (
            optional_value <Flag, Name, Specification, Implementation>*,
            Input const& in, output_flags_frame <Type>& out)
        {
            if (!interface <Name> (in).check())
                return true;

            out.set_flag(Flag);

            using value_alias = value_type <Name, Specification, Implementation>;
            return dispatch_write <value_alias> (in, out);
        }
    }
}
