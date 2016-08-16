/* 
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 * 
 * The Original Code is the Netscape Portable Runtime (NSPR).
 * 
 * The Initial Developer of the Original Code is Netscape
 * Communications Corporation.  Portions created by Netscape are 
 * Copyright (C) 1998-2000 Netscape Communications Corporation.  All
 * Rights Reserved.
 * 
 * Contributor(s):
 * 
 * Alternatively, the contents of this file may be used under the
 * terms of the GNU General Public License Version 2 or later (the
 * "GPL"), in which case the provisions of the GPL are applicable 
 * instead of those above.  If you wish to allow use of your 
 * version of this file only under the terms of the GPL and not to
 * allow others to use your version of this file under the MPL,
 * indicate your decision by deleting the provisions above and
 * replace them with the notice and other provisions required by
 * the GPL.  If you do not delete the provisions above, a recipient
 * may use your version of this file under either the MPL or the
 * GPL.
 */

#ifndef prlong_h___
#define prlong_h___

#include "prtypes.h"

PR_BEGIN_EXTERN_C

#if defined(HAVE_WATCOM_BUG_2)
PRInt64 __pascal __loadds __export
    LL_MaxInt(void);
PRInt64 __pascal __loadds __export
    LL_MinInt(void);
PRInt64 __pascal __loadds __export
    LL_Zero(void);
PRUint64 __pascal __loadds __export
    LL_MaxUint(void);
#else
NSPR_API(PRInt64) LL_MaxInt(void);
NSPR_API(PRInt64) LL_MinInt(void);
NSPR_API(PRInt64) LL_Zero(void);
NSPR_API(PRUint64) LL_MaxUint(void);
#endif

#define LL_MAXINT   LL_MaxInt()
#define LL_MININT   LL_MinInt()
#define LL_ZERO     LL_Zero()
#define LL_MAXUINT  LL_MaxUint()

#if defined(HAVE_LONG_LONG)

#if PR_BYTES_PER_LONG == 8
#define LL_INIT(hi, lo)  ((hi ## L << 32) + lo ## L)
#elif (defined(WIN32) || defined(WIN16)) && !defined(__GNUC__)
#define LL_INIT(hi, lo)  ((hi ## i64 << 32) + lo ## i64)
#else
#define LL_INIT(hi, lo)  ((hi ## LL << 32) + lo ## LL)
#endif


#define LL_IS_ZERO(a)       ((a) == 0)
#define LL_EQ(a, b)         ((a) == (b))
#define LL_NE(a, b)         ((a) != (b))
#define LL_GE_ZERO(a)       ((a) >= 0)
#define LL_CMP(a, op, b)    ((PRInt64)(a) op (PRInt64)(b))
#define LL_UCMP(a, op, b)   ((PRUint64)(a) op (PRUint64)(b))

#define LL_AND(r, a, b)        ((r) = (a) & (b))
#define LL_OR(r, a, b)        ((r) = (a) | (b))
#define LL_XOR(r, a, b)        ((r) = (a) ^ (b))
#define LL_OR2(r, a)        ((r) = (r) | (a))
#define LL_NOT(r, a)        ((r) = ~(a))

#define LL_NEG(r, a)        ((r) = -(a))
#define LL_ADD(r, a, b)     ((r) = (a) + (b))
#define LL_SUB(r, a, b)     ((r) = (a) - (b))

#define LL_MUL(r, a, b)        ((r) = (a) * (b))
#define LL_DIV(r, a, b)        ((r) = (a) / (b))
#define LL_MOD(r, a, b)        ((r) = (a) % (b))

#define LL_SHL(r, a, b)     ((r) = (PRInt64)(a) << (b))
#define LL_SHR(r, a, b)     ((r) = (PRInt64)(a) >> (b))
#define LL_USHR(r, a, b)    ((r) = (PRUint64)(a) >> (b))
#define LL_ISHL(r, a, b)    ((r) = (PRInt64)(a) << (b))

#define LL_L2I(i, l)        ((i) = (PRInt32)(l))
#define LL_L2UI(ui, l)        ((ui) = (PRUint32)(l))
#define LL_L2F(f, l)        ((f) = (PRFloat64)(l))
#define LL_L2D(d, l)        ((d) = (PRFloat64)(l))

#define LL_I2L(l, i)        ((l) = (PRInt64)(i))
#define LL_UI2L(l, ui)        ((l) = (PRInt64)(ui))
#define LL_F2L(l, f)        ((l) = (PRInt64)(f))
#define LL_D2L(l, d)        ((l) = (PRInt64)(d))

