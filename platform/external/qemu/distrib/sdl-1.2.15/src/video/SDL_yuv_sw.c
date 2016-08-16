/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2012 Sam Lantinga

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
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/
#include "SDL_config.h"


/* This code was derived from code carrying the following copyright notices:

 * Copyright (c) 1995 The Regents of the University of California.
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

 * Copyright (c) 1995 Erik Corry
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 * 
 * IN NO EVENT SHALL ERIK CORRY BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
 * SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF
 * THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF ERIK CORRY HAS BEEN ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * ERIK CORRY SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS"
 * BASIS, AND ERIK CORRY HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT,
 * UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

 * Portions of this software Copyright (c) 1995 Brown University.
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement
 * is hereby granted, provided that the above copyright notice and the
 * following two paragraphs appear in all copies of this software.
 * 
 * IN NO EVENT SHALL BROWN UNIVERSITY BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF BROWN
 * UNIVERSITY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * BROWN UNIVERSITY SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS"
 * BASIS, AND BROWN UNIVERSITY HAS NO OBLIGATION TO PROVIDE MAINTENANCE,
 * SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

#include "SDL_video.h"
#include "SDL_cpuinfo.h"
#include "SDL_stretch_c.h"
#include "SDL_yuvfuncs.h"
#include "SDL_yuv_sw_c.h"

static struct private_yuvhwfuncs sw_yuvfuncs = {
	SDL_LockYUV_SW,
	SDL_UnlockYUV_SW,
	SDL_DisplayYUV_SW,
	SDL_FreeYUV_SW
};

struct private_yuvhwdata {
	SDL_Surface *stretch;
	SDL_Surface *display;
	Uint8 *pixels;
	int *colortab;
	Uint32 *rgb_2_pix;
	void (*Display1X)(int *colortab, Uint32 *rgb_2_pix,
                          unsigned char *lum, unsigned char *cr,
                          unsigned char *cb, unsigned char *out,
                          int rows, int cols, int mod );
	void (*Display2X)(int *colortab, Uint32 *rgb_2_pix,
	                  unsigned char *lum, unsigned char *cr,
                          unsigned char *cb, unsigned char *out,
                          int rows, int cols, int mod );

	
	Uint16 pitches[3];
	Uint8 *planes[3];
};



#if (__GNUC__ > 2) && defined(__i386__) && __OPTIMIZE__ && SDL_ASSEMBLY_ROUTINES
extern void Color565DitherYV12MMX1X( int *colortab, Uint32 *rgb_2_pix,
                                     unsigned char *lum, unsigned char *cr,
                                     unsigned char *cb, unsigned char *out,
                                     int rows, int cols, int mod );
extern void ColorRGBDitherYV12MMX1X( int *colortab, Uint32 *rgb_2_pix,
                                     unsigned char *lum, unsigned char *cr,
                                     unsigned char *cb, unsigned char *out,
                                     int rows, int cols, int mod );
#endif 

static void Color16DitherYV12Mod1X( int *colortab, Uint32 *rgb_2_pix,
                                    unsigned char *lum, unsigned char *cr,
                                    unsigned char *cb, unsigned char *out,
                                    int rows, int cols, int mod )
{
    unsigned short* row1;
    unsigned short* row2;
    unsigned char* lum2;
    int x, y;
    int cr_r;
    int crb_g;
    int cb_b;
    int cols_2 = cols / 2;

    row1 = (unsigned short*) out;
    row2 = row1 + cols + mod;
    lum2 = lum + cols;

    mod += cols + mod;

    y = rows / 2;
    while( y-- )
    {
        x = cols_2;
        while( x-- )
        {
            register int L;

            cr_r   = 0*768+256 + colortab[ *cr + 0*256 ];
            crb_g  = 1*768+256 + colortab[ *cr + 1*256 ]
                               + colortab[ *cb + 2*256 ];
            cb_b   = 2*768+256 + colortab[ *cb + 3*256 ];
            ++cr; ++cb;

            L = *lum++;
            *row1++ = (unsigned short)(rgb_2_pix[ L + cr_r ] |
                                       rgb_2_pix[ L + crb_g ] |
                                       rgb_2_pix[ L + cb_b ]);

            L = *lum++;
            *row1++ = (unsigned short)(rgb_2_pix[ L + cr_r ] |
                                       rgb_2_pix[ L + crb_g ] |
                                       rgb_2_pix[ L + cb_b ]);


            

            L = *lum2++;
            *row2++ = (unsigned short)(rgb_2_pix[ L + cr_r ] |
                                       rgb_2_pix[ L + crb_g ] |
                                       rgb_2_pix[ L + cb_b ]);

            L = *lum2++;
            *row2++ = (unsigned short)(rgb_2_pix[ L + cr_r ] |
                                       rgb_2_pix[ L + crb_g ] |
                                       rgb_2_pix[ L + cb_b ]);
        }

        lum  += cols;
        lum2 += cols;
        row1 += mod;
        row2 += mod;
    }
}

static void Color24DitherYV12Mod1X( int *colortab, Uint32 *rgb_2_pix,
                                    unsigned char *lum, unsigned char *cr,
                                    unsigned char *cb, unsigned char *out,
                                    int rows, int cols, int mod )
{
    unsigned int value;
    unsigned char* row1;
    unsigned char* row2;
    unsigned char* lum2;
    int x, y;
    int cr_r;
    int crb_g;
    int cb_b;
    int cols_2 = cols / 2;

    row1 = out;
    row2 = row1 + cols*3 + mod*3;
    lum2 = lum + cols;

    mod += cols + mod;
    mod *= 3;

    y = rows / 2;
    while( y-- )
    {
        x = cols_2;
        while( x-- )
        {
            register int L;

            cr_r   = 0*768+256 + colortab[ *cr + 0*256 ];
            crb_g  = 1*768+256 + colortab[ *cr + 1*256 ]
                               + colortab[ *cb + 2*256 ];
            cb_b   = 2*768+256 + colortab[ *cb + 3*256 ];
            ++cr; ++cb;

            L = *lum++;
            value = (rgb_2_pix[ L + cr_r ] |
                     rgb_2_pix[ L + crb_g ] |
                     rgb_2_pix[ L + cb_b ]);
            *row1++ = (value      ) & 0xFF;
            *row1++ = (value >>  8) & 0xFF;
            *row1++ = (value >> 16) & 0xFF;

            L = *lum++;
            value = (rgb_2_pix[ L + cr_r ] |
                     rgb_2_pix[ L + crb_g ] |
                     rgb_2_pix[ L + cb_b ]);
            *row1++ = (value      ) & 0xFF;
            *row1++ = (value >>  8) & 0xFF;
            *row1++ = (value >> 16) & 0xFF;


            

            L = *lum2++;
            value = (rgb_2_pix[ L + cr_r ] |
                     rgb_2_pix[ L + crb_g ] |
                     rgb_2_pix[ L + cb_b ]);
            *row2++ = (value      ) & 0xFF;
            *row2++ = (value >>  8) & 0xFF;
            *row2++ = (value >> 16) & 0xFF;

            L = *lum2++;
            value = (rgb_2_pix[ L + cr_r ] |
                     rgb_2_pix[ L + crb_g ] |
                     rgb_2_pix[ L + cb_b ]);
            *row2++ = (value      ) & 0xFF;
            *row2++ = (value >>  8) & 0xFF;
            *row2++ = (value >> 16) & 0xFF;
        }

        lum  += cols;
        lum2 += cols;
        row1 += mod;
        row2 += mod;
    }
}

static void Color32DitherYV12Mod1X( int *colortab, Uint32 *rgb_2_pix,
                                    unsigned char *lum, unsigned char *cr,
                                    unsigned char *cb, unsigned char *out,
                                    int rows, int cols, int mod )
{
    unsigned int* row1;
    unsigned int* row2;
    unsigned char* lum2;
    int x, y;
    int cr_r;
    int crb_g;
    int cb_b;
    int cols_2 = cols / 2;

    row1 = (unsigned int*) out;
    row2 = row1 + cols + mod;
    lum2 = lum + cols;

    mod += cols + mod;

    y = rows / 2;
    while( y-- )
    {
        x = cols_2;
        while( x-- )
        {
            register int L;

            cr_r   = 0*768+256 + colortab[ *cr + 0*256 ];
            crb_g  = 1*768+256 + colortab[ *cr + 1*256 ]
                               + colortab[ *cb + 2*256 ];
            cb_b   = 2*768+256 + colortab[ *cb + 3*256 ];
            ++cr; ++cb;

            L = *lum++;
            *row1++ = (rgb_2_pix[ L + cr_r ] |
                       rgb_2_pix[ L + crb_g ] |
                       rgb_2_pix[ L + cb_b ]);

            L = *lum++;
            *row1++ = (rgb_2_pix[ L + cr_r ] |
                       rgb_2_pix[ L + crb_g ] |
                       rgb_2_pix[ L + cb_b ]);


            

            L = *lum2++;
            *row2++ = (rgb_2_pix[ L + cr_r ] |
                       rgb_2_pix[ L + crb_g ] |
                       rgb_2_pix[ L + cb_b ]);

            L = *lum2++;
            *row2++ = (rgb_2_pix[ L + cr_r ] |
                       rgb_2_pix[ L + crb_g ] |
                       rgb_2_pix[ L + cb_b ]);
        }

        lum  += cols;
        lum2 += cols;
        row1 += mod;
        row2 += mod;
    }
}

static void Color16DitherYV12Mod2X( int *colortab, Uint32 *rgb_2_pix,
                                    unsigned char *lum, unsigned char *cr,
                                    unsigned char *cb, unsigned char *out,
                                    int rows, int cols, int mod )
{
    unsigned int* row1 = (unsigned int*) out;
    const int next_row = cols+(mod/2);
    unsigned int* row2 = row1 + 2*next_row;
    unsigned char* lum2;
    int x, y;
    int cr_r;
    int crb_g;
    int cb_b;
    int cols_2 = cols / 2;

    lum2 = lum + cols;

    mod = (next_row * 3) + (mod/2);

    y = rows / 2;
    while( y-- )
    {
        x = cols_2;
        while( x-- )
        {
            register int L;

            cr_r   = 0*768+256 + colortab[ *cr + 0*256 ];
            crb_g  = 1*768+256 + colortab[ *cr + 1*256 ]
                               + colortab[ *cb + 2*256 ];
            cb_b   = 2*768+256 + colortab[ *cb + 3*256 ];
            ++cr; ++cb;

            L = *lum++;
            row1[0] = row1[next_row] = (rgb_2_pix[ L + cr_r ] |
                                        rgb_2_pix[ L + crb_g ] |
                                        rgb_2_pix[ L + cb_b ]);
            row1++;

            L = *lum++;
            row1[0] = row1[next_row] = (rgb_2_pix[ L + cr_r ] |
                                        rgb_2_pix[ L + crb_g ] |
                                        rgb_2_pix[ L + cb_b ]);
            row1++;


            

            L = *lum2++;
            row2[0] = row2[next_row] = (rgb_2_pix[ L + cr_r ] |
                                        rgb_2_pix[ L + crb_g ] |
                                        rgb_2_pix[ L + cb_b ]);
            row2++;

            L = *lum2++;
            row2[0] = row2[next_row] = (rgb_2_pix[ L + cr_r ] |
                                        rgb_2_pix[ L + crb_g ] |
                                        rgb_2_pix[ L + cb_b ]);
            row2++;
        }

        lum  += cols;
        lum2 += cols;
        row1 += mod;
        row2 += mod;
    }
}

static void Color24DitherYV12Mod2X( int *colortab, Uint32 *rgb_2_pix,
                                    unsigned char *lum, unsigned char *cr,
                                    unsigned char *cb, unsigned char *out,
                                    int rows, int cols, int mod )
{
    unsigned int value;
    unsigned char* row1 = out;
    const int next_row = (cols*2 + mod) * 3;
    unsigned char* row2 = row1 + 2*next_row;
    unsigned char* lum2;
    int x, y;
    int cr_r;
    int crb_g;
    int cb_b;
    int cols_2 = cols / 2;

    lum2 = lum + cols;

    mod = next_row*3 + mod*3;

    y = rows / 2;
    while( y-- )
    {
        x = cols_2;
        while( x-- )
        {
            register int L;

            cr_r   = 0*768+256 + colortab[ *cr + 0*256 ];
            crb_g  = 1*768+256 + colortab[ *cr + 1*256 ]
                               + colortab[ *cb + 2*256 ];
            cb_b   = 2*768+256 + colortab[ *cb + 3*256 ];
            ++cr; ++cb;

            L = *lum++;
            value = (rgb_2_pix[ L + cr_r ] |
                     rgb_2_pix[ L + crb_g ] |
                     rgb_2_pix[ L + cb_b ]);
            row1[0+0] = row1[3+0] = row1[next_row+0] = row1[next_row+3+0] =
                     (value      ) & 0xFF;
            row1[0+1] = row1[3+1] = row1[next_row+1] = row1[next_row+3+1] =
                     (value >>  8) & 0xFF;
            row1[0+2] = row1[3+2] = row1[next_row+2] = row1[next_row+3+2] =
                     (value >> 16) & 0xFF;
            row1 += 2*3;

            L = *lum++;
            value = (rgb_2_pix[ L + cr_r ] |
                     rgb_2_pix[ L + crb_g ] |
                     rgb_2_pix[ L + cb_b ]);
            row1[0+0] = row1[3+0] = row1[next_row+0] = row1[next_row+3+0] =
                     (value      ) & 0xFF;
            row1[0+1] = row1[3+1] = row1[next_row+1] = row1[next_row+3+1] =
                     (value >>  8) & 0xFF;
            row1[0+2] = row1[3+2] = row1[next_row+2] = row1[next_row+3+2] =
                     (value >> 16) & 0xFF;
            row1 += 2*3;


            

            L = *lum2++;
            value = (rgb_2_pix[ L + cr_r ] |
                     rgb_2_pix[ L + crb_g ] |
                     rgb_2_pix[ L + cb_b ]);
            row2[0+0] = row2[3+0] = row2[next_row+0] = row2[next_row+3+0] =
                     (value      ) & 0xFF;
            row2[0+1] = row2[3+1] = row2[next_row+1] = row2[next_row+3+1] =
                     (value >>  8) & 0xFF;
            row2[0+2] = row2[3+2] = row2[next_row+2] = row2[next_row+3+2] =
                     (value >> 16) & 0xFF;
            row2 += 2*3;

            L = *lum2++;
            value = (rgb_2_pix[ L + cr_r ] |
                     rgb_2_pix[ L + crb_g ] |
                     rgb_2_pix[ L + cb_b ]);
            row2[0+0] = row2[3+0] = row2[next_row+0] = row2[next_row+3+0] =
                     (value      ) & 0xFF;
            row2[0+1] = row2[3+1] = row2[next_row+1] = row2[next_row+3+1] =
                     (value >>  8) & 0xFF;
            row2[0+2] = row2[3+2] = row2[next_row+2] = row2[next_row+3+2] =
                     (value >> 16) & 0xFF;
            row2 += 2*3;
        }

        lum  += cols;
        lum2 += cols;
        row1 += mod;
        row2 += mod;
    }
}

static void Color32DitherYV12Mod2X( int *colortab, Uint32 *rgb_2_pix,
                                    unsigned char *lum, unsigned char *cr,
                                    unsigned char *cb, unsigned char *out,
                                    int rows, int cols, int mod )
{
    unsigned int* row1 = (unsigned int*) out;
    const int next_row = cols*2+mod;
    unsigned int* row2 = row1 + 2*next_row;
    unsigned char* lum2;
    int x, y;
    int cr_r;
    int crb_g;
    int cb_b;
    int cols_2 = cols / 2;

    lum2 = lum + cols;

    mod = (next_row * 3) + mod;

    y = rows / 2;
    while( y-- )
    {
        x = cols_2;
        while( x-- )
        {
            register int L;

            cr_r   = 0*768+256 + colortab[ *cr + 0*256 ];
            crb_g  = 1*768+256 + colortab[ *cr + 1*256 ]
                               + colortab[ *cb + 2*256 ];
            cb_b   = 2*768+256 + colortab[ *cb + 3*256 ];
            ++cr; ++cb;

            L = *lum++;
            row1[0] = row1[1] = row1[next_row] = row1[next_row+1] =
                                       (rgb_2_pix[ L + cr_r ] |
                                        rgb_2_pix[ L + crb_g ] |
                                        rgb_2_pix[ L + cb_b ]);
            row1 += 2;

            L = *lum++;
            row1[0] = row1[1] = row1[next_row] = row1[next_row+1] =
                                       (rgb_2_pix[ L + cr_r ] |
                                        rgb_2_pix[ L + crb_g ] |
                                        rgb_2_pix[ L + cb_b ]);
            row1 += 2;


            

            L = *lum2++;
            row2[0] = row2[1] = row2[next_row] = row2[next_row+1] =
                                       (rgb_2_pix[ L + cr_r ] |
                                        rgb_2_pix[ L + crb_g ] |
                                        rgb_2_pix[ L + cb_b ]);
            row2 += 2;

            L = *lum2++;
            row2[0] = row2[1] = row2[next_row] = row2[next_row+1] =
                                       (rgb_2_pix[ L + cr_r ] |
                                        rgb_2_pix[ L + crb_g ] |
                                        rgb_2_pix[ L + cb_b ]);
            row2 += 2;
        }

        lum  += cols;
        lum2 += cols;
        row1 += mod;
        row2 += mod;
    }
}

static void Color16DitherYUY2Mod1X( int *colortab, Uint32 *rgb_2_pix,
                                    unsigned char *lum, unsigned char *cr,
                                    unsigned char *cb, unsigned char *out,
                                    int rows, int cols, int mod )
{
    unsigned short* row;
    int x, y;
    int cr_r;
    int crb_g;
    int cb_b;
    int cols_2 = cols / 2;

    row = (unsigned short*) out;

    y = rows;
    while( y-- )
    {
        x = cols_2;
        while( x-- )
        {
            register int L;

            cr_r   = 0*768+256 + colortab[ *cr + 0*256 ];
            crb_g  = 1*768+256 + colortab[ *cr + 1*256 ]
                               + colortab[ *cb + 2*256 ];
            cb_b   = 2*768+256 + colortab[ *cb + 3*256 ];
            cr += 4; cb += 4;

            L = *lum; lum += 2;
            *row++ = (unsigned short)(rgb_2_pix[ L + cr_r ] |
                                      rgb_2_pix[ L + crb_g ] |
                                      rgb_2_pix[ L + cb_b ]);

            L = *lum; lum += 2;
            *row++ = (unsigned short)(rgb_2_pix[ L + cr_r ] |
                                      rgb_2_pix[ L + crb_g ] |
                                      rgb_2_pix[ L + cb_b ]);

        }

        row += mod;
    }
}

static void Color24DitherYUY2Mod1X( int *colortab, Uint32 *rgb_2_pix,
                                    unsigned char *lum, unsigned char *cr,
                                    unsigned char *cb, unsigned char *out,
                                    int rows, int cols, int mod )
{
    unsigned int value;
    unsigned char* row;
    int x, y;
    int cr_r;
    int crb_g;
    int cb_b;
    int cols_2 = cols / 2;

    row = (unsigned char*) out;
    mod *= 3;
    y = rows;
    while( y-- )
    {
        x = cols_2;
        while( x-- )
        {
            register int L;

            cr_r   = 0*768+256 + colortab[ *cr + 0*256 ];
            crb_g  = 1*768+256 + colortab[ *cr + 1*256 ]
                               + colortab[ *cb + 2*256 ];
            cb_b   = 2*768+256 + colortab[ *cb + 3*256 ];
            cr += 4; cb += 4;

            L = *lum; lum += 2;
            value = (rgb_2_pix[ L + cr_r ] |
                     rgb_2_pix[ L + crb_g ] |
                     rgb_2_pix[ L + cb_b ]);
            *row++ = (value      ) & 0xFF;
            *row++ = (value >>  8) & 0xFF;
            *row++ = (value >> 16) & 0xFF;

            L = *lum; lum += 2;
            value = (rgb_2_pix[ L + cr_r ] |
                     rgb_2_pix[ L + crb_g ] |
                     rgb_2_pix[ L + cb_b ]);
            *row++ = (value      ) & 0xFF;
            *row++ = (value >>  8) & 0xFF;
            *row++ = (value >> 16) & 0xFF;

        }
        row += mod;
    }
}

static void Color32DitherYUY2Mod1X( int *colortab, Uint32 *rgb_2_pix,
                                    unsigned char *lum, unsigned char *cr,
                                    unsigned char *cb, unsigned char *out,
                                    int rows, int cols, int mod )
{
    unsigned int* row;
    int x, y;
    int cr_r;
    int crb_g;
    int cb_b;
    int cols_2 = cols / 2;

    row = (unsigned int*) out;
    y = rows;
    while( y-- )
    {
        x = cols_2;
        while( x-- )
        {
            register int L;

            cr_r   = 0*768+256 + colortab[ *cr + 0*256 ];
            crb_g  = 1*768+256 + colortab[ *cr + 1*256 ]
                               + colortab[ *cb + 2*256 ];
            cb_b   = 2*768+256 + colortab[ *cb + 3*256 ];
            cr += 4; cb += 4;

            L = *lum; lum += 2;
            *row++ = (rgb_2_pix[ L + cr_r ] |
                       rgb_2_pix[ L + crb_g ] |
                       rgb_2_pix[ L + cb_b ]);

            L = *lum; lum += 2;
            *row++ = (rgb_2_pix[ L + cr_r ] |
                       rgb_2_pix[ L + crb_g ] |
                       rgb_2_pix[ L + cb_b ]);


        }
        row += mod;
    }
}

static void Color16DitherYUY2Mod2X( int *colortab, Uint32 *rgb_2_pix,
                                    unsigned char *lum, unsigned char *cr,
                                    unsigned char *cb, unsigned char *out,
                                    int rows, int cols, int mod )
{
    unsigned int* row = (unsigned int*) out;
    const int next_row = cols+(mod/2);
    int x, y;
    int cr_r;
    int crb_g;
    int cb_b;
    int cols_2 = cols / 2;

    y = rows;
    while( y-- )
    {
        x = cols_2;
        while( x-- )
        {
            register int L;

            cr_r   = 0*768+256 + colortab[ *cr + 0*256 ];
            crb_g  = 1*768+256 + colortab[ *cr + 1*256 ]
                               + colortab[ *cb + 2*256 ];
            cb_b   = 2*768+256 + colortab[ *cb + 3*256 ];
            cr += 4; cb += 4;

            L = *lum; lum += 2;
            row[0] = row[next_row] = (rgb_2_pix[ L + cr_r ] |
                                        rgb_2_pix[ L + crb_g ] |
                                        rgb_2_pix[ L + cb_b ]);
            row++;

            L = *lum; lum += 2;
            row[0] = row[next_row] = (rgb_2_pix[ L + cr_r ] |
                                        rgb_2_pix[ L + crb_g ] |
                                        rgb_2_pix[ L + cb_b ]);
            row++;

        }
        row += next_row;
    }
}

static void Color24DitherYUY2Mod2X( int *colortab, Uint32 *rgb_2_pix,
                                    unsigned char *lum, unsigned char *cr,
                                    unsigned char *cb, unsigned char *out,
                                    int rows, int cols, int mod )
{
    unsigned int value;
    unsigned char* row = out;
    const int next_row = (cols*2 + mod) * 3;
    int x, y;
    int cr_r;
    int crb_g;
    int cb_b;
    int cols_2 = cols / 2;
    y = rows;
    while( y-- )
    {
        x = cols_2;
        while( x-- )
        {
            register int L;

            cr_r   = 0*768+256 + colortab[ *cr + 0*256 ];
            crb_g  = 1*768+256 + colortab[ *cr + 1*256 ]
                               + colortab[ *cb + 2*256 ];
            cb_b   = 2*768+256 + colortab[ *cb + 3*256 ];
            cr += 4; cb += 4;

            L = *lum; lum += 2;
            value = (rgb_2_pix[ L + cr_r ] |
                     rgb_2_pix[ L + crb_g ] |
                     rgb_2_pix[ L + cb_b ]);
            row[0+0] = row[3+0] = row[next_row+0] = row[next_row+3+0] =
                     (value      ) & 0xFF;
            row[0+1] = row[3+1] = row[next_row+1] = row[next_row+3+1] =
                     (value >>  8) & 0xFF;
            row[0+2] = row[3+2] = row[next_row+2] = row[next_row+3+2] =
                     (value >> 16) & 0xFF;
            row += 2*3;

            L = *lum; lum += 2;
            value = (rgb_2_pix[ L + cr_r ] |
                     rgb_2_pix[ L + crb_g ] |
                     rgb_2_pix[ L + cb_b ]);
            row[0+0] = row[3+0] = row[next_row+0] = row[next_row+3+0] =
                     (value      ) & 0xFF;
            row[0+1] = row[3+1] = row[next_row+1] = row[next_row+3+1] =
                     (value >>  8) & 0xFF;
            row[0+2] = row[3+2] = row[next_row+2] = row[next_row+3+2] =
                     (value >> 16) & 0xFF;
            row += 2*3;

        }
        row += next_row;
    }
}

static void Color32DitherYUY2Mod2X( int *colortab, Uint32 *rgb_2_pix,
                                    unsigned char *lum, unsigned char *cr,
                                    unsigned char *cb, unsigned char *out,
                                    int rows, int cols, int mod )
{
    unsigned int* row = (unsigned int*) out;
    const int next_row = cols*2+mod;
    int x, y;
    int cr_r;
    int crb_g;
    int cb_b;
    int cols_2 = cols / 2;
    mod+=mod;
    y = rows;
    while( y-- )
    {
        x = cols_2;
        while( x-- )
        {
            register int L;

            cr_r   = 0*768+256 + colortab[ *cr + 0*256 ];
            crb_g  = 1*768+256 + colortab[ *cr + 1*256 ]
                               + colortab[ *cb + 2*256 ];
            cb_b   = 2*768+256 + colortab[ *cb + 3*256 ];
            cr += 4; cb += 4;

            L = *lum; lum += 2;
            row[0] = row[1] = row[next_row] = row[next_row+1] =
                                       (rgb_2_pix[ L + cr_r ] |
                                        rgb_2_pix[ L + crb_g ] |
                                        rgb_2_pix[ L + cb_b ]);
            row += 2;

            L = *lum; lum += 2;
            row[0] = row[1] = row[next_row] = row[next_row+1] =
                                       (rgb_2_pix[ L + cr_r ] |
                                        rgb_2_pix[ L + crb_g ] |
                                        rgb_2_pix[ L + cb_b ]);
            row += 2;


        }

        row += next_row;
    }
}

static int number_of_bits_set( Uint32 a )
{
    if(!a) return 0;
    if(a & 1) return 1 + number_of_bits_set(a >> 1);
    return(number_of_bits_set(a >> 1));
}

static int free_bits_at_bottom( Uint32 a )
{
      
    if(!a) return sizeof(Uint32) * 8;
    if(((Sint32)a) & 1l) return 0;
    return 1 + free_bits_at_bottom ( a >> 1);
}


SDL_Overlay *SDL_CreateYUV_SW(_THIS, int width, int height, Uint32 format, SDL_Surface *display)
{
	SDL_Overlay *overlay;
	struct private_yuvhwdata *swdata;
	int *Cr_r_tab;
	int *Cr_g_tab;
	int *Cb_g_tab;
	int *Cb_b_tab;
	Uint32 *r_2_pix_alloc;
	Uint32 *g_2_pix_alloc;
	Uint32 *b_2_pix_alloc;
	int i;
	int CR, CB;
	Uint32 Rmask, Gmask, Bmask;

	
	if ( (display->format->BytesPerPixel != 2) &&
	     (display->format->BytesPerPixel != 3) &&
	     (display->format->BytesPerPixel != 4) ) {
		SDL_SetError("Can't use YUV data on non 16/24/32 bit surfaces");
		return(NULL);
	}

	
	switch (format) {
	    case SDL_YV12_OVERLAY:
	    case SDL_IYUV_OVERLAY:
	    case SDL_YUY2_OVERLAY:
	    case SDL_UYVY_OVERLAY:
	    case SDL_YVYU_OVERLAY:
		break;
	    default:
		SDL_SetError("Unsupported YUV format");
		return(NULL);
	}

	
	overlay = (SDL_Overlay *)SDL_malloc(sizeof *overlay);
	if ( overlay == NULL ) {
		SDL_OutOfMemory();
		return(NULL);
	}
	SDL_memset(overlay, 0, (sizeof *overlay));

	
	overlay->format = format;
	overlay->w = width;
	overlay->h = height;

	
	overlay->hwfuncs = &sw_yuvfuncs;

	
	swdata = (struct private_yuvhwdata *)SDL_malloc(sizeof *swdata);
	overlay->hwdata = swdata;
	if ( swdata == NULL ) {
		SDL_OutOfMemory();
		SDL_FreeYUVOverlay(overlay);
		return(NULL);
	}
	swdata->stretch = NULL;
	swdata->display = display;
	swdata->pixels = (Uint8 *) SDL_malloc(width*height*2);
	swdata->colortab = (int *)SDL_malloc(4*256*sizeof(int));
	Cr_r_tab = &swdata->colortab[0*256];
	Cr_g_tab = &swdata->colortab[1*256];
	Cb_g_tab = &swdata->colortab[2*256];
	Cb_b_tab = &swdata->colortab[3*256];
	swdata->rgb_2_pix = (Uint32 *)SDL_malloc(3*768*sizeof(Uint32));
	r_2_pix_alloc = &swdata->rgb_2_pix[0*768];
	g_2_pix_alloc = &swdata->rgb_2_pix[1*768];
	b_2_pix_alloc = &swdata->rgb_2_pix[2*768];
	if ( ! swdata->pixels || ! swdata->colortab || ! swdata->rgb_2_pix ) {
		SDL_OutOfMemory();
		SDL_FreeYUVOverlay(overlay);
		return(NULL);
	}

	
	for (i=0; i<256; i++) {
		CB = CR = (i-128);
		Cr_r_tab[i] = (int) ( (0.419/0.299) * CR);
		Cr_g_tab[i] = (int) (-(0.299/0.419) * CR);
		Cb_g_tab[i] = (int) (-(0.114/0.331) * CB); 
		Cb_b_tab[i] = (int) ( (0.587/0.331) * CB);
	}

	Rmask = display->format->Rmask;
	Gmask = display->format->Gmask;
	Bmask = display->format->Bmask;
	for ( i=0; i<256; ++i ) {
		r_2_pix_alloc[i+256] = i >> (8 - number_of_bits_set(Rmask));
		r_2_pix_alloc[i+256] <<= free_bits_at_bottom(Rmask);
		g_2_pix_alloc[i+256] = i >> (8 - number_of_bits_set(Gmask));
		g_2_pix_alloc[i+256] <<= free_bits_at_bottom(Gmask);
		b_2_pix_alloc[i+256] = i >> (8 - number_of_bits_set(Bmask));
		b_2_pix_alloc[i+256] <<= free_bits_at_bottom(Bmask);
	}

	if( display->format->BytesPerPixel == 2 ) {
		for ( i=0; i<256; ++i ) {
			r_2_pix_alloc[i+256] |= (r_2_pix_alloc[i+256]) << 16;
			g_2_pix_alloc[i+256] |= (g_2_pix_alloc[i+256]) << 16;
			b_2_pix_alloc[i+256] |= (b_2_pix_alloc[i+256]) << 16;
		}
	}

	for ( i=0; i<256; ++i ) {
		r_2_pix_alloc[i] = r_2_pix_alloc[256];
		r_2_pix_alloc[i+512] = r_2_pix_alloc[511];
		g_2_pix_alloc[i] = g_2_pix_alloc[256];
		g_2_pix_alloc[i+512] = g_2_pix_alloc[511];
		b_2_pix_alloc[i] = b_2_pix_alloc[256];
		b_2_pix_alloc[i+512] = b_2_pix_alloc[511];
	}

	
	switch (format) {
	    case SDL_YV12_OVERLAY:
	    case SDL_IYUV_OVERLAY:
		if ( display->format->BytesPerPixel == 2 ) {
#if (__GNUC__ > 2) && defined(__i386__) && __OPTIMIZE__ && SDL_ASSEMBLY_ROUTINES
			
			if ( SDL_HasMMX() && (Rmask == 0xF800) &&
			                     (Gmask == 0x07E0) &&
				             (Bmask == 0x001F) &&
			                     (width & 15) == 0) {
				swdata->Display1X = Color565DitherYV12MMX1X;
			} else {
				swdata->Display1X = Color16DitherYV12Mod1X;
			}
#else
			swdata->Display1X = Color16DitherYV12Mod1X;
#endif
			swdata->Display2X = Color16DitherYV12Mod2X;
		}
		if ( display->format->BytesPerPixel == 3 ) {
			swdata->Display1X = Color24DitherYV12Mod1X;
			swdata->Display2X = Color24DitherYV12Mod2X;
		}
		if ( display->format->BytesPerPixel == 4 ) {
#if (__GNUC__ > 2) && defined(__i386__) && __OPTIMIZE__ && SDL_ASSEMBLY_ROUTINES
			
			if ( SDL_HasMMX() && (Rmask == 0x00FF0000) &&
			                     (Gmask == 0x0000FF00) &&
				             (Bmask == 0x000000FF) && 
			                     (width & 15) == 0) {
				swdata->Display1X = ColorRGBDitherYV12MMX1X;
			} else {
				swdata->Display1X = Color32DitherYV12Mod1X;
			}
#else
			swdata->Display1X = Color32DitherYV12Mod1X;
#endif
			swdata->Display2X = Color32DitherYV12Mod2X;
		}
		break;
	    case SDL_YUY2_OVERLAY:
	    case SDL_UYVY_OVERLAY:
	    case SDL_YVYU_OVERLAY:
		if ( display->format->BytesPerPixel == 2 ) {
			swdata->Display1X = Color16DitherYUY2Mod1X;
			swdata->Display2X = Color16DitherYUY2Mod2X;
		}
		if ( display->format->BytesPerPixel == 3 ) {
			swdata->Display1X = Color24DitherYUY2Mod1X;
			swdata->Display2X = Color24DitherYUY2Mod2X;
		}
		if ( display->format->BytesPerPixel == 4 ) {
			swdata->Display1X = Color32DitherYUY2Mod1X;
			swdata->Display2X = Color32DitherYUY2Mod2X;
		}
		break;
	    default:
		
		break;
	}

	
	overlay->pitches = swdata->pitches;
	overlay->pixels = swdata->planes;
	switch (format) {
	    case SDL_YV12_OVERLAY:
	    case SDL_IYUV_OVERLAY:
		overlay->pitches[0] = overlay->w;
		overlay->pitches[1] = overlay->pitches[0] / 2;
		overlay->pitches[2] = overlay->pitches[0] / 2;
	        overlay->pixels[0] = swdata->pixels;
	        overlay->pixels[1] = overlay->pixels[0] +
		                     overlay->pitches[0] * overlay->h;
	        overlay->pixels[2] = overlay->pixels[1] +
		                     overlay->pitches[1] * overlay->h / 2;
		overlay->planes = 3;
		break;
	    case SDL_YUY2_OVERLAY:
	    case SDL_UYVY_OVERLAY:
	    case SDL_YVYU_OVERLAY:
		overlay->pitches[0] = overlay->w*2;
	        overlay->pixels[0] = swdata->pixels;
		overlay->planes = 1;
		break;
	    default:
		
		break;
	}

	
	return(overlay);
}

int SDL_LockYUV_SW(_THIS, SDL_Overlay *overlay)
{
	return(0);
}

void SDL_UnlockYUV_SW(_THIS, SDL_Overlay *overlay)
{
	return;
}

int SDL_DisplayYUV_SW(_THIS, SDL_Overlay *overlay, SDL_Rect *src, SDL_Rect *dst)
{
	struct private_yuvhwdata *swdata;
	int stretch;
	int scale_2x;
	SDL_Surface *display;
	Uint8 *lum, *Cr, *Cb;
	Uint8 *dstp;
	int mod;

	swdata = overlay->hwdata;
	stretch = 0;
	scale_2x = 0;
	if ( src->x || src->y || src->w < overlay->w || src->h < overlay->h ) {
		stretch = 1;
	} else if ( (src->w != dst->w) || (src->h != dst->h) ) {
		if ( (dst->w == 2*src->w) &&
		     (dst->h == 2*src->h) ) {
			scale_2x = 1;
		} else {
			stretch = 1;
		}
	}
	if ( stretch ) {
		if ( ! swdata->stretch ) {
			display = swdata->display;
			swdata->stretch = SDL_CreateRGBSurface(
				SDL_SWSURFACE,
				overlay->w, overlay->h,
				display->format->BitsPerPixel,
				display->format->Rmask,
				display->format->Gmask,
				display->format->Bmask, 0);
			if ( ! swdata->stretch ) {
				return(-1);
			}
		}
		display = swdata->stretch;
	} else {
		display = swdata->display;
	}
	switch (overlay->format) {
	    case SDL_YV12_OVERLAY:
		lum = overlay->pixels[0];
		Cr =  overlay->pixels[1];
		Cb =  overlay->pixels[2];
		break;
	    case SDL_IYUV_OVERLAY:
		lum = overlay->pixels[0];
		Cr =  overlay->pixels[2];
		Cb =  overlay->pixels[1];
		break;
	    case SDL_YUY2_OVERLAY:
		lum = overlay->pixels[0];
		Cr = lum + 3;
		Cb = lum + 1;
		break;
	    case SDL_UYVY_OVERLAY:
		lum = overlay->pixels[0]+1;
		Cr = lum + 1;
		Cb = lum - 1;
		break;
	    case SDL_YVYU_OVERLAY:
		lum = overlay->pixels[0];
		Cr = lum + 1;
		Cb = lum + 3;
		break;
	    default:
		SDL_SetError("Unsupported YUV format in blit");
		return(-1);
	}
	if ( SDL_MUSTLOCK(display) ) {
        	if ( SDL_LockSurface(display) < 0 ) {
			return(-1);
		}
	}
	if ( stretch ) {
		dstp = (Uint8 *)swdata->stretch->pixels;
	} else {
		dstp = (Uint8 *)display->pixels
			+ dst->x * display->format->BytesPerPixel
			+ dst->y * display->pitch;
	}
	mod = (display->pitch / display->format->BytesPerPixel);

	if ( scale_2x ) {
		mod -= (overlay->w * 2);
		swdata->Display2X(swdata->colortab, swdata->rgb_2_pix,
		                  lum, Cr, Cb, dstp, overlay->h, overlay->w, mod);
	} else {
		mod -= overlay->w;
		swdata->Display1X(swdata->colortab, swdata->rgb_2_pix,
		                  lum, Cr, Cb, dstp, overlay->h, overlay->w, mod);
	}
	if ( SDL_MUSTLOCK(display) ) {
		SDL_UnlockSurface(display);
	}
	if ( stretch ) {
		display = swdata->display;
		SDL_SoftStretch(swdata->stretch, src, display, dst);
	}
	SDL_UpdateRects(display, 1, dst);

	return(0);
}

void SDL_FreeYUV_SW(_THIS, SDL_Overlay *overlay)
{
	struct private_yuvhwdata *swdata;

	swdata = overlay->hwdata;
	if ( swdata ) {
		if ( swdata->stretch ) {
			SDL_FreeSurface(swdata->stretch);
		}
		if ( swdata->pixels ) {
			SDL_free(swdata->pixels);
		}
		if ( swdata->colortab ) {
			SDL_free(swdata->colortab);
		}
		if ( swdata->rgb_2_pix ) {
			SDL_free(swdata->rgb_2_pix);
		}
		SDL_free(swdata);
		overlay->hwdata = NULL;
	}
}