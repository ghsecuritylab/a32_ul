// Copyright 2011 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#include "./dsp.h"

#if defined(WEBP_USE_SSE2)


#include <emmintrin.h>
#include "../dec/vp8i.h"


static void Transform(const int16_t* in, uint8_t* dst, int do_two) {
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  const __m128i k1 = _mm_set1_epi16(20091);
  const __m128i k2 = _mm_set1_epi16(-30068);
  __m128i T0, T1, T2, T3;

  
  
  
  __m128i in0, in1, in2, in3;
  {
    in0 = _mm_loadl_epi64((__m128i*)&in[0]);
    in1 = _mm_loadl_epi64((__m128i*)&in[4]);
    in2 = _mm_loadl_epi64((__m128i*)&in[8]);
    in3 = _mm_loadl_epi64((__m128i*)&in[12]);
    
    
    
    
    if (do_two) {
      const __m128i inB0 = _mm_loadl_epi64((__m128i*)&in[16]);
      const __m128i inB1 = _mm_loadl_epi64((__m128i*)&in[20]);
      const __m128i inB2 = _mm_loadl_epi64((__m128i*)&in[24]);
      const __m128i inB3 = _mm_loadl_epi64((__m128i*)&in[28]);
      in0 = _mm_unpacklo_epi64(in0, inB0);
      in1 = _mm_unpacklo_epi64(in1, inB1);
      in2 = _mm_unpacklo_epi64(in2, inB2);
      in3 = _mm_unpacklo_epi64(in3, inB3);
      
      
      
      
    }
  }

  
  {
    
    
    const __m128i a = _mm_add_epi16(in0, in2);
    const __m128i b = _mm_sub_epi16(in0, in2);
    
    const __m128i c1 = _mm_mulhi_epi16(in1, k2);
    const __m128i c2 = _mm_mulhi_epi16(in3, k1);
    const __m128i c3 = _mm_sub_epi16(in1, in3);
    const __m128i c4 = _mm_sub_epi16(c1, c2);
    const __m128i c = _mm_add_epi16(c3, c4);
    
    const __m128i d1 = _mm_mulhi_epi16(in1, k1);
    const __m128i d2 = _mm_mulhi_epi16(in3, k2);
    const __m128i d3 = _mm_add_epi16(in1, in3);
    const __m128i d4 = _mm_add_epi16(d1, d2);
    const __m128i d = _mm_add_epi16(d3, d4);

    
    const __m128i tmp0 = _mm_add_epi16(a, d);
    const __m128i tmp1 = _mm_add_epi16(b, c);
    const __m128i tmp2 = _mm_sub_epi16(b, c);
    const __m128i tmp3 = _mm_sub_epi16(a, d);

    
    
    
    
    
    const __m128i transpose0_0 = _mm_unpacklo_epi16(tmp0, tmp1);
    const __m128i transpose0_1 = _mm_unpacklo_epi16(tmp2, tmp3);
    const __m128i transpose0_2 = _mm_unpackhi_epi16(tmp0, tmp1);
    const __m128i transpose0_3 = _mm_unpackhi_epi16(tmp2, tmp3);
    
    
    
    
    const __m128i transpose1_0 = _mm_unpacklo_epi32(transpose0_0, transpose0_1);
    const __m128i transpose1_1 = _mm_unpacklo_epi32(transpose0_2, transpose0_3);
    const __m128i transpose1_2 = _mm_unpackhi_epi32(transpose0_0, transpose0_1);
    const __m128i transpose1_3 = _mm_unpackhi_epi32(transpose0_2, transpose0_3);
    
    
    
    
    T0 = _mm_unpacklo_epi64(transpose1_0, transpose1_1);
    T1 = _mm_unpackhi_epi64(transpose1_0, transpose1_1);
    T2 = _mm_unpacklo_epi64(transpose1_2, transpose1_3);
    T3 = _mm_unpackhi_epi64(transpose1_2, transpose1_3);
    
    
    
    
  }

  
  {
    
    
    const __m128i four = _mm_set1_epi16(4);
    const __m128i dc = _mm_add_epi16(T0, four);
    const __m128i a =  _mm_add_epi16(dc, T2);
    const __m128i b =  _mm_sub_epi16(dc, T2);
    
    const __m128i c1 = _mm_mulhi_epi16(T1, k2);
    const __m128i c2 = _mm_mulhi_epi16(T3, k1);
    const __m128i c3 = _mm_sub_epi16(T1, T3);
    const __m128i c4 = _mm_sub_epi16(c1, c2);
    const __m128i c = _mm_add_epi16(c3, c4);
    
    const __m128i d1 = _mm_mulhi_epi16(T1, k1);
    const __m128i d2 = _mm_mulhi_epi16(T3, k2);
    const __m128i d3 = _mm_add_epi16(T1, T3);
    const __m128i d4 = _mm_add_epi16(d1, d2);
    const __m128i d = _mm_add_epi16(d3, d4);

    
    const __m128i tmp0 = _mm_add_epi16(a, d);
    const __m128i tmp1 = _mm_add_epi16(b, c);
    const __m128i tmp2 = _mm_sub_epi16(b, c);
    const __m128i tmp3 = _mm_sub_epi16(a, d);
    const __m128i shifted0 = _mm_srai_epi16(tmp0, 3);
    const __m128i shifted1 = _mm_srai_epi16(tmp1, 3);
    const __m128i shifted2 = _mm_srai_epi16(tmp2, 3);
    const __m128i shifted3 = _mm_srai_epi16(tmp3, 3);

    
    
    
    
    
    const __m128i transpose0_0 = _mm_unpacklo_epi16(shifted0, shifted1);
    const __m128i transpose0_1 = _mm_unpacklo_epi16(shifted2, shifted3);
    const __m128i transpose0_2 = _mm_unpackhi_epi16(shifted0, shifted1);
    const __m128i transpose0_3 = _mm_unpackhi_epi16(shifted2, shifted3);
    
    
    
    
    const __m128i transpose1_0 = _mm_unpacklo_epi32(transpose0_0, transpose0_1);
    const __m128i transpose1_1 = _mm_unpacklo_epi32(transpose0_2, transpose0_3);
    const __m128i transpose1_2 = _mm_unpackhi_epi32(transpose0_0, transpose0_1);
    const __m128i transpose1_3 = _mm_unpackhi_epi32(transpose0_2, transpose0_3);
    
    
    
    
    T0 = _mm_unpacklo_epi64(transpose1_0, transpose1_1);
    T1 = _mm_unpackhi_epi64(transpose1_0, transpose1_1);
    T2 = _mm_unpacklo_epi64(transpose1_2, transpose1_3);
    T3 = _mm_unpackhi_epi64(transpose1_2, transpose1_3);
    
    
    
    
  }

  
  {
    const __m128i zero = _mm_setzero_si128();
    
    __m128i dst0, dst1, dst2, dst3;
    if (do_two) {
      
      dst0 = _mm_loadl_epi64((__m128i*)(dst + 0 * BPS));
      dst1 = _mm_loadl_epi64((__m128i*)(dst + 1 * BPS));
      dst2 = _mm_loadl_epi64((__m128i*)(dst + 2 * BPS));
      dst3 = _mm_loadl_epi64((__m128i*)(dst + 3 * BPS));
    } else {
      
      dst0 = _mm_cvtsi32_si128(*(int*)(dst + 0 * BPS));
      dst1 = _mm_cvtsi32_si128(*(int*)(dst + 1 * BPS));
      dst2 = _mm_cvtsi32_si128(*(int*)(dst + 2 * BPS));
      dst3 = _mm_cvtsi32_si128(*(int*)(dst + 3 * BPS));
    }
    
    dst0 = _mm_unpacklo_epi8(dst0, zero);
    dst1 = _mm_unpacklo_epi8(dst1, zero);
    dst2 = _mm_unpacklo_epi8(dst2, zero);
    dst3 = _mm_unpacklo_epi8(dst3, zero);
    
    dst0 = _mm_add_epi16(dst0, T0);
    dst1 = _mm_add_epi16(dst1, T1);
    dst2 = _mm_add_epi16(dst2, T2);
    dst3 = _mm_add_epi16(dst3, T3);
    
    dst0 = _mm_packus_epi16(dst0, dst0);
    dst1 = _mm_packus_epi16(dst1, dst1);
    dst2 = _mm_packus_epi16(dst2, dst2);
    dst3 = _mm_packus_epi16(dst3, dst3);
    
    if (do_two) {
      
      _mm_storel_epi64((__m128i*)(dst + 0 * BPS), dst0);
      _mm_storel_epi64((__m128i*)(dst + 1 * BPS), dst1);
      _mm_storel_epi64((__m128i*)(dst + 2 * BPS), dst2);
      _mm_storel_epi64((__m128i*)(dst + 3 * BPS), dst3);
    } else {
      
      *(int*)(dst + 0 * BPS) = _mm_cvtsi128_si32(dst0);
      *(int*)(dst + 1 * BPS) = _mm_cvtsi128_si32(dst1);
      *(int*)(dst + 2 * BPS) = _mm_cvtsi128_si32(dst2);
      *(int*)(dst + 3 * BPS) = _mm_cvtsi128_si32(dst3);
    }
  }
}

