

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.
   Spring 2008:
      derived from readelf.c and valgrind-20031012-wine/vg_symtab2.c
      derived from wine-1.0/tools/winedump/pdb.c and msc.c

   Copyright (C) 2000-2013 Julian Seward
      jseward@acm.org

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307, USA.

   The GNU General Public License is contained in the file COPYING.
*/

#if defined(VGO_linux) || defined(VGO_darwin)

#ifndef __PRIV_READPDB_H
#define __PRIV_READPDB_H

#include "pub_core_basics.h"     
#include "pub_core_debuginfo.h"  

extern Bool ML_(read_pdb_debug_info)(
               DebugInfo* di,
               Addr       obj_avma,
               PtrdiffT   obj_bias,
               void*      pdbimage,
               SizeT      n_pdbimage,
               HChar*     pdbname,
               ULong      pdbmtime
            );

HChar* ML_(find_name_of_pdb_file)( HChar* pename );


#endif 

#endif 

