// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Serializable container comparison.
*
* Provides a series of member-wise comparison free functions for member-wise serializable object
* comparisons.  The templates provided here require only that the size of the container's value 
* types match; no additional constraints are placed on the types or names associated with the values.
*
* Value pair requirements:
*   - less: operator <
*   - greater: operator <
*   - less_or_equal: operator <
*   - greater_or_equal: operator <
*   - equal: operator ==
*   - not_equal: operator ==
*
* Support is provided for a \c check member function in a value type's interface returning
* a boolean-comparable type, intended to allow these operators to continue to function properly
* in the presence of unset value types (ie: \c optional_value).  When these members are present
* an unset value type is considered less than any set value type and equal to any unset value
* type.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <framework/serializable/utility/interface.hpp>

namespace framework
{
    namespace serializable
    {
        namespace detail
        {
            template <typename LhsNames, typename RhsNames>
            struct compare;
        }

        /**
        * \brief Less than.
        */
        template <typename LhsType, typename RhsType>
        bool less (LhsType const& lhs, RhsType const& rhs)
        {
            return detail::compare <
                    object_names <LhsType>,
                    object_names <RhsType>
                >::less(lhs, rhs);
        }

        /**
        * \brief Greater than.
        */
        template <typename LhsType, typename RhsType>
        bool greater (LhsType const& lhs, RhsType const& rhs)
        {
            return detail::compare <
                    object_names <LhsType>,
                    object_names <RhsType>
                >::greater(lhs, rhs);
        }

        /**
        * \brief Less than or equal to.
        */
        template <typename LhsType, typename RhsType>
        bool less_or_equal (LhsType const& lhs, RhsType const& rhs)
        {
            return detail::compare <
                    object_names <LhsType>,
                    object_names <RhsType>
                >::less_or_equal(lhs, rhs);
        }

        /**
        * \brief Greater than or equal to.
        */
        template <typename LhsType, typename RhsType>
        bool greater_or_equal (LhsType const& lhs, RhsType const& rhs)
        {
            return detail::compare <
                    object_names <LhsType>,
                    object_names <RhsType>
                >::greater_or_equal(lhs, rhs);
        }

        /**
        * \brief Equal to
        */
        template <typename LhsType, typename RhsType>
        bool equal (LhsType const& lhs, RhsType const& rhs)
        {
            return detail::compare <
                    object_names <LhsType>,
                    object_names <RhsType>
                >::equal(lhs, rhs);
        }
        
        /**
        * \brief Not equal
        */
        template <typename LhsType, typename RhsType>
        bool not_equal (LhsType const& lhs, RhsType const& rhs)
        {
            return detail::compare <
                    object_names <LhsType>,
                    object_names <RhsType>
                >::not_equal(lhs, rhs);
        }

        /**
        * \headerfile comparable.hpp <framework/serializable/mutators/comparable.hpp>
        * \brief Comparable template.
        *
        * Provide \c Parent with conventional operator methods to interface with the above
        * free functions using the Barton-Nackman trick.
        *
        * \tparam Parent parent class
        */
        template <typename Parent>
        struct comparable
        {
            /**
            * \brief Less than operator.
            */
            FRAMEWORK_ALWAYS_INLINE
            friend bool operator< (Parent const& lhs, Parent const& rhs)
            {
                return less(lhs, rhs);
            }

            /**
            * \brief Greater than operator.
            */
            FRAMEWORK_ALWAYS_INLINE
            friend bool operator> (Parent const& lhs, Parent const& rhs)
            {
                return greater(lhs, rhs);
            }

            /**
            * \brief Less than or equal to operator.
            */
            FRAMEWORK_ALWAYS_INLINE
            friend bool operator<= (Parent const& lhs, Parent const& rhs)
            {
                return less_or_equal(lhs, rhs);
            }

            /**
            * \brief Greater than or equal to operator.
            */
            FRAMEWORK_ALWAYS_INLINE
            friend bool operator>= (Parent const& lhs, Parent const& rhs)
            {
                return greater_or_equal(lhs, rhs);
            }

            /**
            * \brief Equal to operator.
            */
            FRAMEWORK_ALWAYS_INLINE
            friend bool operator== (Parent const& lhs, Parent const& rhs)
            {
                return equal(lhs, rhs);
            }

            /**
            * \brief Not equal to operator.
            */
            FRAMEWORK_ALWAYS_INLINE
            friend bool operator!= (Parent const& lhs, Parent const& rhs)
            {
                return not_equal(lhs, rhs);
            }
        };
    }
}

#include <framework/serializable/utility/comparable.inl>
