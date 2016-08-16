/***************************************************************************\
|*                                                                           *|
|*       Copyright 1993-1999 NVIDIA, Corporation.  All rights reserved.      *|
|*                                                                           *|
|*     NOTICE TO USER:   The source code  is copyrighted under  U.S. and     *|
|*     international laws.  Users and possessors of this source code are     *|
|*     hereby granted a nonexclusive,  royalty-free copyright license to     *|
|*     use this code in individual and commercial software.                  *|
|*                                                                           *|
|*     Any use of this source code must include,  in the user documenta-     *|
|*     tion and  internal comments to the code,  notices to the end user     *|
|*     as follows:                                                           *|
|*                                                                           *|
|*       Copyright 1993-1999 NVIDIA, Corporation.  All rights reserved.      *|
|*                                                                           *|
|*     NVIDIA, CORPORATION MAKES NO REPRESENTATION ABOUT THE SUITABILITY     *|
|*     OF  THIS SOURCE  CODE  FOR ANY PURPOSE.  IT IS  PROVIDED  "AS IS"     *|
|*     WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND.  NVIDIA, CORPOR-     *|
|*     ATION DISCLAIMS ALL WARRANTIES  WITH REGARD  TO THIS SOURCE CODE,     *|
|*     INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY, NONINFRINGE-     *|
|*     MENT,  AND FITNESS  FOR A PARTICULAR PURPOSE.   IN NO EVENT SHALL     *|
|*     NVIDIA, CORPORATION  BE LIABLE FOR ANY SPECIAL,  INDIRECT,  INCI-     *|
|*     DENTAL, OR CONSEQUENTIAL DAMAGES,  OR ANY DAMAGES  WHATSOEVER RE-     *|
|*     SULTING FROM LOSS OF USE,  DATA OR PROFITS,  WHETHER IN AN ACTION     *|
|*     OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,  ARISING OUT OF     *|
|*     OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOURCE CODE.     *|
|*                                                                           *|
|*     U.S. Government  End  Users.   This source code  is a "commercial     *|
|*     item,"  as that  term is  defined at  48 C.F.R. 2.101 (OCT 1995),     *|
|*     consisting  of "commercial  computer  software"  and  "commercial     *|
|*     computer  software  documentation,"  as such  terms  are  used in     *|
|*     48 C.F.R. 12.212 (SEPT 1995)  and is provided to the U.S. Govern-     *|
|*     ment only as  a commercial end item.   Consistent with  48 C.F.R.     *|
|*     12.212 and  48 C.F.R. 227.7202-1 through  227.7202-4 (JUNE 1995),     *|
|*     all U.S. Government End Users  acquire the source code  with only     *|
|*     those rights set forth herein.                                        *|
|*                                                                           *|
\***************************************************************************/

#ifndef __RIVA_HW_H__
#define __RIVA_HW_H__
#define RIVA_SW_VERSION 0x00010003

typedef Uint8  U008;
typedef Uint16 U016;
typedef Uint32 U032;

#define NV_WR08(p,i,d)  (((U008 *)(p))[i]=(d))
#define NV_RD08(p,i)    (((U008 *)(p))[i])
#define NV_WR16(p,i,d)  (((U016 *)(p))[(i)/2]=(d))
#define NV_RD16(p,i)    (((U016 *)(p))[(i)/2])
#define NV_WR32(p,i,d)  (((U032 *)(p))[(i)/4]=(d))
#define NV_RD32(p,i)    (((U032 *)(p))[(i)/4])
#define VGA_WR08(p,i,d) NV_WR08(p,i,d)
#define VGA_RD08(p,i)   NV_RD08(p,i)

#define NV_ARCH_03  0x03
#define NV_ARCH_04  0x04
#define NV_ARCH_10  0x10

