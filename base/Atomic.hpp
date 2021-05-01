/******************************************************************************
 * Name         : Atom.hpp
 * Author       : 671643387
 * Date         : 2015年9月11日
 * Description  :
 *****************************************************************************/

#ifndef BASE_ATOM_HPP_
#define BASE_ATOM_HPP_

#include <stdint.h>
#if defined(__WINDOWS__)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(__LINUX__)
#endif

namespace dev
{
    namespace base
    {  
#if defined(__WINDOWS__)

        inline uint16_t atom_inc16(uint16_t* x)
        {
            return uint16_t(::InterlockedIncrement16((short*)x));
        }

        inline uint32_t atom_inc32(uint32_t* x)
        {
            return uint32_t(::InterlockedIncrement((long*)x));
        }

        inline uint64_t atom_inc64(uint64_t* x)
        {
            return uint64_t(::InterlockedIncrement64((long long*)x));
        }

        inline uint16_t atom_dec16(uint16_t* x)
        {
            return uint16_t(::InterlockedDecrement16((short*)x));
        }

        inline uint32_t atom_dec32(uint32_t* x)
        {
            return uint32_t(::InterlockedDecrement((long*)x));
        }

        inline uint64_t atom_dec64(uint64_t* x)
        {
            return uint64_t(::InterlockedDecrement64((long long*)x));
        }

        inline uint16_t atom_add16(uint16_t* x, uint16_t v)
        {
            return uint16_t(::InterlockedAdd((long*)x, v));
        }

        inline uint32_t atom_add32(uint32_t* x, uint32_t v)
        {
            return uint32_t(::InterlockedAdd((long*)x, v));
        }

        inline uint64_t atom_add64(uint64_t* x, uint64_t v)
        {
            return uint64_t(::InterlockedAdd64((long long*)x, v));
        }
#elif defined(__LINUX__)
        inline uint16_t atom_inc16(uint16_t* x)
        {
            return uint16_t(::__sync_add_and_fetch(x, 1));
        }

        inline uint32_t atom_inc32(uint32_t* x)
        {
            return uint32_t(::__sync_add_and_fetch(x, 1));
        }

        inline uint64_t atom_inc64(uint64_t* x)
        {
            return uint64_t(::__sync_add_and_fetch(x, 1));
        }

        inline uint16_t atom_dec16(uint16_t* x)
        {
            return uint16_t(::__sync_sub_and_fetch(x, 1));
        }

        inline uint32_t atom_dec32(uint32_t* x)
        {
            return uint32_t(::__sync_sub_and_fetch(x, 1));
        }

        inline uint64_t atom_dec64(uint64_t* x)
        {
            return uint64_t(::__sync_sub_and_fetch(x, 1));
        }

        inline uint16_t atom_add16(uint16_t* x, uint16_t v)
        {
            return uint16_t(::__sync_add_and_fetch(x, v));
        }

        inline uint32_t atom_add32(uint32_t* x, uint32_t v)
        {
            return uint32_t(::__sync_add_and_fetch(x, v));
        }

        inline uint64_t atom_add64(uint64_t* x, uint64_t v)
        {
            return uint64_t(::__sync_add_and_fetch(x, v));
        }
#endif
    }
}

#endif /* BASE_ATOM_HPP_ */