#if defined(USE_TRANSFORM_AC3)
#define MUL(a, b) (((a) * (b)) >> 16)
static void TransformAC3(const int16_t* in, uint8_t* dst) {
  static const int kC1 = 20091 + (1 << 16);
  static const int kC2 = 35468;
  const __m128i A = _mm_set1_epi16(in[0] + 4);
  const __m128i c4 = _mm_set1_epi16(MUL(in[4], kC2));
  const __m128i d4 = _mm_set1_epi16(MUL(in[4], kC1));
  const int c1 = MUL(in[1], kC2);
  const int d1 = MUL(in[1], kC1);
  const __m128i CD = _mm_set_epi16(0, 0, 0, 0, -d1, -c1, c1, d1);
  const __m128i B = _mm_adds_epi16(A, CD);
  const __m128i m0 = _mm_adds_epi16(B, d4);
  const __m128i m1 = _mm_adds_epi16(B, c4);
  const __m128i m2 = _mm_subs_epi16(B, c4);
  const __m128i m3 = _mm_subs_epi16(B, d4);
  const __m128i zero = _mm_setzero_si128();
  
  __m128i dst0 = _mm_cvtsi32_si128(*(int*)(dst + 0 * BPS));
  __m128i dst1 = _mm_cvtsi32_si128(*(int*)(dst + 1 * BPS));
  __m128i dst2 = _mm_cvtsi32_si128(*(int*)(dst + 2 * BPS));
  __m128i dst3 = _mm_cvtsi32_si128(*(int*)(dst + 3 * BPS));
  
  dst0 = _mm_unpacklo_epi8(dst0, zero);
  dst1 = _mm_unpacklo_epi8(dst1, zero);
  dst2 = _mm_unpacklo_epi8(dst2, zero);
  dst3 = _mm_unpacklo_epi8(dst3, zero);
  
  dst0 = _mm_adds_epi16(dst0, _mm_srai_epi16(m0, 3));
  dst1 = _mm_adds_epi16(dst1, _mm_srai_epi16(m1, 3));
  dst2 = _mm_adds_epi16(dst2, _mm_srai_epi16(m2, 3));
  dst3 = _mm_adds_epi16(dst3, _mm_srai_epi16(m3, 3));
  
  dst0 = _mm_packus_epi16(dst0, dst0);
  dst1 = _mm_packus_epi16(dst1, dst1);
  dst2 = _mm_packus_epi16(dst2, dst2);
  dst3 = _mm_packus_epi16(dst3, dst3);
  
  *(int*)(dst + 0 * BPS) = _mm_cvtsi128_si32(dst0);
  *(int*)(dst + 1 * BPS) = _mm_cvtsi128_si32(dst1);
  *(int*)(dst + 2 * BPS) = _mm_cvtsi128_si32(dst2);
  *(int*)(dst + 3 * BPS) = _mm_cvtsi128_si32(dst3);
}
#undef MUL
#endif   


