/* Target-dependent code for FreeBSD/Alpha.
   Copyright 2001, 2002, 2003 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include "defs.h"
#include "value.h"
#include "osabi.h"

#include "alpha-tdep.h"

static int
alphafbsd_use_struct_convention (int gcc_p, struct type *type)
{
  enum type_code code;
  int i;

  /* All aggregate types that won't fit in a register must be returned
     in memory.  */
  if (TYPE_LENGTH (type) > REGISTER_SIZE)
    return 1;

  /* The only aggregate types that can be returned in a register are
     structs and unions.  Arrays must be returned in memory.  */
  code = TYPE_CODE (type);
  if (code != TYPE_CODE_STRUCT && code != TYPE_CODE_UNION)
    return 1;

  /* We need to check if this struct/union is "integer" like.  For
     this to be true, the offset of each adressable subfield must be
     zero.  Note that bit fields are not addressable.  */
  for (i = 0; i < TYPE_NFIELDS (type); i++)
    {
      /* If the field bitsize is non-zero, it isn't adressable.  */
      if (TYPE_FIELD_BITPOS (type, i) != 0
	  && TYPE_FIELD_BITSIZE (type, i) == 0)
	return 1;
    }

  return 0;
}

static int
alphafbsd_pc_in_sigtramp (CORE_ADDR pc, char *func_name)
{
  /* FIXME */
  return 0;
}

static void
alphafbsd_init_abi (struct gdbarch_info info,
                    struct gdbarch *gdbarch)
{
  struct gdbarch_tdep *tdep = gdbarch_tdep (gdbarch);

  set_gdbarch_pc_in_sigtramp (gdbarch, alphafbsd_pc_in_sigtramp);

  set_gdbarch_use_struct_convention (gdbarch, alphafbsd_use_struct_convention);

  tdep->jb_pc = 2;
  tdep->jb_elt_size = 8;
}

void
_initialize_alphafbsd_tdep (void)
{
  gdbarch_register_osabi (bfd_arch_alpha, 0, GDB_OSABI_FREEBSD_ELF,
                          alphafbsd_init_abi);
}