#define LL_UDIVMOD(qp, rp, a, b) \
    (*(qp) = ((PRUint64)(a) / (b)), \
     *(rp) = ((PRUint64)(a) % (b)))

#else  

#ifdef IS_LITTLE_ENDIAN
#define LL_INIT(hi, lo) {PR_INT32(lo), PR_INT32(hi)}
#else
#define LL_INIT(hi, lo) {PR_INT32(hi), PR_INT32(lo)}
#endif

#define LL_IS_ZERO(a)        (((a).hi == 0) && ((a).lo == 0))
#define LL_EQ(a, b)        (((a).hi == (b).hi) && ((a).lo == (b).lo))
#define LL_NE(a, b)        (((a).hi != (b).hi) || ((a).lo != (b).lo))
#define LL_GE_ZERO(a)        (((a).hi >> 31) == 0)

#define LL_CMP(a, op, b)    (((a).hi == (b).hi) ? ((a).lo op (b).lo) : \
                 ((PRInt32)(a).hi op (PRInt32)(b).hi))
#define LL_UCMP(a, op, b)    (((a).hi == (b).hi) ? ((a).lo op (b).lo) : \
                 ((a).hi op (b).hi))

#define LL_AND(r, a, b)        ((r).lo = (a).lo & (b).lo, \
                 (r).hi = (a).hi & (b).hi)
#define LL_OR(r, a, b)        ((r).lo = (a).lo | (b).lo, \
                 (r).hi = (a).hi | (b).hi)
#define LL_XOR(r, a, b)        ((r).lo = (a).lo ^ (b).lo, \
                 (r).hi = (a).hi ^ (b).hi)
#define LL_OR2(r, a)        ((r).lo = (r).lo | (a).lo, \
                 (r).hi = (r).hi | (a).hi)
#define LL_NOT(r, a)        ((r).lo = ~(a).lo, \
                 (r).hi = ~(a).hi)

#define LL_NEG(r, a)        ((r).lo = -(PRInt32)(a).lo, \
                 (r).hi = -(PRInt32)(a).hi - ((r).lo != 0))
#define LL_ADD(r, a, b) { \
    PRInt64 _a, _b; \
    _a = a; _b = b; \
    (r).lo = _a.lo + _b.lo; \
    (r).hi = _a.hi + _b.hi + ((r).lo < _b.lo); \
}

#define LL_SUB(r, a, b) { \
    PRInt64 _a, _b; \
    _a = a; _b = b; \
    (r).lo = _a.lo - _b.lo; \
    (r).hi = _a.hi - _b.hi - (_a.lo < _b.lo); \
}

#define LL_MUL(r, a, b) { \
    PRInt64 _a, _b; \
    _a = a; _b = b; \
    LL_MUL32(r, _a.lo, _b.lo); \
    (r).hi += _a.hi * _b.lo + _a.lo * _b.hi; \
}

#define _lo16(a)        ((a) & PR_BITMASK(16))
#define _hi16(a)        ((a) >> 16)

#define LL_MUL32(r, a, b) { \
     PRUint32 _a1, _a0, _b1, _b0, _y0, _y1, _y2, _y3; \
     _a1 = _hi16(a), _a0 = _lo16(a); \
     _b1 = _hi16(b), _b0 = _lo16(b); \
     _y0 = _a0 * _b0; \
     _y1 = _a0 * _b1; \
     _y2 = _a1 * _b0; \
     _y3 = _a1 * _b1; \
     _y1 += _hi16(_y0);                          \
     _y1 += _y2;                                 \
     if (_y1 < _y2)    \
        _y3 += (PRUint32)(PR_BIT(16));   \
     (r).lo = (_lo16(_y1) << 16) + _lo16(_y0); \
     (r).hi = _y3 + _hi16(_y1); \
}

#define LL_UDIVMOD(qp, rp, a, b)    ll_udivmod(qp, rp, a, b)

NSPR_API(void) ll_udivmod(PRUint64 *qp, PRUint64 *rp, PRUint64 a, PRUint64 b);

#define LL_DIV(r, a, b) { \
    PRInt64 _a, _b; \
    PRUint32 _negative = (PRInt32)(a).hi < 0; \
    if (_negative) { \
    LL_NEG(_a, a); \
    } else { \
    _a = a; \
    } \
    if ((PRInt32)(b).hi < 0) { \
    _negative ^= 1; \
    LL_NEG(_b, b); \
    } else { \
    _b = b; \
    } \
    LL_UDIVMOD(&(r), 0, _a, _b); \
    if (_negative) \
    LL_NEG(r, r); \
}