#define MM_ABS(p, q)  _mm_or_si128(                                            \
    _mm_subs_epu8((q), (p)),                                                   \
    _mm_subs_epu8((p), (q)))

static WEBP_INLINE void SignedShift8b(__m128i* const x) {
  const __m128i zero = _mm_setzero_si128();
  const __m128i signs = _mm_cmpgt_epi8(zero, *x);
  const __m128i lo_0 = _mm_unpacklo_epi8(*x, signs);  
  const __m128i hi_0 = _mm_unpackhi_epi8(*x, signs);
  const __m128i lo_1 = _mm_srai_epi16(lo_0, 3);
  const __m128i hi_1 = _mm_srai_epi16(hi_0, 3);
  *x = _mm_packs_epi16(lo_1, hi_1);
}

#define FLIP_SIGN_BIT2(a, b) {                                                 \
  a = _mm_xor_si128(a, sign_bit);                                              \
  b = _mm_xor_si128(b, sign_bit);                                              \
}

#define FLIP_SIGN_BIT4(a, b, c, d) {                                           \
  FLIP_SIGN_BIT2(a, b);                                                        \
  FLIP_SIGN_BIT2(c, d);                                                        \
}

static WEBP_INLINE void GetNotHEV(const __m128i* const p1,
                                  const __m128i* const p0,
                                  const __m128i* const q0,
                                  const __m128i* const q1,
                                  int hev_thresh, __m128i* const not_hev) {
  const __m128i zero = _mm_setzero_si128();
  const __m128i t_1 = MM_ABS(*p1, *p0);
  const __m128i t_2 = MM_ABS(*q1, *q0);

  const __m128i h = _mm_set1_epi8(hev_thresh);
  const __m128i t_3 = _mm_subs_epu8(t_1, h);  
  const __m128i t_4 = _mm_subs_epu8(t_2, h);  

  *not_hev = _mm_or_si128(t_3, t_4);
  *not_hev = _mm_cmpeq_epi8(*not_hev, zero);  
}

static WEBP_INLINE void GetBaseDelta(const __m128i* const p1,
                                     const __m128i* const p0,
                                     const __m128i* const q0,
                                     const __m128i* const q1,
                                     __m128i* const delta) {
  
  const __m128i p1_q1 = _mm_subs_epi8(*p1, *q1);   
  const __m128i q0_p0 = _mm_subs_epi8(*q0, *p0);   
  const __m128i s1 = _mm_adds_epi8(p1_q1, q0_p0);  
  const __m128i s2 = _mm_adds_epi8(q0_p0, s1);     
  const __m128i s3 = _mm_adds_epi8(q0_p0, s2);     
  *delta = s3;
}

