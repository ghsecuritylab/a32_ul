// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SAFE_MATH_IMPL_H_
#define SAFE_MATH_IMPL_H_

#include <stdint.h>

#include <cmath>
#include <cstdlib>
#include <limits>

#include "base/macros.h"
#include "base/numerics/safe_conversions.h"
#include "base/template_util.h"

namespace base {
namespace internal {


template <size_t Size, bool IsSigned>
struct IntegerForSizeAndSign;
template <>
struct IntegerForSizeAndSign<1, true> {
  typedef int8_t type;
};
template <>
struct IntegerForSizeAndSign<1, false> {
  typedef uint8_t type;
};
template <>
struct IntegerForSizeAndSign<2, true> {
  typedef int16_t type;
};
template <>
struct IntegerForSizeAndSign<2, false> {
  typedef uint16_t type;
};
template <>
struct IntegerForSizeAndSign<4, true> {
  typedef int32_t type;
};
template <>
struct IntegerForSizeAndSign<4, false> {
  typedef uint32_t type;
};
template <>
struct IntegerForSizeAndSign<8, true> {
  typedef int64_t type;
};
template <>
struct IntegerForSizeAndSign<8, false> {
  typedef uint64_t type;
};


template <typename Integer>
struct UnsignedIntegerForSize {
  typedef typename enable_if<
      std::numeric_limits<Integer>::is_integer,
      typename IntegerForSizeAndSign<sizeof(Integer), false>::type>::type type;
};

template <typename Integer>
struct SignedIntegerForSize {
  typedef typename enable_if<
      std::numeric_limits<Integer>::is_integer,
      typename IntegerForSizeAndSign<sizeof(Integer), true>::type>::type type;
};

template <typename Integer>
struct TwiceWiderInteger {
  typedef typename enable_if<
      std::numeric_limits<Integer>::is_integer,
      typename IntegerForSizeAndSign<
          sizeof(Integer) * 2,
          std::numeric_limits<Integer>::is_signed>::type>::type type;
};

template <typename Integer>
struct PositionOfSignBit {
  static const typename enable_if<std::numeric_limits<Integer>::is_integer,
                                  size_t>::type value = 8 * sizeof(Integer) - 1;
};


template <typename T>
bool HasSignBit(T x) {
  
  return !!(static_cast<typename UnsignedIntegerForSize<T>::type>(x) >>
            PositionOfSignBit<T>::value);
}

template <typename T>
T BinaryComplement(T x) {
  return ~x;
}


template <typename T>
typename enable_if<std::numeric_limits<T>::is_integer, T>::type
CheckedAdd(T x, T y, RangeConstraint* validity) {
  
  
  typedef typename UnsignedIntegerForSize<T>::type UnsignedDst;
  UnsignedDst ux = static_cast<UnsignedDst>(x);
  UnsignedDst uy = static_cast<UnsignedDst>(y);
  UnsignedDst uresult = ux + uy;
  
  
  if (std::numeric_limits<T>::is_signed) {
    if (HasSignBit(BinaryComplement((uresult ^ ux) & (uresult ^ uy))))
      *validity = RANGE_VALID;
    else  
      *validity = HasSignBit(uresult) ? RANGE_OVERFLOW : RANGE_UNDERFLOW;

  } else {  
    *validity = BinaryComplement(x) >= y ? RANGE_VALID : RANGE_OVERFLOW;
  }
  return static_cast<T>(uresult);
}

template <typename T>
typename enable_if<std::numeric_limits<T>::is_integer, T>::type
CheckedSub(T x, T y, RangeConstraint* validity) {
  
  
  typedef typename UnsignedIntegerForSize<T>::type UnsignedDst;
  UnsignedDst ux = static_cast<UnsignedDst>(x);
  UnsignedDst uy = static_cast<UnsignedDst>(y);
  UnsignedDst uresult = ux - uy;
  
  
  if (std::numeric_limits<T>::is_signed) {
    if (HasSignBit(BinaryComplement((uresult ^ ux) & (ux ^ uy))))
      *validity = RANGE_VALID;
    else  
      *validity = HasSignBit(uresult) ? RANGE_OVERFLOW : RANGE_UNDERFLOW;

  } else {  
    *validity = x >= y ? RANGE_VALID : RANGE_UNDERFLOW;
  }
  return static_cast<T>(uresult);
}

template <typename T>
typename enable_if<
    std::numeric_limits<T>::is_integer && sizeof(T) * 2 <= sizeof(uintmax_t),
    T>::type
CheckedMul(T x, T y, RangeConstraint* validity) {
  typedef typename TwiceWiderInteger<T>::type IntermediateType;
  IntermediateType tmp =
      static_cast<IntermediateType>(x) * static_cast<IntermediateType>(y);
  *validity = DstRangeRelationToSrcRange<T>(tmp);
  return static_cast<T>(tmp);
}

template <typename T>
typename enable_if<std::numeric_limits<T>::is_integer&& std::numeric_limits<
                       T>::is_signed&&(sizeof(T) * 2 > sizeof(uintmax_t)),
                   T>::type
CheckedMul(T x, T y, RangeConstraint* validity) {
  
  if (!(x || y)) {
    return RANGE_VALID;

  } else if (x > 0) {
    if (y > 0)
      *validity =
          x <= std::numeric_limits<T>::max() / y ? RANGE_VALID : RANGE_OVERFLOW;
    else
      *validity = y >= std::numeric_limits<T>::min() / x ? RANGE_VALID
                                                         : RANGE_UNDERFLOW;

  } else {
    if (y > 0)
      *validity = x >= std::numeric_limits<T>::min() / y ? RANGE_VALID
                                                         : RANGE_UNDERFLOW;
    else
      *validity =
          y >= std::numeric_limits<T>::max() / x ? RANGE_VALID : RANGE_OVERFLOW;
  }

  return x * y;
}

template <typename T>
typename enable_if<std::numeric_limits<T>::is_integer &&
                       !std::numeric_limits<T>::is_signed &&
                       (sizeof(T) * 2 > sizeof(uintmax_t)),
                   T>::type
CheckedMul(T x, T y, RangeConstraint* validity) {
  *validity = (y == 0 || x <= std::numeric_limits<T>::max() / y)
                  ? RANGE_VALID
                  : RANGE_OVERFLOW;
  return x * y;
}

template <typename T>
T CheckedDiv(
    T x,
    T y,
    RangeConstraint* validity,
    typename enable_if<std::numeric_limits<T>::is_integer, int>::type = 0) {
  if (std::numeric_limits<T>::is_signed && x == std::numeric_limits<T>::min() &&
      y == static_cast<T>(-1)) {
    *validity = RANGE_OVERFLOW;
    return std::numeric_limits<T>::min();
  }

  *validity = RANGE_VALID;
  return x / y;
}

template <typename T>
typename enable_if<
    std::numeric_limits<T>::is_integer&& std::numeric_limits<T>::is_signed,
    T>::type
CheckedMod(T x, T y, RangeConstraint* validity) {
  *validity = y > 0 ? RANGE_VALID : RANGE_INVALID;
  return x % y;
}

template <typename T>
typename enable_if<
    std::numeric_limits<T>::is_integer && !std::numeric_limits<T>::is_signed,
    T>::type
CheckedMod(T x, T y, RangeConstraint* validity) {
  *validity = RANGE_VALID;
  return x % y;
}

template <typename T>
typename enable_if<
    std::numeric_limits<T>::is_integer&& std::numeric_limits<T>::is_signed,
    T>::type
CheckedNeg(T value, RangeConstraint* validity) {
  *validity =
      value != std::numeric_limits<T>::min() ? RANGE_VALID : RANGE_OVERFLOW;
  
  return -value;
}

template <typename T>
typename enable_if<
    std::numeric_limits<T>::is_integer && !std::numeric_limits<T>::is_signed,
    T>::type
CheckedNeg(T value, RangeConstraint* validity) {
  
  *validity = value ? RANGE_UNDERFLOW : RANGE_VALID;
  return static_cast<T>(
      -static_cast<typename SignedIntegerForSize<T>::type>(value));
}

template <typename T>
typename enable_if<
    std::numeric_limits<T>::is_integer&& std::numeric_limits<T>::is_signed,
    T>::type
CheckedAbs(T value, RangeConstraint* validity) {
  *validity =
      value != std::numeric_limits<T>::min() ? RANGE_VALID : RANGE_OVERFLOW;
  return std::abs(value);
}

template <typename T>
typename enable_if<
    std::numeric_limits<T>::is_integer && !std::numeric_limits<T>::is_signed,
    T>::type
CheckedAbs(T value, RangeConstraint* validity) {
  
  *validity = RANGE_VALID;
  return value;
}

#define BASE_FLOAT_ARITHMETIC_STUBS(NAME)                        \
  template <typename T>                                          \
  typename enable_if<std::numeric_limits<T>::is_iec559, T>::type \
  Checked##NAME(T, T, RangeConstraint*) {                        \
    NOTREACHED();                                                \
    return 0;                                                    \
  }

BASE_FLOAT_ARITHMETIC_STUBS(Add)
BASE_FLOAT_ARITHMETIC_STUBS(Sub)
BASE_FLOAT_ARITHMETIC_STUBS(Mul)
BASE_FLOAT_ARITHMETIC_STUBS(Div)
BASE_FLOAT_ARITHMETIC_STUBS(Mod)

#undef BASE_FLOAT_ARITHMETIC_STUBS

template <typename T>
typename enable_if<std::numeric_limits<T>::is_iec559, T>::type CheckedNeg(
    T value,
    RangeConstraint*) {
  return -value;
}

template <typename T>
typename enable_if<std::numeric_limits<T>::is_iec559, T>::type CheckedAbs(
    T value,
    RangeConstraint*) {
  return std::abs(value);
}

enum NumericRepresentation {
  NUMERIC_INTEGER,
  NUMERIC_FLOATING,
  NUMERIC_UNKNOWN
};

template <typename NumericType>
struct GetNumericRepresentation {
  static const NumericRepresentation value =
      std::numeric_limits<NumericType>::is_integer
          ? NUMERIC_INTEGER
          : (std::numeric_limits<NumericType>::is_iec559 ? NUMERIC_FLOATING
                                                         : NUMERIC_UNKNOWN);
};

template <typename T, NumericRepresentation type =
                          GetNumericRepresentation<T>::value>
class CheckedNumericState {};

template <typename T>
class CheckedNumericState<T, NUMERIC_INTEGER> {
 private:
  T value_;
  RangeConstraint validity_;

