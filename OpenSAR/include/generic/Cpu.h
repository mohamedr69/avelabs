/* -------------------------------- Arctic Core ------------------------------
 * Arctic Core - the open source AUTOSAR platform http://arccore.com
 *
 * Copyright (C) 2009  ArcCore AB <contact@arccore.com>
 *
 * This source code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation; See <http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 * -------------------------------- Arctic Core ------------------------------*/



#ifndef CPU_H
#define CPU_H

#include "Std_Types.h"
typedef uint32_t imask_t;

#define Irq_Save(flags)		flags = arch_save_int()		// Dummy assignment to avoid compiler warnings
#define Irq_Restore(flags)	arch_restore_int(flags)

#define Irq_Disable()	arch_disable_int()
#define Irq_Enable()	arch_enable_int()

#define Irq_SuspendAll() 	Irq_Disable()
#define Irq_ResumeAll() 	Irq_Enable()

#define Irq_SuspendOs() 	Irq_Disable()
#define Irq_ResumeOs() 		Irq_Enable()

#define ilog2(_x)			__builtin_ffs(_x)

imask_t arch_save_int(void);
void arch_restore_int(imask_t flags);
void arch_disable_int(void);
void arch_enable_int(void);

#endif /* CPU_H */
