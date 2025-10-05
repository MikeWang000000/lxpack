/* SPDX-License-Identifier: LGPL-2.1 OR MIT */
/*
 * OpenRISC specific definitions for NOLIBC
 * Copyright (C) 2025 MikeWang000000
 */

#ifndef _NOLIBC_ARCH_OR1K_H
#define _NOLIBC_ARCH_OR1K_H

#include "compiler.h"

#define _NOLIBC_SYSCALL_CLOBBERLIST                                          \
    "memory", "r12", "r13", "r15", "r17", "r19", "r21", "r23", "r25", "r27", \
        "r29", "r31"

#define my_syscall0(num)                                                  \
    ({                                                                    \
        register unsigned long _num __asm__("r11") = (num);               \
                                                                          \
        __asm__ volatile("l.sys 1"                                        \
                         : "=r"(_num)                                     \
                         : "r"(_num)                                      \
                         : _NOLIBC_SYSCALL_CLOBBERLIST, "r3", "r4", "r5", \
                           "r6", "r7", "r8");                             \
        _num;                                                             \
    })

#define my_syscall1(num, arg1)                                            \
    ({                                                                    \
        register unsigned long _num __asm__("r11") = (num);               \
        register unsigned long _arg1 __asm__("r3") = (long) (arg1);       \
                                                                          \
        __asm__ volatile("l.sys 1"                                        \
                         : "=r"(_num)                                     \
                         : "r"(_num), "r"(_arg1)                          \
                         : _NOLIBC_SYSCALL_CLOBBERLIST, "r4", "r5", "r6", \
                           "r7", "r8");                                   \
        _num;                                                             \
    })

#define my_syscall2(num, arg1, arg2)                                      \
    ({                                                                    \
        register unsigned long _num __asm__("r11") = (num);               \
        register unsigned long _arg1 __asm__("r3") = (long) (arg1);       \
        register unsigned long _arg2 __asm__("r4") = (long) (arg2);       \
                                                                          \
        __asm__ volatile("l.sys 1"                                        \
                         : "=r"(_num)                                     \
                         : "r"(_num), "r"(_arg1), "r"(_arg2)              \
                         : _NOLIBC_SYSCALL_CLOBBERLIST, "r5", "r6", "r7", \
                           "r8");                                         \
        _num;                                                             \
    })

#define my_syscall3(num, arg1, arg2, arg3)                                 \
    ({                                                                     \
        register unsigned long _num __asm__("r11") = (num);                \
        register unsigned long _arg1 __asm__("r3") = (long) (arg1);        \
        register unsigned long _arg2 __asm__("r4") = (long) (arg2);        \
        register unsigned long _arg3 __asm__("r5") = (long) (arg3);        \
                                                                           \
        __asm__ volatile("l.sys 1"                                         \
                         : "=r"(_num)                                      \
                         : "r"(_num), "r"(_arg1), "r"(_arg2), "r"(_arg3)   \
                         : _NOLIBC_SYSCALL_CLOBBERLIST, "r6", "r7", "r8"); \
        _num;                                                              \
    })

#define my_syscall4(num, arg1, arg2, arg3, arg4)                          \
    ({                                                                    \
        register unsigned long _num __asm__("r11") = (num);               \
        register unsigned long _arg1 __asm__("r3") = (long) (arg1);       \
        register unsigned long _arg2 __asm__("r4") = (long) (arg2);       \
        register unsigned long _arg3 __asm__("r5") = (long) (arg3);       \
        register unsigned long _arg4 __asm__("r6") = (long) (arg4);       \
                                                                          \
        __asm__ volatile("l.sys 1"                                        \
                         : "=r"(_num)                                     \
                         : "r"(_num), "r"(_arg1), "r"(_arg2), "r"(_arg3), \
                           "r"(_arg4)                                     \
                         : _NOLIBC_SYSCALL_CLOBBERLIST, "r7", "r8");      \
        _num;                                                             \
    })

#define my_syscall5(num, arg1, arg2, arg3, arg4, arg5)                    \
    ({                                                                    \
        register unsigned long _num __asm__("r11") = (num);               \
        register unsigned long _arg1 __asm__("r3") = (long) (arg1);       \
        register unsigned long _arg2 __asm__("r4") = (long) (arg2);       \
        register unsigned long _arg3 __asm__("r5") = (long) (arg3);       \
        register unsigned long _arg4 __asm__("r6") = (long) (arg4);       \
        register unsigned long _arg5 __asm__("r7") = (long) (arg5);       \
                                                                          \
        __asm__ volatile("l.sys 1"                                        \
                         : "=r"(_num)                                     \
                         : "r"(_num), "r"(_arg1), "r"(_arg2), "r"(_arg3), \
                           "r"(_arg4), "r"(_arg5)                         \
                         : _NOLIBC_SYSCALL_CLOBBERLIST, "r8");            \
        _num;                                                             \
    })

#define my_syscall6(num, arg1, arg2, arg3, arg4, arg5, arg6)              \
    ({                                                                    \
        register unsigned long _num __asm__("r11") = (num);               \
        register unsigned long _arg1 __asm__("r3") = (long) (arg1);       \
        register unsigned long _arg2 __asm__("r4") = (long) (arg2);       \
        register unsigned long _arg3 __asm__("r5") = (long) (arg3);       \
        register unsigned long _arg4 __asm__("r6") = (long) (arg4);       \
        register unsigned long _arg5 __asm__("r7") = (long) (arg5);       \
        register unsigned long _arg6 __asm__("r8") = (long) (arg6);       \
                                                                          \
        __asm__ volatile("l.sys 1"                                        \
                         : "=r"(_num)                                     \
                         : "r"(_num), "r"(_arg1), "r"(_arg2), "r"(_arg3), \
                           "r"(_arg4), "r"(_arg5), "r"(_arg6)             \
                         : _NOLIBC_SYSCALL_CLOBBERLIST);                  \
        _num;                                                             \
    })

/* startup code */
void __attribute__((weak, noreturn)) __nolibc_entrypoint __no_stack_protector
_start(void)
{
    __asm__ volatile("l.ori r2, r0, 0\n"
                     "l.ori r3, r1, 0\n"
                     "l.jal _start_c\n"
                     "l.nop\n");
    __nolibc_entrypoint_epilogue();
}

#endif /* _NOLIBC_ARCH_OR1K_H */