 public:
  template <typename Src, NumericRepresentation type>
  friend class CheckedNumericState;

  CheckedNumericState() : value_(0), validity_(RANGE_VALID) {}

  template <typename Src>
  CheckedNumericState(Src value, RangeConstraint validity)
      : value_(value),
        validity_(GetRangeConstraint(validity |
                                     DstRangeRelationToSrcRange<T>(value))) {
    COMPILE_ASSERT(std::numeric_limits<Src>::is_specialized,
                   argument_must_be_numeric);
  }

  
  template <typename Src>
  CheckedNumericState(const CheckedNumericState<Src>& rhs)
      : value_(static_cast<T>(rhs.value())),
        validity_(GetRangeConstraint(
            rhs.validity() | DstRangeRelationToSrcRange<T>(rhs.value()))) {}

  template <typename Src>
  explicit CheckedNumericState(
      Src value,
      typename enable_if<std::numeric_limits<Src>::is_specialized, int>::type =
          0)
      : value_(static_cast<T>(value)),
        validity_(DstRangeRelationToSrcRange<T>(value)) {}

  RangeConstraint validity() const { return validity_; }
  T value() const { return value_; }
};

template <typename T>
class CheckedNumericState<T, NUMERIC_FLOATING> {
 private:
  T value_;

 public:
  template <typename Src, NumericRepresentation type>
  friend class CheckedNumericState;

