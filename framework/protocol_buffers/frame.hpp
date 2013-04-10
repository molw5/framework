// Copyright (C) 2013 iwg molw5
// For conditions of distribution and use, see copyright notice in COPYING

/**
* \file
* \brief Frame wrappers.
*
* \copyright
* Copyright &copy; 2013 iwg molw5<br>
* For conditions of distribution and use, see copyright notice in COPYING
*/

#pragma once

#include <string.h>
#include <functional>

#include <framework/serializable/base_types.hpp>

namespace framework
{
    namespace protocol_buffers
    {
        /**
        * \brief Input frame wrapper.
        *
        * Provides a trivial input stream wrapper around a raw character buffer.
        */
        struct raw_input_frame
        {
            public:
                /**
                * \brief Iterator position type.
                */
                using pos_type = std::size_t;
                
                /**
                * \brief Iterator offset type.
                */
                using off_type = typename std::make_signed <pos_type>::type;

            public:
                /**
                * \brief Frame constructor.
                * \warning
                * The lifetime of the underlying character buffer must exceed that of raw_input_frame.  Behaviour when
                * this condition is not met is undefined.
                */
                raw_input_frame (char const* begin, char const* end)
                    : p_pIt(begin),
                      p_pBegin(begin),
                      p_pEnd(end)
                {
                }
    
                /**
                * \brief Read bytes.
                */
                FRAMEWORK_ALWAYS_INLINE
                bool read (char* s, std::size_t n)
                {
                    if (FRAMEWORK_EXPECT_FALSE(p_pEnd - p_pIt < static_cast <long> (n)))
                        return false;
    
                    memcpy(s, p_pIt, n);
                    p_pIt += n;
                    return true;
                }
    
                /**
                * \brief Read bytes.
                */
                template <std::size_t N>
                FRAMEWORK_ALWAYS_INLINE
                bool read (char* s)
                {
                    if (FRAMEWORK_EXPECT_FALSE(p_pEnd - p_pIt < static_cast <long> (N)))
                        return false;
    
                    /*static*/ if (N > 1)
                    {
                        memcpy(s, p_pIt, N);
                        p_pIt += N;
                    }
                    else if (N == 1) 
                    {
                        *s = (char)*(p_pIt++);
                    }

                    return true;
                }

                /**
                * \brief Input iterator position.
                */
                FRAMEWORK_ALWAYS_INLINE
                pos_type tellg () const
                {
                    return p_pIt - p_pBegin;
                }

                /**
                * \brief Seek to position.
                *
                * Sets the input iterator to the provided position.  Note that the definition used here may not be
                * compatible with that of basic_istream::seekg.
                *
                * \param pos new input iterator position
                * \return true on success, false on failure
                */
                FRAMEWORK_ALWAYS_INLINE
                bool seekg (pos_type pos)
                {
                    if (FRAMEWORK_EXPECT_FALSE(p_pEnd - p_pBegin <= static_cast <long> (pos)))
                        return false;

                    p_pIt = p_pBegin + pos;
                    return true;
                }

                /**
                * \brief Seek to offset.
                *
                * Sets the input iterator to an offset relative to the provided position (dir).  Note that the definition 
                * used here may not be compatible with that of basic_istream::seekg.
                *
                * \param off input iterator offset
                * \param dir reference position
                * \return true on success, false on failure
                * 
                */
                FRAMEWORK_ALWAYS_INLINE
                bool seekg (off_type off, std::ios_base::seekdir dir)
                {
                    switch (dir)
                    {
                        case std::ios::beg:
                            if (FRAMEWORK_EXPECT_FALSE(off < 0 || p_pEnd - p_pBegin < off))
                                return false;

                            p_pIt = p_pBegin + off;
                            return true;

                        case std::ios::end:
                            if (FRAMEWORK_EXPECT_FALSE(off > 0 || p_pEnd - p_pBegin < off))
                                return false;

                            p_pIt = p_pEnd - off;
                            return true;

                        case std::ios::cur:
                            if (off >= 0)
                            {
                                if (FRAMEWORK_EXPECT_FALSE(p_pEnd - p_pIt < off))
                                    return false;

                                p_pIt += off;
                                return true;
                            }
                            else
                            {
                                if (FRAMEWORK_EXPECT_FALSE(p_pIt - p_pBegin < -off))
                                    return false;

                                p_pIt += off;
                                return true;
                            }

                        default:
                            assert(false);
                            return false;
                    }
                }

