// Copyright (C) 2012 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief CRC wrapper.
*
* \copyright
* Copyright &copy; 2012 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#ifndef FRAMEWORK_NO_BOOST
    #include <boost/crc.hpp>
#endif

namespace framework
{
    namespace serializable
    {
        /**
        * \brief CRC Wrapper.
        *
        * Provides a simple wrapper around a boost-like CRC implementation.
        *
        * \brief T CRC implementation
        */
        template <typename T>
        class crc_wrapper
        {
            public:
                /**
                * \brief Forwarding constructor.
                */
                template <typename... Args>
                crc_wrapper (Args&&... args)
                    : p_tSum(std::forward <Args> (args)...)
                {
                }

                /**
                * \brief Write bytes.
                */
                bool write (char const* s, std::size_t n)
                {
                    p_tSum.process_bytes(s, n);
                    return true;
                }

                /**
                * \brief Get checksum.
                */
                template <typename... Args>
                auto checksum (Args&&... args) const ->
                decltype(std::declval <T> ().checksum(std::forward <Args> (args)...))
                {
                    return p_tSum.checksum(std::forward <Args> (args)...);
                }

                /**
                * \brief Reset.
                */
                template <typename... Args>
                auto reset (Args&&... args) ->
                decltype(std::declval <T> ().reset(std::forward <Args> (args)...))
                {
                    return p_tSum.reset(std::forward <Args> (args)...);
                }

            private:
                T p_tSum;
        };

#ifndef FRAMEWORK_NO_BOOST
        using crc_16_wrapper = crc_wrapper <boost::crc_16_type>;
        using crc_ccitt_wrapper = crc_wrapper <boost::crc_ccitt_type>;
        using crc_xmodem_wrapper = crc_wrapper <boost::crc_xmodem_type>;
        using crc_32_wrapper = crc_wrapper <boost::crc_32_type>;
#endif
    }
}