static WEBP_INLINE void DoSimpleFilter(__m128i* const p0, __m128i* const q0,
                                       const __m128i* const fl) {
  const __m128i k3 = _mm_set1_epi8(3);
  const __m128i k4 = _mm_set1_epi8(4);
  __m128i v3 = _mm_adds_epi8(*fl, k3);
  __m128i v4 = _mm_adds_epi8(*fl, k4);

  SignedShift8b(&v4);                  
  SignedShift8b(&v3);                  
  *q0 = _mm_subs_epi8(*q0, v4);        
  *p0 = _mm_adds_epi8(*p0, v3);        
}

static WEBP_INLINE void Update2Pixels(__m128i* const pi, __m128i* const qi,
                                      const __m128i* const a0_lo,
                                      const __m128i* const a0_hi) {
  const __m128i a1_lo = _mm_srai_epi16(*a0_lo, 7);
  const __m128i a1_hi = _mm_srai_epi16(*a0_hi, 7);
  const __m128i delta = _mm_packs_epi16(a1_lo, a1_hi);
  const __m128i sign_bit = _mm_set1_epi8(0x80);
  *pi = _mm_adds_epi8(*pi, delta);
  *qi = _mm_subs_epi8(*qi, delta);
  FLIP_SIGN_BIT2(*pi, *qi);
}

static WEBP_INLINE void NeedsFilter(const __m128i* const p1,
                                    const __m128i* const p0,
                                    const __m128i* const q0,
                                    const __m128i* const q1,
                                    int thresh, __m128i* const mask) {
  const __m128i m_thresh = _mm_set1_epi8(thresh);
  const __m128i t1 = MM_ABS(*p1, *q1);        
  const __m128i kFE = _mm_set1_epi8(0xFE);
  const __m128i t2 = _mm_and_si128(t1, kFE);  
  const __m128i t3 = _mm_srli_epi16(t2, 1);   

  const __m128i t4 = MM_ABS(*p0, *q0);        
  const __m128i t5 = _mm_adds_epu8(t4, t4);   
  const __m128i t6 = _mm_adds_epu8(t5, t3);   

  const __m128i t7 = _mm_subs_epu8(t6, m_thresh);  
  *mask = _mm_cmpeq_epi8(t7, _mm_setzero_si128());
}


static WEBP_INLINE void DoFilter2(__m128i* const p1, __m128i* const p0,
                                  __m128i* const q0, __m128i* const q1,
                                  int thresh) {
  __m128i a, mask;
  const __m128i sign_bit = _mm_set1_epi8(0x80);
  
  const __m128i p1s = _mm_xor_si128(*p1, sign_bit);
  const __m128i q1s = _mm_xor_si128(*q1, sign_bit);

  NeedsFilter(p1, p0, q0, q1, thresh, &mask);

  FLIP_SIGN_BIT2(*p0, *q0);
  GetBaseDelta(&p1s, p0, q0, &q1s, &a);
  a = _mm_and_si128(a, mask);     
  DoSimpleFilter(p0, q0, &a);
  FLIP_SIGN_BIT2(*p0, *q0);
}

static WEBP_INLINE void DoFilter4(__m128i* const p1, __m128i* const p0,
                                  __m128i* const q0, __m128i* const q1,
                                  const __m128i* const mask, int hev_thresh) {
  const __m128i sign_bit = _mm_set1_epi8(0x80);
  const __m128i k64 = _mm_set1_epi8(0x40);
  const __m128i zero = _mm_setzero_si128();
  __m128i not_hev;
  __m128i t1, t2, t3;

  
  GetNotHEV(p1, p0, q0, q1, hev_thresh, &not_hev);

  
  FLIP_SIGN_BIT4(*p1, *p0, *q0, *q1);

  t1 = _mm_subs_epi8(*p1, *q1);        
  t1 = _mm_andnot_si128(not_hev, t1);  
  t2 = _mm_subs_epi8(*q0, *p0);        
  t1 = _mm_adds_epi8(t1, t2);          
  t1 = _mm_adds_epi8(t1, t2);          
  t1 = _mm_adds_epi8(t1, t2);          
  t1 = _mm_and_si128(t1, *mask);       

  t2 = _mm_set1_epi8(3);
  t3 = _mm_set1_epi8(4);
  t2 = _mm_adds_epi8(t1, t2);        
  t3 = _mm_adds_epi8(t1, t3);        
  SignedShift8b(&t2);                
  SignedShift8b(&t3);                
  *p0 = _mm_adds_epi8(*p0, t2);      
  *q0 = _mm_subs_epi8(*q0, t3);      
  FLIP_SIGN_BIT2(*p0, *q0);

  
  t2 = _mm_add_epi8(t3, sign_bit);
  t3 = _mm_avg_epu8(t2, zero);
  t3 = _mm_sub_epi8(t3, k64);

  t3 = _mm_and_si128(not_hev, t3);   
  *q1 = _mm_subs_epi8(*q1, t3);      
  *p1 = _mm_adds_epi8(*p1, t3);      
  FLIP_SIGN_BIT2(*p1, *q1);
}

