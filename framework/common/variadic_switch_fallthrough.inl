// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

namespace framework
{
    namespace detail
    {
#define FRAMEWORK_VARIADIC_CASE_LIST(z, n, text) \
        BOOST_PP_COMMA_IF(n) text c_##n

#define FRAMEWORK_VARIADIC_CASE(z, n, text) \
        case c_##n: text.template operator() <c_##n> (args...);

#define FRAMEWORK_VARIADIC_SWITCH(n) \
        template < \
            typename CaseType \
            BOOST_PP_COMMA_IF(n) BOOST_PP_REPEAT(n, FRAMEWORK_VARIADIC_CASE_LIST, CaseType)> \
        struct variadic_switch_fallthrough_impl < \
            value_container < \
                CaseType \
                BOOST_PP_COMMA_IF(n) BOOST_PP_REPEAT(n, FRAMEWORK_VARIADIC_CASE_LIST, )>> \
        { \
            template < \
                typename Handler, \
                typename... Args> \
            static void run ( \
                CaseType const& index, \
                Handler&& handler, \
                Args&&... args) \
            { \
                switch (index) \
                { \
                    BOOST_PP_REPEAT(n, FRAMEWORK_VARIADIC_CASE, handler) \
                        break; \
\
                    default: \
                        handler(std::forward <Args> (args)...); \
                        break; \
                } \
            } \
        };

#define BOOST_PP_LOCAL_LIMITS (0, FRAMEWORK_VARIADIC_SWITCH_LIMIT-1)
#define BOOST_PP_LOCAL_MACRO(n) FRAMEWORK_VARIADIC_SWITCH(n)

#ifdef __clang__
 #pragma clang diagnostic push
 #pragma clang diagnostic ignored "-Wtrigraphs"
#endif

??=include BOOST_PP_LOCAL_ITERATE()

#ifdef __clang__
 #pragma clang diagnostic pop
#endif

#undef BOOST_PP_LOCAL_LIMITS
#undef BOOST_PP_LOCAL_MACRO

        template <typename CaseList>
        struct call_remaining;

        template <
            typename CaseType,
            CaseType Head,
            CaseType... Tail>
        struct call_remaining <
            value_container <
                CaseType,
                Head,
                Tail...>>
        {
            template <
                typename Handler,
                typename... Args>
            static void run (Handler&& handler, Args&&... args)
            {
                handler.template operator() <Head> (args...);
                return call_remaining <value_container <CaseType, Tail...>>::run(
                    std::forward <Handler> (handler),
                    std::forward <Args> (args)...);
            }
        };

        template <typename CaseType>
        struct call_remaining <value_container <CaseType>>
        {
            template <
                typename Handler,
                typename... Args>
            static void run (Handler&&, Args&&...)
            {
            }
        };

        template <
            typename CaseType,
            BOOST_PP_REPEAT(FRAMEWORK_VARIADIC_SWITCH_LIMIT, FRAMEWORK_VARIADIC_CASE_LIST, CaseType),
            CaseType... Tail>
        struct variadic_switch_fallthrough_impl <
            value_container <
                CaseType,
                BOOST_PP_REPEAT(FRAMEWORK_VARIADIC_SWITCH_LIMIT, FRAMEWORK_VARIADIC_CASE_LIST, ),
                Tail...>>
        {
            template <
                typename Handler,
                typename... Args>
            static void run (
                CaseType const& index,
                Handler&& handler,
                Args&&... args)
            {
                switch (index)
                {
                    // Case list
                    BOOST_PP_REPEAT(FRAMEWORK_VARIADIC_SWITCH_LIMIT, FRAMEWORK_VARIADIC_CASE, handler)

                    // If we found index call the remaining cases
                        return call_remaining <value_container <CaseType, Tail...>>::run(
                            std::forward <Handler> (handler),
                            std::forward <Args> (args)...);

                    // Otherwise, repeat using the remaining cases
                    default: 
                        return variadic_switch_fallthrough_impl <value_container <CaseType, Tail...>>::run(
                            index,
                            std::forward <Handler> (handler),
                            std::forward <Args> (args)...);
                }
            }
        };

#undef FRAMEWORK_VARIADIC_CASE_LIST
#undef FRAMEWORK_VARIADIC_CASE
#undef FRAMEWORK_VARIADIC_SWITCH
    }
}
