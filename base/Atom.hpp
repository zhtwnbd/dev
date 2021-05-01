/******************************************************************************
 * Name         : Atom.hpp
 * Author       : 671643387
 * Date         : 2015年9月11日
 * Description  :
 *****************************************************************************/

#ifndef BASE_ATOM_HPP_
#define BASE_ATOM_HPP_

#if defined(__WINDOWS__)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(__LINUX__)
#error
#endif

namespace dev
{
    namespace base
    {  
#if defined(__WINDOWS__)
        inline unsigned int atom_inc(unsigned int* x)
        {
            return ::InterlockedIncrement(x);
        }

        inline unsigned long atom_inc(unsigned long* x)
        {
            return ::InterlockedIncrement(x);
        }

        inline long atom_inc(long* x)
        {
            return ::InterlockedIncrement(x);
        }

        inline unsigned int atom_dec(unsigned int* x)
        {
            return ::InterlockedDecrement(x);
        }

        inline unsigned long atom_dec(unsigned long* x)
        {
            return ::InterlockedDecrement(x);
        }

        inline long atom_dec(long* x)
        {
            return ::InterlockedDecrement(x);
        }
#endif
    }
}

#endif /* BASE_ATOM_HPP_ */