static WEBP_INLINE void DoFilter6(__m128i* const p2, __m128i* const p1,
                                  __m128i* const p0, __m128i* const q0,
                                  __m128i* const q1, __m128i* const q2,
                                  const __m128i* const mask, int hev_thresh) {
  const __m128i zero = _mm_setzero_si128();
  const __m128i sign_bit = _mm_set1_epi8(0x80);
  __m128i a, not_hev;

  
  GetNotHEV(p1, p0, q0, q1, hev_thresh, &not_hev);

  FLIP_SIGN_BIT4(*p1, *p0, *q0, *q1);
  FLIP_SIGN_BIT2(*p2, *q2);
  GetBaseDelta(p1, p0, q0, q1, &a);

  { 
    const __m128i m = _mm_andnot_si128(not_hev, *mask);
    const __m128i f = _mm_and_si128(a, m);
    DoSimpleFilter(p0, q0, &f);
  }

  { 
    const __m128i k9 = _mm_set1_epi16(0x0900);
    const __m128i k63 = _mm_set1_epi16(63);

    const __m128i m = _mm_and_si128(not_hev, *mask);
    const __m128i f = _mm_and_si128(a, m);

    const __m128i f_lo = _mm_unpacklo_epi8(zero, f);
    const __m128i f_hi = _mm_unpackhi_epi8(zero, f);

    const __m128i f9_lo = _mm_mulhi_epi16(f_lo, k9);    
    const __m128i f9_hi = _mm_mulhi_epi16(f_hi, k9);    

    const __m128i a2_lo = _mm_add_epi16(f9_lo, k63);    
    const __m128i a2_hi = _mm_add_epi16(f9_hi, k63);    

    const __m128i a1_lo = _mm_add_epi16(a2_lo, f9_lo);  
    const __m128i a1_hi = _mm_add_epi16(a2_hi, f9_hi);  

    const __m128i a0_lo = _mm_add_epi16(a1_lo, f9_lo);  
    const __m128i a0_hi = _mm_add_epi16(a1_hi, f9_hi);  

    Update2Pixels(p2, q2, &a2_lo, &a2_hi);
    Update2Pixels(p1, q1, &a1_lo, &a1_hi);
    Update2Pixels(p0, q0, &a0_lo, &a0_hi);
  }
}

static WEBP_INLINE void Load8x4(const uint8_t* const b, int stride,
                                __m128i* const p, __m128i* const q) {
  __m128i t1, t2;

  
  __m128i r0 =  _mm_cvtsi32_si128(*((int*)&b[0 * stride]));  
  __m128i r1 =  _mm_cvtsi32_si128(*((int*)&b[1 * stride]));  
  __m128i r4 =  _mm_cvtsi32_si128(*((int*)&b[4 * stride]));  
  __m128i r5 =  _mm_cvtsi32_si128(*((int*)&b[5 * stride]));  

  r0 = _mm_unpacklo_epi32(r0, r4);               
  r1 = _mm_unpacklo_epi32(r1, r5);               

  
  t1 = _mm_unpacklo_epi8(r0, r1);

  
  r0 =  _mm_cvtsi32_si128(*((int*)&b[2 * stride]));          
  r1 =  _mm_cvtsi32_si128(*((int*)&b[3 * stride]));          
  r4 =  _mm_cvtsi32_si128(*((int*)&b[6 * stride]));          
  r5 =  _mm_cvtsi32_si128(*((int*)&b[7 * stride]));          

  r0 = _mm_unpacklo_epi32(r0, r4);               
  r1 = _mm_unpacklo_epi32(r1, r5);               

  
  t2 = _mm_unpacklo_epi8(r0, r1);

  
  
  r0 = t1;
  t1 = _mm_unpacklo_epi16(t1, t2);
  t2 = _mm_unpackhi_epi16(r0, t2);

  
  
  *p = _mm_unpacklo_epi32(t1, t2);
  *q = _mm_unpackhi_epi32(t1, t2);
}

static WEBP_INLINE void Load16x4(const uint8_t* const r0,
                                 const uint8_t* const r8,
                                 int stride,
                                 __m128i* const p1, __m128i* const p0,
                                 __m128i* const q0, __m128i* const q1) {
  __m128i t1, t2;
  
  
  
  
  
  
  
  
  

  
  
  
  
  
  Load8x4(r0, stride, p1, q0);
  Load8x4(r8, stride, p0, q1);

  t1 = *p1;
  t2 = *q0;
  
  
  
  
  *p1 = _mm_unpacklo_epi64(t1, *p0);
  *p0 = _mm_unpackhi_epi64(t1, *p0);
  *q0 = _mm_unpacklo_epi64(t2, *q1);
  *q1 = _mm_unpackhi_epi64(t2, *q1);
}

static WEBP_INLINE void Store4x4(__m128i* const x, uint8_t* dst, int stride) {
  int i;
  for (i = 0; i < 4; ++i, dst += stride) {
    *((int32_t*)dst) = _mm_cvtsi128_si32(*x);
    *x = _mm_srli_si128(*x, 4);
  }
}

