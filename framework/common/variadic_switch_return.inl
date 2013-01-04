// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

namespace framework
{
    namespace detail
    {
#define FRAMEWORK_VARIADIC_CASE_LIST(z, n, text) \
        BOOST_PP_COMMA_IF(n) text c_##n

#define FRAMEWORK_VARIADIC_CASE(z, n, text) \
        case c_##n: return text.template operator() <c_##n> (std::forward <Args> (args)...);

#define FRAMEWORK_VARIADIC_SWITCH(n) \
        template < \
            typename CaseType \
            BOOST_PP_COMMA_IF(n) BOOST_PP_REPEAT(n, FRAMEWORK_VARIADIC_CASE_LIST, CaseType)> \
        struct variadic_switch_return_impl < \
            value_container < \
                CaseType \
                BOOST_PP_COMMA_IF(n) BOOST_PP_REPEAT(n, FRAMEWORK_VARIADIC_CASE_LIST, )>> \
        { \
            template < \
                typename Handler, \
                typename... Args> \
            static auto run ( \
                CaseType const& index, \
                Handler&& handler, \
                Args&&... args) -> \
            decltype(handler(std::forward <Args> (args)...)) \
            { \
                switch (index) \
                { \
                    BOOST_PP_REPEAT(n, FRAMEWORK_VARIADIC_CASE, handler) \
                    default: return handler(std::forward <Args> (args)...); \
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

        template <
            typename CaseType,
            BOOST_PP_REPEAT(FRAMEWORK_VARIADIC_SWITCH_LIMIT, FRAMEWORK_VARIADIC_CASE_LIST, CaseType),
            CaseType... Tail>
        struct variadic_switch_return_impl <
            value_container <
                CaseType,
                BOOST_PP_REPEAT(FRAMEWORK_VARIADIC_SWITCH_LIMIT, FRAMEWORK_VARIADIC_CASE_LIST, ),
                Tail...>>
        {
            template <
                typename Handler,
                typename... Args>
            static auto run (
                CaseType const& index,
                Handler&& handler,
                Args&&... args) ->
            decltype(handler(std::forward <Args> (args)...))
            {
                switch (index)
                {
                    BOOST_PP_REPEAT(FRAMEWORK_VARIADIC_SWITCH_LIMIT, FRAMEWORK_VARIADIC_CASE, handler)
                    
                    default: 
                        return variadic_switch_return_impl <value_container <CaseType, Tail...>>::run(
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
};