  CheckedNumericState() : value_(0.0) {}

  template <typename Src>
  CheckedNumericState(
      Src value,
      RangeConstraint validity,
      typename enable_if<std::numeric_limits<Src>::is_integer, int>::type = 0) {
    switch (DstRangeRelationToSrcRange<T>(value)) {
      case RANGE_VALID:
        value_ = static_cast<T>(value);
        break;

      case RANGE_UNDERFLOW:
        value_ = -std::numeric_limits<T>::infinity();
        break;

      case RANGE_OVERFLOW:
        value_ = std::numeric_limits<T>::infinity();
        break;

      case RANGE_INVALID:
        value_ = std::numeric_limits<T>::quiet_NaN();
        break;

      default:
        NOTREACHED();
    }
  }

  template <typename Src>
  explicit CheckedNumericState(
      Src value,
      typename enable_if<std::numeric_limits<Src>::is_specialized, int>::type =
          0)
      : value_(static_cast<T>(value)) {}

  
  template <typename Src>
  CheckedNumericState(const CheckedNumericState<Src>& rhs)
      : value_(static_cast<T>(rhs.value())) {}

  RangeConstraint validity() const {
    return GetRangeConstraint(value_ <= std::numeric_limits<T>::max(),
                              value_ >= -std::numeric_limits<T>::max());
  }
  T value() const { return value_; }
};

enum ArithmeticPromotionCategory {
  LEFT_PROMOTION,
  RIGHT_PROMOTION,
  DEFAULT_PROMOTION
};

template <typename Lhs,
          typename Rhs = Lhs,
          ArithmeticPromotionCategory Promotion =
              (MaxExponent<Lhs>::value > MaxExponent<Rhs>::value)
                  ? (MaxExponent<Lhs>::value > MaxExponent<int>::value
                         ? LEFT_PROMOTION
                         : DEFAULT_PROMOTION)
                  : (MaxExponent<Rhs>::value > MaxExponent<int>::value
                         ? RIGHT_PROMOTION
                         : DEFAULT_PROMOTION) >
struct ArithmeticPromotion;

template <typename Lhs, typename Rhs>
struct ArithmeticPromotion<Lhs, Rhs, LEFT_PROMOTION> {
  typedef Lhs type;
};

template <typename Lhs, typename Rhs>
struct ArithmeticPromotion<Lhs, Rhs, RIGHT_PROMOTION> {
  typedef Rhs type;
};

template <typename Lhs, typename Rhs>
struct ArithmeticPromotion<Lhs, Rhs, DEFAULT_PROMOTION> {
  typedef int type;
};

template <typename T, typename Lhs, typename Rhs>
struct IsIntegerArithmeticSafe {
  static const bool value = !std::numeric_limits<T>::is_iec559 &&
                            StaticDstRangeRelationToSrcRange<T, Lhs>::value ==
                                NUMERIC_RANGE_CONTAINED &&
                            sizeof(T) >= (2 * sizeof(Lhs)) &&
                            StaticDstRangeRelationToSrcRange<T, Rhs>::value !=
                                NUMERIC_RANGE_CONTAINED &&
                            sizeof(T) >= (2 * sizeof(Rhs));
};

}  
}  

#endif  