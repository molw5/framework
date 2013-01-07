// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Stream wrapper.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <functional>

namespace framework
{
    namespace serializable
    {
        /**
        * \headerfile stream_wrapper.hpp <framework/serializable/streams/stream_wrapper.hpp>
        * \brief Stream wrapper class.
        *
        * Provides the overrides necessary to forward read/write calls to an underlying stream.
        */
        template <typename Parent, typename Stream>
        class stream_wrapper : private std::reference_wrapper <Stream>
        {
            private:
                using base = std::reference_wrapper <Stream>;
                friend Parent;

            public:
                /**
                * \brief Wrapper constructor.
                *
                * \warn
                * The lifetime of stream must exceed this class; behaviour when this condition is not met is undefined.
                */
                stream_wrapper (Stream& stream)
                    : base(stream)
                {
                }

                /**
                * \brief Write forwarder.
                */
                template <typename T>
                auto write (T const& t) ->
                typename std::enable_if <write_type_exists <T, Stream>::value, bool>::type
                {
                    auto const parent_this = static_cast <Parent*> (this);
                    if (!parent_this->writepre(t))
                        return false;

                    if (!base::get().write(t))
                        return false;

                    if (!parent_this->writepost(t))
                        return false;

                    return true;
                }

                /**
                * \brief Write forwarder.
                */
                template <std::size_t N>
                auto write (char const* s) ->
                typename std::enable_if <write_count_exists <N, Stream>::value, bool>::type
                {
                    auto const parent_this = static_cast <Parent*> (this);
                    if (!parent_this->wrie_prefix(s))
                        return false;

                    if (!base::get().write <N> (s))
                        return false;

                    if (!parent_this->writepost(s))
                        return false;

                    return true;
                }

                /**
                * \brief Write forwarder.
                */
                template <typename... Args>
                auto write (Args&&... args) ->
                decltype(std::declval <Stream> ().write(args...), bool())
                {
                    auto const parent_this = static_cast <Parent*> (this);
                    if (!parent_this->writepre(args...))
                        return false;

                    if (!base::get().write(args...))
                        return false;

                    if (!parent_this->writepost(args...))
                        return false;

                    return true;
                }

                /**
                * \brief Read forwarder.
                */
                template <typename T>
                auto read (T const& t) ->
                typename std::enable_if <read_type_exists <Stream, T>::value, bool>::type
                {
                    auto const parent_this = static_cast <Parent*> (this);
                    if (!parent_this->readpre(t))
                        return false;

                    if (!base::get().read(t))
                        return false;

                    if (!parent_this->readpost(t))
                        return false;

                    return true;
                }

                /**
                * \brief Read forwarder.
                */
                template <std::size_t N>
                auto read (char const* s) ->
                typename std::enable_if <read_count_exists <Stream, N>::value, bool>::type
                {
                    auto const parent_this = static_cast <Parent*> (this);
                    if (!parent_this->wrie_prefix(s))
                        return false;

                    if (!base::get().read <N> (s))
                        return false;

                    if (!parent_this->readpost(s))
                        return false;

                    return true;
                }

                /**
                * \brief Read forwarder.
                */
                template <typename... Args>
                auto read (Args&&... args) ->
                decltype(std::declval <Stream> ().read(args...), bool())
                {
                    auto const parent_this = static_cast <Parent*> (this);
                    if (!parent_this->readpre(args...))
                        return false;

                    if (!base::get().read(args...))
                        return false;

                    if (!parent_this->readpost(args...))
                        return false;

                    return true;
                }

            private:
                /**
                * \brief Default write-pre intercept.
                */
                template <typename... Args>
                bool writepre(Args&...) { return true; }

                /**
                * \brief Default write-post intercept.
                */
                template <typename... Args>
                bool writepost(Args&...) { return true; }

                /**
                * \brief Default read-pre intercept.
                */
                template <typename... Args>
                bool readpre(Args&...) { return true; }

                /**
                * \brief Default read-post intercept.
                */
                template <typename... Args>
                bool readpost(Args&...) { return true; }
        };
    }
}
