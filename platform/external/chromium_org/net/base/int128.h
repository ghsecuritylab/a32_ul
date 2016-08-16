// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_INT128_H_
#define NET_BASE_INT128_H_

#include <iosfwd>
#include "base/basictypes.h"
#include "net/base/net_export.h"

struct uint128_pod;

class uint128 {
public:
  uint128();  
  uint128(uint64 top, uint64 bottom);
  uint128(int bottom);
  uint128(uint32 bottom);   
  uint128(uint64 bottom);   
  uint128(const uint128 &val);
  uint128(const uint128_pod &val);

  void Initialize(uint64 top, uint64 bottom);

  uint128& operator=(const uint128& b);

  
  
  uint128& operator+=(const uint128& b);
  uint128& operator-=(const uint128& b);
  uint128& operator*=(const uint128& b);
  uint128 operator++(int);
  uint128 operator--(int);
  uint128& operator<<=(int);
  uint128& operator>>=(int);
  uint128& operator&=(const uint128& b);
  uint128& operator|=(const uint128& b);
  uint128& operator^=(const uint128& b);
  uint128& operator++();
  uint128& operator--();

  friend uint64 Uint128Low64(const uint128& v);
  friend uint64 Uint128High64(const uint128& v);

  
  friend NET_EXPORT_PRIVATE std::ostream& operator<<(std::ostream& o,
                                                     const uint128& b);

private:
  
  
  
  uint64        lo_;
  uint64        hi_;

  
  uint128(uint8);
  uint128(uint16);
  uint128(float v);
  uint128(double v);
};

struct uint128_pod {
  
  
  
  
  
  uint64 hi;
  uint64 lo;
};

NET_EXPORT_PRIVATE extern const uint128_pod kuint128max;

NET_EXPORT_PRIVATE extern std::ostream& operator<<(std::ostream& o,
                                                   const uint128& b);

inline uint64 Uint128Low64(const uint128& v) { return v.lo_; }
inline uint64 Uint128High64(const uint128& v) { return v.hi_; }


inline bool operator==(const uint128& lhs, const uint128& rhs) {
  return (Uint128Low64(lhs) == Uint128Low64(rhs) &&
          Uint128High64(lhs) == Uint128High64(rhs));
}
inline bool operator!=(const uint128& lhs, const uint128& rhs) {
  return !(lhs == rhs);
}
inline uint128& uint128::operator=(const uint128& b) {
  lo_ = b.lo_;
  hi_ = b.hi_;
  return *this;
}

inline uint128::uint128(): lo_(0), hi_(0) { }
inline uint128::uint128(uint64 top, uint64 bottom) : lo_(bottom), hi_(top) { }
inline uint128::uint128(const uint128 &v) : lo_(v.lo_), hi_(v.hi_) { }
inline uint128::uint128(const uint128_pod &v) : lo_(v.lo), hi_(v.hi) { }
inline uint128::uint128(uint64 bottom) : lo_(bottom), hi_(0) { }
inline uint128::uint128(uint32 bottom) : lo_(bottom), hi_(0) { }
inline uint128::uint128(int bottom) : lo_(bottom), hi_(0) {
  if (bottom < 0) {
    --hi_;
  }
}
inline void uint128::Initialize(uint64 top, uint64 bottom) {
  hi_ = top;
  lo_ = bottom;
}


#define CMP128(op)                                                \
inline bool operator op(const uint128& lhs, const uint128& rhs) { \
  return (Uint128High64(lhs) == Uint128High64(rhs)) ?             \
      (Uint128Low64(lhs) op Uint128Low64(rhs)) :                  \
      (Uint128High64(lhs) op Uint128High64(rhs));                 \
}

CMP128(<)
CMP128(>)
CMP128(>=)
CMP128(<=)

#undef CMP128


inline uint128 operator-(const uint128& val) {
  const uint64 hi_flip = ~Uint128High64(val);
  const uint64 lo_flip = ~Uint128Low64(val);
  const uint64 lo_add = lo_flip + 1;
  if (lo_add < lo_flip) {
    return uint128(hi_flip + 1, lo_add);
  }
  return uint128(hi_flip, lo_add);
}

inline bool operator!(const uint128& val) {
  return !Uint128High64(val) && !Uint128Low64(val);
}


inline uint128 operator~(const uint128& val) {
  return uint128(~Uint128High64(val), ~Uint128Low64(val));
}

#define LOGIC128(op)                                                 \
inline uint128 operator op(const uint128& lhs, const uint128& rhs) { \
  return uint128(Uint128High64(lhs) op Uint128High64(rhs),           \
                 Uint128Low64(lhs) op Uint128Low64(rhs));            \
}