static WEBP_INLINE void Store16x4(const __m128i* const p1,
                                  const __m128i* const p0,
                                  const __m128i* const q0,
                                  const __m128i* const q1,
                                  uint8_t* r0, uint8_t* r8,
                                  int stride) {
  __m128i t1, p1_s, p0_s, q0_s, q1_s;

  
  
  t1 = *p0;
  p0_s = _mm_unpacklo_epi8(*p1, t1);
  p1_s = _mm_unpackhi_epi8(*p1, t1);

  
  
  t1 = *q0;
  q0_s = _mm_unpacklo_epi8(t1, *q1);
  q1_s = _mm_unpackhi_epi8(t1, *q1);

  
  
  t1 = p0_s;
  p0_s = _mm_unpacklo_epi16(t1, q0_s);
  q0_s = _mm_unpackhi_epi16(t1, q0_s);

  
  
  t1 = p1_s;
  p1_s = _mm_unpacklo_epi16(t1, q1_s);
  q1_s = _mm_unpackhi_epi16(t1, q1_s);

  Store4x4(&p0_s, r0, stride);
  r0 += 4 * stride;
  Store4x4(&q0_s, r0, stride);

  Store4x4(&p1_s, r8, stride);
  r8 += 4 * stride;
  Store4x4(&q1_s, r8, stride);
}


static void SimpleVFilter16(uint8_t* p, int stride, int thresh) {
  
  __m128i p1 = _mm_loadu_si128((__m128i*)&p[-2 * stride]);
  __m128i p0 = _mm_loadu_si128((__m128i*)&p[-stride]);
  __m128i q0 = _mm_loadu_si128((__m128i*)&p[0]);
  __m128i q1 = _mm_loadu_si128((__m128i*)&p[stride]);

  DoFilter2(&p1, &p0, &q0, &q1, thresh);

  
  _mm_storeu_si128((__m128i*)&p[-stride], p0);
  _mm_storeu_si128((__m128i*)&p[0], q0);
}

static void SimpleHFilter16(uint8_t* p, int stride, int thresh) {
  __m128i p1, p0, q0, q1;

  p -= 2;  

  Load16x4(p, p + 8 * stride, stride, &p1, &p0, &q0, &q1);
  DoFilter2(&p1, &p0, &q0, &q1, thresh);
  Store16x4(&p1, &p0, &q0, &q1, p, p + 8 * stride, stride);
}

static void SimpleVFilter16i(uint8_t* p, int stride, int thresh) {
  int k;
  for (k = 3; k > 0; --k) {
    p += 4 * stride;
    SimpleVFilter16(p, stride, thresh);
  }
}

static void SimpleHFilter16i(uint8_t* p, int stride, int thresh) {
  int k;
  for (k = 3; k > 0; --k) {
    p += 4;
    SimpleHFilter16(p, stride, thresh);
  }
}


#define MAX_DIFF1(p3, p2, p1, p0, m) do {                                      \
  m = MM_ABS(p1, p0);                                                          \
  m = _mm_max_epu8(m, MM_ABS(p3, p2));                                         \
  m = _mm_max_epu8(m, MM_ABS(p2, p1));                                         \
} while (0)

#define MAX_DIFF2(p3, p2, p1, p0, m) do {                                      \
  m = _mm_max_epu8(m, MM_ABS(p1, p0));                                         \
  m = _mm_max_epu8(m, MM_ABS(p3, p2));                                         \
  m = _mm_max_epu8(m, MM_ABS(p2, p1));                                         \
} while (0)

#define LOAD_H_EDGES4(p, stride, e1, e2, e3, e4) {                             \
  e1 = _mm_loadu_si128((__m128i*)&(p)[0 * stride]);                            \
  e2 = _mm_loadu_si128((__m128i*)&(p)[1 * stride]);                            \
  e3 = _mm_loadu_si128((__m128i*)&(p)[2 * stride]);                            \
  e4 = _mm_loadu_si128((__m128i*)&(p)[3 * stride]);                            \
}

#define LOADUV_H_EDGE(p, u, v, stride) do {                                    \
  const __m128i U = _mm_loadl_epi64((__m128i*)&(u)[(stride)]);                 \
  const __m128i V = _mm_loadl_epi64((__m128i*)&(v)[(stride)]);                 \
  p = _mm_unpacklo_epi64(U, V);                                                \
} while (0)

#define LOADUV_H_EDGES4(u, v, stride, e1, e2, e3, e4) {                        \
  LOADUV_H_EDGE(e1, u, v, 0 * stride);                                         \
  LOADUV_H_EDGE(e2, u, v, 1 * stride);                                         \
  LOADUV_H_EDGE(e3, u, v, 2 * stride);                                         \
  LOADUV_H_EDGE(e4, u, v, 3 * stride);                                         \
}

#define STOREUV(p, u, v, stride) {                                             \
  _mm_storel_epi64((__m128i*)&u[(stride)], p);                                 \
  p = _mm_srli_si128(p, 8);                                                    \
  _mm_storel_epi64((__m128i*)&v[(stride)], p);                                 \
}

