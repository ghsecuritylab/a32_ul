

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2000-2013 Julian Seward
      jseward@acm.org
   Copyright (C) 2005-2013 Nicholas Nethercote
      njn@valgrind.org
   Copyright (C) 2006-2013 OpenWorks LLP
      info@open-works.co.uk

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


#ifndef __PUB_TOOL_VKI_H
#define __PUB_TOOL_VKI_H

#if defined(VGO_linux)
#  include "vki/vki-linux.h"
#  include "vki/vki-linux-drm.h"
#elif defined(VGO_darwin)
#  include "vki/vki-darwin.h"
#else
#  error Unknown Plat/OS
#endif

#if defined(VGP_amd64_linux) || defined(VGP_x86_linux)
#  include "vki/vki-xen.h"
#endif


#endif 