LOGIC128(|)
LOGIC128(&)
LOGIC128(^)

#undef LOGIC128

#define LOGICASSIGN128(op)                                   \
inline uint128& uint128::operator op(const uint128& other) { \
  hi_ op other.hi_;                                          \
  lo_ op other.lo_;                                          \
  return *this;                                              \
}

LOGICASSIGN128(|=)
LOGICASSIGN128(&=)
LOGICASSIGN128(^=)

#undef LOGICASSIGN128


inline uint128 operator<<(const uint128& val, int amount) {
  
  if (amount < 64) {
    if (amount == 0) {
      return val;
    }
    uint64 new_hi = (Uint128High64(val) << amount) |
                    (Uint128Low64(val) >> (64 - amount));
    uint64 new_lo = Uint128Low64(val) << amount;
    return uint128(new_hi, new_lo);
  } else if (amount < 128) {
    return uint128(Uint128Low64(val) << (amount - 64), 0);
  } else {
    return uint128(0, 0);
  }
}

inline uint128 operator>>(const uint128& val, int amount) {
  
  if (amount < 64) {
    if (amount == 0) {
      return val;
    }
    uint64 new_hi = Uint128High64(val) >> amount;
    uint64 new_lo = (Uint128Low64(val) >> amount) |
                    (Uint128High64(val) << (64 - amount));
    return uint128(new_hi, new_lo);
  } else if (amount < 128) {
    return uint128(0, Uint128High64(val) >> (amount - 64));
  } else {
    return uint128(0, 0);
  }
}

inline uint128& uint128::operator<<=(int amount) {
  
  if (amount < 64) {
    if (amount != 0) {
      hi_ = (hi_ << amount) | (lo_ >> (64 - amount));
      lo_ = lo_ << amount;
    }
  } else if (amount < 128) {
    hi_ = lo_ << (amount - 64);
    lo_ = 0;
  } else {
    hi_ = 0;
    lo_ = 0;
  }
  return *this;
}

inline uint128& uint128::operator>>=(int amount) {
  
  if (amount < 64) {
    if (amount != 0) {
      lo_ = (lo_ >> amount) | (hi_ << (64 - amount));
      hi_ = hi_ >> amount;
    }
  } else if (amount < 128) {
    hi_ = 0;
    lo_ = hi_ >> (amount - 64);
  } else {
    hi_ = 0;
    lo_ = 0;
  }
  return *this;
}

inline uint128 operator+(const uint128& lhs, const uint128& rhs) {
  return uint128(lhs) += rhs;
}

inline uint128 operator-(const uint128& lhs, const uint128& rhs) {
  return uint128(lhs) -= rhs;
}

inline uint128 operator*(const uint128& lhs, const uint128& rhs) {
  return uint128(lhs) *= rhs;
}

inline uint128& uint128::operator+=(const uint128& b) {
  hi_ += b.hi_;
  uint64 lolo = lo_ + b.lo_;
  if (lolo < lo_)
    ++hi_;
  lo_ = lolo;
  return *this;
}

inline uint128& uint128::operator-=(const uint128& b) {
  hi_ -= b.hi_;
  if (b.lo_ > lo_)
    --hi_;
  lo_ -= b.lo_;
  return *this;
}

inline uint128& uint128::operator*=(const uint128& b) {
  uint64 a96 = hi_ >> 32;
  uint64 a64 = hi_ & 0xffffffffu;
  uint64 a32 = lo_ >> 32;
  uint64 a00 = lo_ & 0xffffffffu;
  uint64 b96 = b.hi_ >> 32;
  uint64 b64 = b.hi_ & 0xffffffffu;
  uint64 b32 = b.lo_ >> 32;
  uint64 b00 = b.lo_ & 0xffffffffu;
  
  
  
  uint64 c96 = a96 * b00 + a64 * b32 + a32 * b64 + a00 * b96;
  uint64 c64 = a64 * b00 + a32 * b32 + a00 * b64;
  this->hi_ = (c96 << 32) + c64;
  this->lo_ = 0;
  
  *this += uint128(a32 * b00) << 32;
  *this += uint128(a00 * b32) << 32;
  *this += a00 * b00;
  return *this;
}

inline uint128 uint128::operator++(int) {
  uint128 tmp(*this);
  *this += 1;
  return tmp;
}

inline uint128 uint128::operator--(int) {
  uint128 tmp(*this);
  *this -= 1;
  return tmp;
}

inline uint128& uint128::operator++() {
  *this += 1;
  return *this;
}

inline uint128& uint128::operator--() {
  *this -= 1;
  return *this;
}

#endif  