static WEBP_INLINE void ComplexMask(const __m128i* const p1,
                                    const __m128i* const p0,
                                    const __m128i* const q0,
                                    const __m128i* const q1,
                                    int thresh, int ithresh,
                                    __m128i* const mask) {
  const __m128i it = _mm_set1_epi8(ithresh);
  const __m128i diff = _mm_subs_epu8(*mask, it);
  const __m128i thresh_mask = _mm_cmpeq_epi8(diff, _mm_setzero_si128());
  __m128i filter_mask;
  NeedsFilter(p1, p0, q0, q1, thresh, &filter_mask);
  *mask = _mm_and_si128(thresh_mask, filter_mask);
}

static void VFilter16(uint8_t* p, int stride,
                      int thresh, int ithresh, int hev_thresh) {
  __m128i t1;
  __m128i mask;
  __m128i p2, p1, p0, q0, q1, q2;

  
  LOAD_H_EDGES4(p - 4 * stride, stride, t1, p2, p1, p0);
  MAX_DIFF1(t1, p2, p1, p0, mask);

  
  LOAD_H_EDGES4(p, stride, q0, q1, q2, t1);
  MAX_DIFF2(t1, q2, q1, q0, mask);

  ComplexMask(&p1, &p0, &q0, &q1, thresh, ithresh, &mask);
  DoFilter6(&p2, &p1, &p0, &q0, &q1, &q2, &mask, hev_thresh);

  
  _mm_storeu_si128((__m128i*)&p[-3 * stride], p2);
  _mm_storeu_si128((__m128i*)&p[-2 * stride], p1);
  _mm_storeu_si128((__m128i*)&p[-1 * stride], p0);
  _mm_storeu_si128((__m128i*)&p[+0 * stride], q0);
  _mm_storeu_si128((__m128i*)&p[+1 * stride], q1);
  _mm_storeu_si128((__m128i*)&p[+2 * stride], q2);
}

static void HFilter16(uint8_t* p, int stride,
                      int thresh, int ithresh, int hev_thresh) {
  __m128i mask;
  __m128i p3, p2, p1, p0, q0, q1, q2, q3;

  uint8_t* const b = p - 4;
  Load16x4(b, b + 8 * stride, stride, &p3, &p2, &p1, &p0);  
  MAX_DIFF1(p3, p2, p1, p0, mask);

  Load16x4(p, p + 8 * stride, stride, &q0, &q1, &q2, &q3);  
  MAX_DIFF2(q3, q2, q1, q0, mask);

  ComplexMask(&p1, &p0, &q0, &q1, thresh, ithresh, &mask);
  DoFilter6(&p2, &p1, &p0, &q0, &q1, &q2, &mask, hev_thresh);

  Store16x4(&p3, &p2, &p1, &p0, b, b + 8 * stride, stride);
  Store16x4(&q0, &q1, &q2, &q3, p, p + 8 * stride, stride);
}

static void VFilter16i(uint8_t* p, int stride,
                       int thresh, int ithresh, int hev_thresh) {
  int k;
  __m128i p3, p2, p1, p0;   

  LOAD_H_EDGES4(p, stride, p3, p2, p1, p0);  

  for (k = 3; k > 0; --k) {
    __m128i mask, tmp1, tmp2;
    uint8_t* const b = p + 2 * stride;   
    p += 4 * stride;

    MAX_DIFF1(p3, p2, p1, p0, mask);   
    LOAD_H_EDGES4(p, stride, p3, p2, tmp1, tmp2);
    MAX_DIFF2(p3, p2, tmp1, tmp2, mask);

    
    
    ComplexMask(&p1, &p0, &p3, &p2, thresh, ithresh, &mask);
    DoFilter4(&p1, &p0, &p3, &p2, &mask, hev_thresh);

    
    _mm_storeu_si128((__m128i*)&b[0 * stride], p1);
    _mm_storeu_si128((__m128i*)&b[1 * stride], p0);
    _mm_storeu_si128((__m128i*)&b[2 * stride], p3);
    _mm_storeu_si128((__m128i*)&b[3 * stride], p2);

    
    p1 = tmp1;
    p0 = tmp2;
  }
}

static void HFilter16i(uint8_t* p, int stride,
                       int thresh, int ithresh, int hev_thresh) {
  int k;
  __m128i p3, p2, p1, p0;   

  Load16x4(p, p + 8 * stride, stride, &p3, &p2, &p1, &p0);  

  for (k = 3; k > 0; --k) {
    __m128i mask, tmp1, tmp2;
    uint8_t* const b = p + 2;   

    p += 4;  

    MAX_DIFF1(p3, p2, p1, p0, mask);   
    Load16x4(p, p + 8 * stride, stride, &p3, &p2, &tmp1, &tmp2);
    MAX_DIFF2(p3, p2, tmp1, tmp2, mask);

    ComplexMask(&p1, &p0, &p3, &p2, thresh, ithresh, &mask);
    DoFilter4(&p1, &p0, &p3, &p2, &mask, hev_thresh);

    Store16x4(&p1, &p0, &p3, &p2, b, b + 8 * stride, stride);

    
    p1 = tmp1;
    p0 = tmp2;
  }
}

