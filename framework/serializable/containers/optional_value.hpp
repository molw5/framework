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
#include <framework/common/containers/index_container.hpp>

namespace framework
{
    namespace serializable
    {
        /**
        * \headerfile optional_value.hpp <framework/serializable/containers/optional_value.hpp>
        * \brief Optional value implementation.
        */
        template <typename T>
        struct optional_value_implementation
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
                ~optional_value_implementation () = default;
                
                /**
                * \brief Default constructor.
                * \post check() == false
                */
                optional_value_implementation ()
                    : p_tValue()
                {
                    assert(!static_cast <implementation const*> (this)->check());
                }

                /**
                * \brief Value constructor.
                * \post check() == true
                */
                optional_value_implementation (value_type value)
                    : p_tValue(std::move(value)),
                      p_bIsSet{true}
                {
                    assert(static_cast <implementation const*> (this)->check());
                }

                /**
                * \brief Default constructor alias.
                */
                optional_value_implementation (std::tuple <>)
                    : optional_value_implementation ()
                {
                }

                /**
                * \brief In-place constructor.
                * \post check() == true
                */
                template <typename... Args>
                optional_value_implementation (std::tuple <Args&&...>&& args)
                    : optional_value_implementation (
                        std::forward <std::tuple <Args&&...>> (args), 
                        static_cast <typename make_indices <sizeof... (Args)>::type*> (nullptr))
                {
                    assert(static_cast <implementation const*> (this)->check());
                }

            private:
                template <typename... Args, std::size_t... Indices>
                optional_value_implementation (std::tuple <Args&&...> args, index_container <Indices...>*)
                    : p_tValue{std::forward <Args> (std::get <Indices> (args))...},
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
            template <typename> class Implementation = optional_value_implementation>
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
        * \brief Optional value serialization.
        */
        template <
            int64_t Flag,
            typename Name,
            typename Specification,
            template <typename> class Implementation>
        struct serializable_specification <optional_value <
            Flag,
            Name,
            Specification,
            Implementation>>
        {
            private:
                using value_alias = value_type <Name, Specification, Implementation>;

            public:
                /**
                * \brief Conditionally read value.
                * \param in input stream
                * \param out output object
                * \return true on sucess, false on failure
                */
                template <typename Input, typename Output>
                static bool read (Input& in, Output& out)
                {
                    if (!in.checkFlag(Flag))
                        interface <Name> (out).clear();
                    else if (!serializable_specification <value_alias>::read(in, out))
                        return false;
                    
                    return true;
                }
    
                /**
                * \brief Conditionally write value.
                * \param in input object
                * \param out output stream
                * \return true on sucess, false on failure
                */
                template <typename Input, typename Output>
                static bool write (Input const& in, Output& out)
                {
                    if (!interface <Name> (in).check())
                        return true;
                    if (!serializable_specification <value_alias>::write(in, out))
                        return false;

                    out.setFlag(Flag);
                    return true;
                }
        };
    }
}
