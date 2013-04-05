// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

#pragma once

#include <framework/serializable/utility/interface.hpp>

namespace framework
{
    namespace serializable
    {
        namespace detail
        {
            // Calls the implementation's check() function, if a compatible
            // definition exists - otherwise, returns true.
            template <typename T, typename Name, typename Enabler = void>
            struct conditional_check
            {
                FRAMEWORK_ALWAYS_INLINE
                static bool check (T const&)
                {
                    return true;
                }
            };

            template <typename T, typename Name>
            struct conditional_check <T, Name,
                typename std::enable_if <
                    std::is_same < 
                        decltype(interface <Name> (std::declval <T> ()).check()),
                        decltype(interface <Name> (std::declval <T> ()).check())
                    >::value
                >::type>
            {
                FRAMEWORK_ALWAYS_INLINE
                static bool check (T const& x)
                {
                    return interface <Name> (x).check();
                }
            };

            template <>
            struct compare <
                pack_container <>,
                pack_container <>>
            {
                template <typename LhsType, typename RhsType>
                FRAMEWORK_ALWAYS_INLINE
                static bool less (LhsType const&, RhsType const&)
                {
                    return false;
                }

                template <typename LhsType, typename RhsType>
                FRAMEWORK_ALWAYS_INLINE
                static bool greater (LhsType const&, RhsType const&)
                {
                    return false;
                }

                template <typename LhsType, typename RhsType>
                FRAMEWORK_ALWAYS_INLINE
                static bool less_or_equal (LhsType const&, RhsType const&)
                {
                    return true;
                }

                template <typename LhsType, typename RhsType>
                FRAMEWORK_ALWAYS_INLINE
                static bool greater_or_equal (LhsType const&, RhsType const&)
                {
                    return true;
                }

                template <typename LhsType, typename RhsType>
                FRAMEWORK_ALWAYS_INLINE
                static bool equal (LhsType const&, RhsType const&)
                {
                    return true;
                }

                template <typename LhsType, typename RhsType>
                FRAMEWORK_ALWAYS_INLINE
                static bool not_equal (LhsType const&, RhsType const&)
                {
                    return false;
                }
            };

            template <
                typename LhsHead, typename... LhsTail,
                typename RhsHead, typename... RhsTail>
            struct compare <
                pack_container <LhsHead, LhsTail...>,
                pack_container <RhsHead, RhsTail...>>
            {
                template <typename LhsType, typename RhsType>
                FRAMEWORK_ALWAYS_INLINE
                static bool less (LhsType const& lhs, RhsType const& rhs)
                {
                    // The fixed case (where a compatible check() definition does not exist)
                    // could be handled here explicitly, but the compiler will likely
                    // optimize these out anyway.
                    auto const flag_lhs = conditional_check <LhsType, LhsHead>::check(lhs);
                    auto const flag_rhs = conditional_check <RhsType, RhsHead>::check(rhs);
                    if (!flag_lhs && flag_rhs)
                        return true;
                    else if (flag_lhs && !flag_rhs)
                        return false;
                    else if (flag_lhs && flag_rhs)
                    {
                        auto const& value_lhs = interface <LhsHead> (lhs).get();
                        auto const& value_rhs = interface <LhsHead> (rhs).get();
                        if (value_lhs < value_rhs)
                            return true;
                        if (value_rhs < value_lhs)
                            return false;
                    }

                    return compare <
                            pack_container <LhsTail...>,
                            pack_container <RhsTail...>
                        >::less(lhs, rhs);
                }

                template <typename LhsType, typename RhsType>
                FRAMEWORK_ALWAYS_INLINE
                static bool greater (LhsType const& lhs, RhsType const& rhs)
                {
                    return compare <
                            pack_container <RhsHead, RhsTail...>,
                            pack_container <LhsHead, LhsTail...>
                        >::less(rhs, lhs);
                }

                template <typename LhsType, typename RhsType>
                FRAMEWORK_ALWAYS_INLINE
                static bool less_or_equal (LhsType const& lhs, RhsType const& rhs)
                {
                    return !greater(lhs, rhs);
                }

                template <typename LhsType, typename RhsType>
                FRAMEWORK_ALWAYS_INLINE
                static bool greater_or_equal (LhsType const& lhs, RhsType const& rhs)
                {
                    return !less(lhs, rhs);
                }

                template <typename LhsType, typename RhsType>
                FRAMEWORK_ALWAYS_INLINE
                static bool equal (LhsType const& lhs, RhsType const& rhs)
                {
                    // The fixed case (where a compatible check() definition does not exist)
                    // could be handled here explicitly, but the compiler will likely
                    // optimize these out anyway.
                    auto const flag_lhs = conditional_check <LhsType, LhsHead>::check(lhs);
                    auto const flag_rhs = conditional_check <RhsType, RhsHead>::check(rhs);
                    if (!(flag_lhs == flag_rhs))
                        return false;
                    else if (flag_lhs)
                    {
                        auto const& value_lhs = interface <LhsHead> (lhs).get();
                        auto const& value_rhs = interface <LhsHead> (rhs).get();
                        if (!(value_lhs == value_rhs))
                            return false;
                    }

                    return compare <
                            pack_container <LhsTail...>,
                            pack_container <RhsTail...>
                        >::equal(lhs, rhs);
                }

                template <typename LhsType, typename RhsType>
                FRAMEWORK_ALWAYS_INLINE
                static bool not_equal (LhsType const& lhs, RhsType const& rhs)
                {
                    return !equal(lhs, rhs);
                }
            };
        }
    }
}