#define LL_MOD(r, a, b) { \
    PRInt64 _a, _b; \
    PRUint32 _negative = (PRInt32)(a).hi < 0; \
    if (_negative) { \
    LL_NEG(_a, a); \
    } else { \
    _a = a; \
    } \
    if ((PRInt32)(b).hi < 0) { \
    LL_NEG(_b, b); \
    } else { \
    _b = b; \
    } \
    LL_UDIVMOD(0, &(r), _a, _b); \
    if (_negative) \
    LL_NEG(r, r); \
}

#define LL_SHL(r, a, b) { \
    if (b) { \
    PRInt64 _a; \
        _a = a; \
        if ((b) < 32) { \
        (r).lo = _a.lo << ((b) & 31); \
        (r).hi = (_a.hi << ((b) & 31)) | (_a.lo >> (32 - (b))); \
    } else { \
        (r).lo = 0; \
        (r).hi = _a.lo << ((b) & 31); \
    } \
    } else { \
    (r) = (a); \
    } \
}

#define LL_ISHL(r, a, b) { \
    if (b) { \
    PRInt64 _a; \
    _a.lo = (a); \
    _a.hi = 0; \
        if ((b) < 32) { \
        (r).lo = (a) << ((b) & 31); \
        (r).hi = ((a) >> (32 - (b))); \
    } else { \
        (r).lo = 0; \
        (r).hi = (a) << ((b) & 31); \
    } \
    } else { \
    (r).lo = (a); \
    (r).hi = 0; \
    } \
}

#define LL_SHR(r, a, b) { \
    if (b) { \
    PRInt64 _a; \
        _a = a; \
    if ((b) < 32) { \
        (r).lo = (_a.hi << (32 - (b))) | (_a.lo >> ((b) & 31)); \
        (r).hi = (PRInt32)_a.hi >> ((b) & 31); \
    } else { \
        (r).lo = (PRInt32)_a.hi >> ((b) & 31); \
        (r).hi = (PRInt32)_a.hi >> 31; \
    } \
    } else { \
    (r) = (a); \
    } \
}

#define LL_USHR(r, a, b) { \
    if (b) { \
    PRInt64 _a; \
        _a = a; \
    if ((b) < 32) { \
        (r).lo = (_a.hi << (32 - (b))) | (_a.lo >> ((b) & 31)); \
        (r).hi = _a.hi >> ((b) & 31); \
    } else { \
        (r).lo = _a.hi >> ((b) & 31); \
        (r).hi = 0; \
    } \
    } else { \
    (r) = (a); \
    } \
}

#define LL_L2I(i, l)        ((i) = (l).lo)
#define LL_L2UI(ui, l)        ((ui) = (l).lo)
#define LL_L2F(f, l)        { double _d; LL_L2D(_d, l); (f) = (PRFloat64)_d; }

#define LL_L2D(d, l) { \
    int _negative; \
    PRInt64 _absval; \
 \
    _negative = (l).hi >> 31; \
    if (_negative) { \
    LL_NEG(_absval, l); \
    } else { \
    _absval = l; \
    } \
    (d) = (double)_absval.hi * 4.294967296e9 + _absval.lo; \
    if (_negative) \
    (d) = -(d); \
}

#define LL_I2L(l, i)        { PRInt32 _i = ((PRInt32)(i)) >> 31; (l).lo = (i); (l).hi = _i; }
#define LL_UI2L(l, ui)      ((l).lo = (ui), (l).hi = 0)
#define LL_F2L(l, f)        { double _d = (double)f; LL_D2L(l, _d); }

#define LL_D2L(l, d) { \
    int _negative; \
    double _absval, _d_hi; \
    PRInt64 _lo_d; \
 \
    _negative = ((d) < 0); \
    _absval = _negative ? -(d) : (d); \
 \
    (l).hi = _absval / 4.294967296e9; \
    (l).lo = 0; \
    LL_L2D(_d_hi, l); \
    _absval -= _d_hi; \
    _lo_d.hi = 0; \
    if (_absval < 0) { \
    _lo_d.lo = -_absval; \
    LL_SUB(l, l, _lo_d); \
    } else { \
    _lo_d.lo = _absval; \
    LL_ADD(l, l, _lo_d); \
    } \
 \
    if (_negative) \
    LL_NEG(l, l); \
}

#endif 

PR_END_EXTERN_C

#endif 