/* Convert from memory to file representation.
   Copyright (C) 1998, 1999, 2000, 2002 Red Hat, Inc.
   This file is part of Red Hat elfutils.
   Written by Ulrich Drepper <drepper@redhat.com>, 1998.

   Red Hat elfutils is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by the
   Free Software Foundation; version 2 of the License.

   Red Hat elfutils is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with Red Hat elfutils; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301 USA.

   In addition, as a special exception, Red Hat, Inc. gives You the
   additional right to link the code of Red Hat elfutils with code licensed
   under any Open Source Initiative certified open source license
   (http://www.opensource.org/licenses/index.php) which requires the
   distribution of source code with any binary distribution and to
   distribute linked combinations of the two.  Non-GPL Code permitted under
   this exception must only link to the code of Red Hat elfutils through
   those well defined interfaces identified in the file named EXCEPTION
   found in the source code files (the "Approved Interfaces").  The files
   of Non-GPL Code may instantiate templates or use macros or inline
   functions from the Approved Interfaces without causing the resulting
   work to be covered by the GNU General Public License.  Only Red Hat,
   Inc. may make changes or additions to the list of Approved Interfaces.
   Red Hat's grant of this exception is conditioned upon your not adding
   any new exceptions.  If you wish to add a new Approved Interface or
   exception, please contact Red Hat.  You must obey the GNU General Public
   License in all respects for all of the Red Hat elfutils code and other
   code used in conjunction with Red Hat elfutils except the Non-GPL Code
   covered by this exception.  If you modify this file, you may extend this
   exception to your version of the file, but you are not obligated to do
   so.  If you do not wish to provide this exception without modification,
   you must delete this exception statement from your version and license
   this file solely under the GPL without exception.

   Red Hat elfutils is an included package of the Open Invention Network.
   An included package of the Open Invention Network is a package for which
   Open Invention Network licensees cross-license their patents.  No patent
   license is granted, either expressly or impliedly, by designation as an
   included package.  Should you wish to participate in the Open Invention
   Network licensing program, please visit www.openinventionnetwork.com
   <http://www.openinventionnetwork.com>.  */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <assert.h>
#include <endian.h>
#include <string.h>

#include "libelfP.h"

#ifndef LIBELFBITS
# define LIBELFBITS	32
#endif


Elf_Data *
elfw2(LIBELFBITS, xlatetof) (dest, src, encode)
     Elf_Data *dest;
     const Elf_Data *src;
     unsigned int encode;
{
#if EV_NUM != 2
  size_t recsize = __libelf_type_sizes[src->d_version - 1][ELFW(ELFCLASS,LIBELFBITS) - 1][src->d_type];
#else
  size_t recsize = __libelf_type_sizes[0][ELFW(ELFCLASS,LIBELFBITS) - 1][src->d_type];
#endif

  if (src->d_size % recsize != 0)
    {
      __libelf_seterrno (ELF_E_INVALID_DATA);
      return NULL;
    }

  
  if (src->d_size > dest->d_size)
    {
      __libelf_seterrno (ELF_E_DEST_SIZE);
      return NULL;
    }

  
  if (encode != ELFDATA2LSB && encode != ELFDATA2MSB)
    {
      __libelf_seterrno (ELF_E_INVALID_ENCODING);
      return NULL;
    }

  if ((__BYTE_ORDER == __LITTLE_ENDIAN && encode == ELFDATA2LSB)
      || (__BYTE_ORDER == __BIG_ENDIAN && encode == ELFDATA2MSB))
    {
      
      if (src->d_buf != dest->d_buf)
	memmove (dest->d_buf, src->d_buf, src->d_size);
    }
  else
    {
      xfct_t fctp;

#if EV_NUM != 2
      fctp = __elf_xfctstom[dest->d_version - 1][src->d_version - 1][ELFW(ELFCLASS, LIBELFBITS) - 1][src->d_type];
#else
      fctp = __elf_xfctstom[0][0][ELFW(ELFCLASS, LIBELFBITS) - 1][src->d_type];
#endif

      
      (*fctp) (dest->d_buf, src->d_buf, src->d_size, 1);
    }

  dest->d_type = src->d_type;
  dest->d_size = src->d_size;

  return dest;
}
INTDEF(elfw2(LIBELFBITS, xlatetof))