            private:
                char const* p_pIt;
                char const* const p_pBegin;
                char const* const p_pEnd;
        };
        
        /**
        * \brief Output frame wrapper.
        *
        * Provides a trivial output stream wrapper around a raw character buffer.
        *
        * \tparam Safe check buffer boundaries when writing to the stream
        */
        template <bool Safe = true>
        struct raw_output_frame
        {
            public:
                /**
                * \brief Iterator position type.
                */
                using pos_type = std::size_t;
                
                /**
                * \brief Iterator offset type.
                */
                using off_type = typename std::make_signed <pos_type>::type;

            public:
                /**
                * \brief Frame constructor.
                * \warning
                * The lifetime of the underlying character buffer must exceed that of raw_output_frame.  Behaviour when
                * this condition is not met is undefined.
                *
                * \pre
                * begin <= end
                */
                raw_output_frame (char* begin, char* end)
                    : p_pIt(begin),
                      p_pBegin(begin),
                      p_pEnd(end)
                {
                    assert(p_pBegin <= p_pEnd);
                }

                /**
                * \brief Write bytes.
                */
                bool write (char const* s, std::streamsize n)
                {
                    /*static*/ if (Safe)
                    {
                        if (FRAMEWORK_EXPECT_FALSE(p_pEnd - p_pIt < n))
                            return false;
                    }
                    else
                    {
                        assert(p_pEnd - p_pIt >= n);
                    }

                    memcpy(p_pIt, s, n);
                    p_pIt += n;
                    return true;
                }

                /**
                * \brief Write bytes.
                */
                template <std::size_t N>
                bool write (char const* s)
                {
                    /*static*/ if (Safe)
                    {
                        if (FRAMEWORK_EXPECT_FALSE(static_cast <std::size_t> (p_pEnd - p_pIt) < N))
                            return false;
                    }
                    else
                    {
                        assert(static_cast <std::size_t> (p_pEnd - p_pIt) >= N);
                    }

                    /*static*/ if (N > 1)
                    {
                        memcpy(p_pIt, s, N);
                        p_pIt += N;
                    }
                    else if (N == 1)
                    {
                        *(p_pIt++) = static_cast <char> (*s);
                    }
                    
                    return true;
                }

                /**
                * \brief Seek to position.
                *
                * Sets the output iterator to the provided position.  Note that the definition used here may not be
                * compatible with that of basic_ostream::seekg.
                *
                * \param pos new output iterator position
                * \return true on success, false on failure
                */
                bool seekp (pos_type pos)
                {
                    /*static*/ if (Safe)
                    {
                        if (FRAMEWORK_EXPECT_FALSE(static_cast <std::size_t> (p_pEnd - p_pBegin) < pos))
                            return false;
                    }
                    else
                    {
                        assert(static_cast <std::size_t> (p_pEnd - p_pBegin) >= pos);
                    }

                    p_pIt = p_pBegin + pos;
                    return true;
                }

                /**
                * \brief Output iterator position.
                */
                std::size_t tellp () const
                {
                    assert((p_pBegin <= p_pIt) && (p_pIt <= p_pEnd));
                    return p_pIt - p_pBegin;
                }

            private:
                char* p_pIt;
                char* const p_pBegin;
                char* const p_pEnd;
        };

