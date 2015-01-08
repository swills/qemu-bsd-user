/*
 * ARM AArch64 thread support for bsd-user.
 *
 * Copyright (c) 2015 Stacey D. Son <sson at FreeBSD>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _TARGET_ARCH_THREAD_H_
#define _TARGET_ARCH_THREAD_H_

/* Compare to arm64/arm64/vm_machdep.c cpu_set_upcall_kse() */
static inline void target_thread_set_upcall(CPUARMState *regs, abi_ulong entry,
    abi_ulong arg, abi_ulong stack_base, abi_ulong stack_size)
{
    abi_ulong sp;

    /*
     * Make sure the stack is properly aligned.
     * arm/include/param.h (STACKLIGN() macro)
     */
    sp = (u_int)((stack_base + stack_size) -
        sizeof(struct target_trapframe)) & ~(16 - 1);

    /* sp = stack base */
    regs->xregs[31] = sp;
    /* pc = start function entry */
    regs->pc = entry &  ~0x3ULL;
    /* r0 = arg */
    regs->xregs[0] = arg;
#if 0
    regs->spsr = ARM_CPU_MODE_USR;
#endif
}

static inline void target_thread_init(struct target_pt_regs *regs,
        struct image_info *infop)
{
    abi_long stack = infop->start_stack;

    memset(regs, 0, sizeof(*regs));
    regs->pc = infop->entry &  ~0x3ULL;
    regs->sp = stack;

#if 0
    if (bsd_type == target_freebsd) {
        regs->ARM_lr = infop->entry & 0xfffffffe;
    }
    /* FIXME - what to for failure of get_user()? */
    get_user_ual(regs->ARM_r2, stack + 8); /* envp */
    get_user_ual(regs->ARM_r1, stack + 4); /* envp */
#endif
}

#endif /* !_TARGET_ARCH_THREAD_H_ */