typedef volatile struct
{
    U032 reserved00[4];
    U016 FifoFree;
    U016 Nop;
    U032 reserved01[0x0BB];
    U032 Rop3;
} RivaRop;
typedef volatile struct
{
    U032 reserved00[4];
    U016 FifoFree;
    U016 Nop;
    U032 reserved01[0x0BD];
    U032 Shape;
    U032 reserved03[0x001];
    U032 Color0;
    U032 Color1;
    U032 Monochrome[2];
} RivaPattern;
typedef volatile struct
{
    U032 reserved00[4];
    U016 FifoFree;
    U016 Nop;
    U032 reserved01[0x0BB];
    U032 TopLeft;
    U032 WidthHeight;
} RivaClip;
typedef volatile struct
{
    U032 reserved00[4];
    U016 FifoFree;
    U016 Nop[1];
    U032 reserved01[0x0BC];
    U032 Color;
    U032 reserved03[0x03E];
    U032 TopLeft;
    U032 WidthHeight;
} RivaRectangle;
typedef volatile struct
{
    U032 reserved00[4];
    U016 FifoFree;
    U016 Nop;
    U032 reserved01[0x0BB];
    U032 TopLeftSrc;
    U032 TopLeftDst;
    U032 WidthHeight;
} RivaScreenBlt;
typedef volatile struct
{
    U032 reserved00[4];
    U016 FifoFree;
    U016 Nop[1];
    U032 reserved01[0x0BC];
    U032 TopLeft;
    U032 WidthHeight;
    U032 WidthHeightIn;
    U032 reserved02[0x03C];
    U032 Pixels;
} RivaPixmap;
typedef volatile struct
{
    U032 reserved00[4];
    U016 FifoFree;
    U016 Nop;
    U032 reserved01[0x0BB];
    U032 reserved03[(0x040)-1];
    U032 Color1A;
    struct
    {
        U032 TopLeft;
        U032 WidthHeight;
    } UnclippedRectangle[64];
    U032 reserved04[(0x080)-3];
    struct
    {
        U032 TopLeft;
        U032 BottomRight;
    } ClipB;
    U032 Color1B;
    struct
    {
        U032 TopLeft;
        U032 BottomRight;
    } ClippedRectangle[64];
    U032 reserved05[(0x080)-5];
    struct
    {
        U032 TopLeft;
        U032 BottomRight;
    } ClipC;
    U032 Color1C;
    U032 WidthHeightC;
    U032 PointC;
    U032 MonochromeData1C;
    U032 reserved06[(0x080)+121];
    struct
    {
        U032 TopLeft;
        U032 BottomRight;
    } ClipD;
    U032 Color1D;
    U032 WidthHeightInD;
    U032 WidthHeightOutD;
    U032 PointD;
    U032 MonochromeData1D;
    U032 reserved07[(0x080)+120];
    struct
    {
        U032 TopLeft;
        U032 BottomRight;
    } ClipE;
    U032 Color0E;
    U032 Color1E;
    U032 WidthHeightInE;
    U032 WidthHeightOutE;
    U032 PointE;
    U032 MonochromeData01E;
} RivaBitmap;
typedef volatile struct
{
    U032 reserved00[4];
    U016 FifoFree;
    U016 Nop;
    U032 reserved01[0x0BC];
    U032 TextureOffset;
    U032 TextureFormat;
    U032 TextureFilter;
    U032 FogColor;
#ifdef Control
#undef Control
#endif
    U032 Control;
    U032 AlphaTest;
    U032 reserved02[0x339];
    U032 FogAndIndex;
    U032 Color;
    float ScreenX;
    float ScreenY;
    float ScreenZ;
    float EyeM;
    float TextureS;
    float TextureT;
} RivaTexturedTriangle03;
typedef volatile struct
{
    U032 reserved00[4];
    U016 FifoFree;
    U016 Nop;
    U032 reserved01[0x0BB];
    U032 ColorKey;
    U032 TextureOffset;
    U032 TextureFormat;
    U032 TextureFilter;
    U032 Blend;
#ifdef Control
#undef Control
#endif
    U032 Control;
    U032 FogColor;
    U032 reserved02[0x39];
    struct
    {
        float ScreenX;
        float ScreenY;
        float ScreenZ;
        float EyeM;
        U032 Color;
        U032 Specular;
        float TextureS;
        float TextureT;
    } Vertex[16];
    U032 DrawTriangle3D;
} RivaTexturedTriangle05;
typedef volatile struct
{
    U032 reserved00[4];
    U016 FifoFree;
    U016 Nop[1];
    U032 reserved01[0x0BC];
    U032 Color;             
    U032 Reserved02[0x03e];
    struct {                
        U032 point0;        
        U032 point1;        
    } Lin[16];              
    struct {                
        U032 point0X;       
        U032 point0Y;       
        U032 point1X;       
        U032 point1Y;       
    } Lin32[8];             
    U032 PolyLin[32];       
    struct {                
        U032 x;             
        U032 y;             
    } PolyLin32[16];        
    struct {                
        U032 color;         
        U032 point;         
    } ColorPolyLin[16];     
} RivaLine;
typedef volatile struct
{
    U032 reserved00[4];
    U016 FifoFree;
    U016 Nop;
    U032 reserved01[0x0BE];
    U032 Offset;
} RivaSurface;
typedef volatile struct
{
    U032 reserved00[4];
    U016 FifoFree;
    U016 Nop;
    U032 reserved01[0x0BD];
    U032 Pitch;
    U032 RenderBufferOffset;
    U032 ZBufferOffset;
} RivaSurface3D;
    

