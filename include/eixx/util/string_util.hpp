//----------------------------------------------------------------------------
/// \file  string.hpp
//----------------------------------------------------------------------------
/// \brief A class implementing an string object of Erlang external 
///        term format.
//----------------------------------------------------------------------------
// Copyright (c) 2010 Serge Aleynikov <saleyn@gmail.com>
// Created: 2010-09-20
//----------------------------------------------------------------------------
/*
***** BEGIN LICENSE BLOCK *****

This file is part of the eixx (Erlang C++ Interface) Library.

Copyright (C) 2010 Serge Aleynikov <saleyn@gmail.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

***** END LICENSE BLOCK *****
*/
#ifndef _EIXX_STRING_UTIL_HPP_
#define _EIXX_STRING_UTIL_HPP_

#include <ostream>
#include <sstream>
#include <string>
#include <eixx/marshal/defaults.hpp>

namespace eixx {

/// Print the content of a buffer to \a out stream in the form:
/// \verbatim <<I1, I2, ..., In>> \endverbatim where <tt>Ik</tt> is
/// unsigned integer less than 256.
inline std::ostream& to_binary_string(std::ostream& out, const char* buf, size_t sz) {
    out << "<<";
    const char* begin = buf, *end = buf + sz;
    for(const char* p = begin; p != end; ++p) {
        out << (p == begin ? "" : ",") << (int)*(unsigned char*)p;
    }
    out << ">>";
    return out;
}

/// Convert the content of a buffer to a binary string in the form:
/// \verbatim <<I1, I2, ..., In>> \endverbatim where <tt>Ik</tt> is
/// unsigned integer less than 256.
inline std::string to_binary_string(const char* a, size_t sz) {
    std::stringstream oss;
    to_binary_string(oss, a, sz);
    return oss.str();
}

/// Convert string to integer
///
/// @tparam TillEOL instructs that the integer must be validated till a_end.
///                 If false, "123ABC" is considered a valid 123 number. Otherwise
///                 the function will return NULL.
/// @return input string ptr beyond the the value read if successful, NULL otherwise
//
template <typename T, bool TillEOL = true>
inline const char* fast_atoi(const char* a_str, const char* a_end, T& res) {
    if (a_str >= a_end) return nullptr;

    bool l_neg;

    if (*a_str == '-') { l_neg = true; ++a_str; }
    else               { l_neg = false; }

    T x = 0;

    do {
        const int c = *a_str - '0';
        if (c < 0 || c > 9) {
            if (TillEOL)
               return nullptr;
            break;
        }
        x = (x << 3) + (x << 1) + c;
    } while (++a_str != a_end);

    res = l_neg ? -x : x;
    return a_str;
}

} // namespace eixx

namespace std {

    template <int N>
    std::string to_string(const uint8_t (&s)[N]) { return std::string((const char*)s, N); }

}

#endif // _EIXX_STRING_UTIL_HPP_