static void VFilter8(uint8_t* u, uint8_t* v, int stride,
                     int thresh, int ithresh, int hev_thresh) {
  __m128i mask;
  __m128i t1, p2, p1, p0, q0, q1, q2;

  
  LOADUV_H_EDGES4(u - 4 * stride, v - 4 * stride, stride, t1, p2, p1, p0);
  MAX_DIFF1(t1, p2, p1, p0, mask);

  
  LOADUV_H_EDGES4(u, v, stride, q0, q1, q2, t1);
  MAX_DIFF2(t1, q2, q1, q0, mask);

  ComplexMask(&p1, &p0, &q0, &q1, thresh, ithresh, &mask);
  DoFilter6(&p2, &p1, &p0, &q0, &q1, &q2, &mask, hev_thresh);

  
  STOREUV(p2, u, v, -3 * stride);
  STOREUV(p1, u, v, -2 * stride);
  STOREUV(p0, u, v, -1 * stride);
  STOREUV(q0, u, v, 0 * stride);
  STOREUV(q1, u, v, 1 * stride);
  STOREUV(q2, u, v, 2 * stride);
}

static void HFilter8(uint8_t* u, uint8_t* v, int stride,
                     int thresh, int ithresh, int hev_thresh) {
  __m128i mask;
  __m128i p3, p2, p1, p0, q0, q1, q2, q3;

  uint8_t* const tu = u - 4;
  uint8_t* const tv = v - 4;
  Load16x4(tu, tv, stride, &p3, &p2, &p1, &p0);  
  MAX_DIFF1(p3, p2, p1, p0, mask);

  Load16x4(u, v, stride, &q0, &q1, &q2, &q3);    
  MAX_DIFF2(q3, q2, q1, q0, mask);

  ComplexMask(&p1, &p0, &q0, &q1, thresh, ithresh, &mask);
  DoFilter6(&p2, &p1, &p0, &q0, &q1, &q2, &mask, hev_thresh);

  Store16x4(&p3, &p2, &p1, &p0, tu, tv, stride);
  Store16x4(&q0, &q1, &q2, &q3, u, v, stride);
}

static void VFilter8i(uint8_t* u, uint8_t* v, int stride,
                      int thresh, int ithresh, int hev_thresh) {
  __m128i mask;
  __m128i t1, t2, p1, p0, q0, q1;

  
  LOADUV_H_EDGES4(u, v, stride, t2, t1, p1, p0);
  MAX_DIFF1(t2, t1, p1, p0, mask);

  u += 4 * stride;
  v += 4 * stride;

  
  LOADUV_H_EDGES4(u, v, stride, q0, q1, t1, t2);
  MAX_DIFF2(t2, t1, q1, q0, mask);

  ComplexMask(&p1, &p0, &q0, &q1, thresh, ithresh, &mask);
  DoFilter4(&p1, &p0, &q0, &q1, &mask, hev_thresh);

  
  STOREUV(p1, u, v, -2 * stride);
  STOREUV(p0, u, v, -1 * stride);
  STOREUV(q0, u, v, 0 * stride);
  STOREUV(q1, u, v, 1 * stride);
}

static void HFilter8i(uint8_t* u, uint8_t* v, int stride,
                      int thresh, int ithresh, int hev_thresh) {
  __m128i mask;
  __m128i t1, t2, p1, p0, q0, q1;
  Load16x4(u, v, stride, &t2, &t1, &p1, &p0);   
  MAX_DIFF1(t2, t1, p1, p0, mask);

  u += 4;  
  v += 4;
  Load16x4(u, v, stride, &q0, &q1, &t1, &t2);  
  MAX_DIFF2(t2, t1, q1, q0, mask);

  ComplexMask(&p1, &p0, &q0, &q1, thresh, ithresh, &mask);
  DoFilter4(&p1, &p0, &q0, &q1, &mask, hev_thresh);

  u -= 2;  
  v -= 2;
  Store16x4(&p1, &p0, &q0, &q1, u, v, stride);
}

#endif   


extern void VP8DspInitSSE2(void);

void VP8DspInitSSE2(void) {
#if defined(WEBP_USE_SSE2)
  VP8Transform = Transform;
#if defined(USE_TRANSFORM_AC3)
  VP8TransformAC3 = TransformAC3;
#endif

  VP8VFilter16 = VFilter16;
  VP8HFilter16 = HFilter16;
  VP8VFilter8 = VFilter8;
  VP8HFilter8 = HFilter8;
  VP8VFilter16i = VFilter16i;
  VP8HFilter16i = HFilter16i;
  VP8VFilter8i = VFilter8i;
  VP8HFilter8i = HFilter8i;

  VP8SimpleVFilter16 = SimpleVFilter16;
  VP8SimpleHFilter16 = SimpleHFilter16;
  VP8SimpleVFilter16i = SimpleVFilter16i;
  VP8SimpleHFilter16i = SimpleHFilter16i;
#endif   
}