struct _riva_hw_inst;
struct _riva_hw_state;
typedef struct _riva_hw_inst
{
    U032 Architecture;
    U032 Version;
    U032 CrystalFreqKHz;
    U032 RamAmountKBytes;
    U032 MaxVClockFreqKHz;
    U032 RamBandwidthKBytesPerSec;
    U032 EnableIRQ;
    U032 IO;
    U032 VBlankBit;
    U032 FifoFreeCount;
    U032 FifoEmptyCount;
    volatile U032 *PCRTC;
    volatile U032 *PRAMDAC;
    volatile U032 *PFB;
    volatile U032 *PFIFO;
    volatile U032 *PGRAPH;
    volatile U032 *PEXTDEV;
    volatile U032 *PTIMER;
    volatile U032 *PMC;
    volatile U032 *PRAMIN;
    volatile U032 *FIFO;
    volatile U032 *CURSOR;
    volatile U032 *CURSORPOS;
    volatile U032 *VBLANKENABLE;
    volatile U032 *VBLANK;
    volatile U008 *PCIO;
    volatile U008 *PVIO;
    volatile U008 *PDIO;
    int  (*Busy)(struct _riva_hw_inst *);
    void (*CalcStateExt)(struct _riva_hw_inst *,struct _riva_hw_state *,int,int,int,int,int,int,int,int,int,int,int,int,int);
    void (*LoadStateExt)(struct _riva_hw_inst *,struct _riva_hw_state *);
    void (*UnloadStateExt)(struct _riva_hw_inst *,struct _riva_hw_state *);
    void (*SetStartAddress)(struct _riva_hw_inst *,U032);
    void (*SetSurfaces2D)(struct _riva_hw_inst *,U032,U032);
    void (*SetSurfaces3D)(struct _riva_hw_inst *,U032,U032);
    int  (*ShowHideCursor)(struct _riva_hw_inst *,int);
    void (*LockUnlock)(struct _riva_hw_inst *, int);
    struct _riva_hw_state *CurrentState;
    RivaRop                 *Rop;
    RivaPattern             *Patt;
    RivaClip                *Clip;
    RivaPixmap              *Pixmap;
    RivaScreenBlt           *Blt;
    RivaBitmap              *Bitmap;
    RivaLine                *Line;
    RivaTexturedTriangle03  *Tri03;
    RivaTexturedTriangle05  *Tri05;
} RIVA_HW_INST;
typedef struct _riva_hw_state
{
    U032 bpp;
    U032 width;
    U032 height;
    U032 repaint0;
    U032 repaint1;
    U032 screen;
    U032 pixel;
    U032 horiz;
    U032 arbitration0;
    U032 arbitration1;
    U032 vpll;
    U032 pllsel;
    U032 general;
    U032 config;
    U032 cursor0;
    U032 cursor1;
    U032 cursor2;
    U032 offset0;
    U032 offset1;
    U032 offset2;
    U032 offset3;
    U032 pitch0;
    U032 pitch1;
    U032 pitch2;
    U032 pitch3;
} RIVA_HW_STATE;


#define RIVA_FIFO_FREE(hwptr,cnt)                                  \
{                                                                  \
   while (FifoFreeCount < (cnt))                                   \
	FifoFreeCount = hwptr->FifoFree >> 2;                      \
   FifoFreeCount -= (cnt);                                         \
}
#endif 
