/*
 * dirhash.c -- Calculate the hash of a directory entry
 *
 * Copyright (c) 2001  Daniel Phillips
 *
 * Copyright (c) 2002 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#include <stdio.h>
#include <string.h>

#include "ext2_fs.h"
#include "ext2fs.h"

/*
 * Keyed 32-bit hash function using TEA in a Davis-Meyer function
 *   H0 = Key
 *   Hi = E Mi(Hi-1) + Hi-1
 *
 * (see Applied Cryptography, 2nd edition, p448).
 *
 * Jeremy Fitzhardinge <jeremy@zip.com.au> 1998
 *
 * This code is made available under the terms of the GPL
 */
#define DELTA 0x9E3779B9

static void TEA_transform(__u32 buf[4], __u32 const in[])
{
	__u32	sum = 0;
	__u32	b0 = buf[0], b1 = buf[1];
	__u32	a = in[0], b = in[1], c = in[2], d = in[3];
	int	n = 16;

	do {
		sum += DELTA;
		b0 += ((b1 << 4)+a) ^ (b1+sum) ^ ((b1 >> 5)+b);
		b1 += ((b0 << 4)+c) ^ (b0+sum) ^ ((b0 >> 5)+d);
	} while(--n);

	buf[0] += b0;
	buf[1] += b1;
}

#define F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z) (((x) & (y)) + (((x) ^ (y)) & (z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))

#define ROUND(f, a, b, c, d, x, s)	\
	(a += f(b, c, d) + x, a = (a << s) | (a >> (32-s)))
#define K1 0
#define K2 013240474631UL
#define K3 015666365641UL

static void halfMD4Transform (__u32 buf[4], __u32 const in[])
{
	__u32	a = buf[0], b = buf[1], c = buf[2], d = buf[3];

	
	ROUND(F, a, b, c, d, in[0] + K1,  3);
	ROUND(F, d, a, b, c, in[1] + K1,  7);
	ROUND(F, c, d, a, b, in[2] + K1, 11);
	ROUND(F, b, c, d, a, in[3] + K1, 19);
	ROUND(F, a, b, c, d, in[4] + K1,  3);
	ROUND(F, d, a, b, c, in[5] + K1,  7);
	ROUND(F, c, d, a, b, in[6] + K1, 11);
	ROUND(F, b, c, d, a, in[7] + K1, 19);

	
	ROUND(G, a, b, c, d, in[1] + K2,  3);
	ROUND(G, d, a, b, c, in[3] + K2,  5);
	ROUND(G, c, d, a, b, in[5] + K2,  9);
	ROUND(G, b, c, d, a, in[7] + K2, 13);
	ROUND(G, a, b, c, d, in[0] + K2,  3);
	ROUND(G, d, a, b, c, in[2] + K2,  5);
	ROUND(G, c, d, a, b, in[4] + K2,  9);
	ROUND(G, b, c, d, a, in[6] + K2, 13);

	
	ROUND(H, a, b, c, d, in[3] + K3,  3);
	ROUND(H, d, a, b, c, in[7] + K3,  9);
	ROUND(H, c, d, a, b, in[2] + K3, 11);
	ROUND(H, b, c, d, a, in[6] + K3, 15);
	ROUND(H, a, b, c, d, in[1] + K3,  3);
	ROUND(H, d, a, b, c, in[5] + K3,  9);
	ROUND(H, c, d, a, b, in[0] + K3, 11);
	ROUND(H, b, c, d, a, in[4] + K3, 15);

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}

#undef ROUND
#undef F
#undef G
#undef H
#undef K1
#undef K2
#undef K3

static ext2_dirhash_t dx_hack_hash (const char *name, int len,
				    int unsigned_flag)
{
	__u32 hash, hash0 = 0x12a3fe2d, hash1 = 0x37abe8f9;
	const unsigned char *ucp = (const unsigned char *) name;
	const signed char *scp = (const signed char *) name;
	int c;

	while (len--) {
		if (unsigned_flag)
			c = (int) *ucp++;
		else
			c = (int) *scp++;
		hash = hash1 + (hash0 ^ (c * 7152373));

		if (hash & 0x80000000) hash -= 0x7fffffff;
		hash1 = hash0;
		hash0 = hash;
	}
	return (hash0 << 1);
}

static void str2hashbuf(const char *msg, int len, __u32 *buf, int num,
			int unsigned_flag)
{
	__u32	pad, val;
	int	i, c;
	const unsigned char *ucp = (const unsigned char *) msg;
	const signed char *scp = (const signed char *) msg;

	pad = (__u32)len | ((__u32)len << 8);
	pad |= pad << 16;

	val = pad;
	if (len > num*4)
		len = num * 4;
	for (i=0; i < len; i++) {
		if ((i % 4) == 0)
			val = pad;
		if (unsigned_flag)
			c = (int) ucp[i];
		else
			c = (int) scp[i];

		val = c + (val << 8);
		if ((i % 4) == 3) {
			*buf++ = val;
			val = pad;
			num--;
		}
	}
	if (--num >= 0)
		*buf++ = val;
	while (--num >= 0)
		*buf++ = pad;
}

errcode_t ext2fs_dirhash(int version, const char *name, int len,
			 const __u32 *seed,
			 ext2_dirhash_t *ret_hash,
			 ext2_dirhash_t *ret_minor_hash)
{
	__u32	hash;
	__u32	minor_hash = 0;
	const char	*p;
	int		i;
	__u32 		in[8], buf[4];
	int		unsigned_flag = 0;

	
	buf[0] = 0x67452301;
	buf[1] = 0xefcdab89;
	buf[2] = 0x98badcfe;
	buf[3] = 0x10325476;

	
	if (seed) {
		for (i=0; i < 4; i++) {
			if (seed[i])
				break;
		}
		if (i < 4)
			memcpy(buf, seed, sizeof(buf));
	}

	switch (version) {
	case EXT2_HASH_LEGACY_UNSIGNED:
		unsigned_flag++;
		
	case EXT2_HASH_LEGACY:
		hash = dx_hack_hash(name, len, unsigned_flag);
		break;
	case EXT2_HASH_HALF_MD4_UNSIGNED:
		unsigned_flag++;
		
	case EXT2_HASH_HALF_MD4:
		p = name;
		while (len > 0) {
			str2hashbuf(p, len, in, 8, unsigned_flag);
			halfMD4Transform(buf, in);
			len -= 32;
			p += 32;
		}
		minor_hash = buf[2];
		hash = buf[1];
		break;
	case EXT2_HASH_TEA_UNSIGNED:
		unsigned_flag++;
		
	case EXT2_HASH_TEA:
		p = name;
		while (len > 0) {
			str2hashbuf(p, len, in, 4, unsigned_flag);
			TEA_transform(buf, in);
			len -= 16;
			p += 16;
		}
		hash = buf[0];
		minor_hash = buf[1];
		break;
	default:
		*ret_hash = 0;
		return EXT2_ET_DIRHASH_UNSUPP;
	}
	*ret_hash = hash & ~1;
	if (ret_minor_hash)
		*ret_minor_hash = minor_hash;
	return 0;
}