        /**
        * \brief Length delimited input frame wrapper.
        *
        * Provides a length-delimited wrapper around an arbitrary stream type.
        */
        template <typename Stream>
        class length_delimited_input_frame : std::reference_wrapper <Stream>
        {
            public:
                /**
                * \brief Frame constructor.
                * \warning
                * The lifetime stream must exceed that of length_delimited_input_frame.  Behaviour when this condition is
                * not met is undefined.
                */
                length_delimited_input_frame (Stream& stream, std::size_t size)
                    : std::reference_wrapper <Stream> (stream),
                      p_iSize(size)
                {
                }

                /**
                * \brief Read bytes.
                */
                FRAMEWORK_ALWAYS_INLINE
                bool read (char* s, std::size_t n)
                {
                    if (FRAMEWORK_EXPECT_FALSE(p_iSize < n))
                        return false;
                    if (FRAMEWORK_EXPECT_FALSE(!serializable::stream_read(this->get(), s, n)))
                        return false;

                    p_iSize -= n;
                    return true;
                }

                /**
                * \brief Read bytes.
                */
                template <std::size_t N>
                FRAMEWORK_ALWAYS_INLINE
                bool read (char* s)
                {
                    if (FRAMEWORK_EXPECT_FALSE(p_iSize < N))
                        return false;
                    if (FRAMEWORK_EXPECT_FALSE(!serializable::stream_read <N> (this->get(), s)))
                        return false;

                    p_iSize -= N;
                    return true;
                }

                /**
                * \brief Frame size.
                * \returns number of bytes remaining
                */
                FRAMEWORK_ALWAYS_INLINE
                std::size_t size () const
                {
                    return p_iSize;
                }
            
            private:
                std::size_t p_iSize;
        };

        /**
        * \brief Size frame.
        *
        * Provides a trivial stream implementation suitable for extracting the serialized size of an object.
        */
        class size_frame
        {
            public:
                /**
                * \brief Iterator position type.
                */
                using pos_type = std::size_t;

            public:
                /**
                * \brief Write bytes.
                */
                FRAMEWORK_ALWAYS_INLINE
                bool write (char const*, std::size_t n)
                {
                    p_iSize += n;
                    return true;
                }

                /**
                * \brief Write bytes.
                */
                template <std::size_t N>
                FRAMEWORK_ALWAYS_INLINE
                bool write (char const*)
                {
                    p_iSize += N;
                    return true;
                }

                /**
                * \brief Seek to position.
                *
                * Sets the output iterator to the provided position.  Note that the definition used here may not be
                * compatible with that of basic_ostream::seekg.
                *
                * \param pos new output iterator position
                * \return true on success, false on failure
                */
                FRAMEWORK_ALWAYS_INLINE
                bool seekp (pos_type pos)
                {
                    p_iSize = pos;
                    return true;
                }

                /**
                * \brief Output iterator position.
                */
                FRAMEWORK_ALWAYS_INLINE
                std::size_t tellp () const
                {
                    return p_iSize;
                }

                /**
                * \brief Skip bytes.
                *
                * Advances the iterator position by n bytes.
                */
                FRAMEWORK_ALWAYS_INLINE
                void skip (std::size_t n)
                {
                    p_iSize += n;
                }

                /**
                * \brief Size conversion operator.
                * \return serialized object size
                */
                FRAMEWORK_ALWAYS_INLINE
                explicit operator std::size_t() const
                {
                    return p_iSize;
                }
            
            private:
                std::size_t p_iSize {};
        };

        /**
        * \brief Size frame.
        *
        * Provides a trivial stream implementation suitable for extracting the maximum serialized size of an object.  This
        * operation is typically much faster than computing the precise size of the object.
        *
        * \note
        * Public inheritance is not desirable here - conversion to size_frame discards information.
        */
        class max_size_frame : private size_frame
        {
            public:
                using size_frame::pos_type;
                using size_frame::write;
                using size_frame::seekp;
                using size_frame::tellp;
                using size_frame::skip;
                using size_frame::operator std::size_t;
        };
    }
}
