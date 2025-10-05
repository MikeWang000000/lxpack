/* SPDX-License-Identifier: LGPL-2.1 OR MIT */
/*
 * MicroBlaze specific definitions for NOLIBC
 * Copyright (C) 2025 MikeWang000000
 */

#ifndef _NOLIBC_ARCH_MICROBLAZE_H
#define _NOLIBC_ARCH_MICROBLAZE_H

#include "compiler.h"

#define _NOLIBC_SYSCALL_CLOBBERLIST "memory", "r4"

#define my_syscall0(num)                                 \
    ({                                                   \
        register long _num __asm__("r12") = (num);       \
        register long _ret __asm__("r3");                \
                                                         \
        __asm__ volatile("brki r14, 0x8\n"               \
                         : "=r"(_ret)                    \
                         : "r"(_num)                     \
                         : _NOLIBC_SYSCALL_CLOBBERLIST); \
        _ret;                                            \
    })

#define my_syscall1(num, arg1)                             \
    ({                                                     \
        register long _num __asm__("r12") = (num);         \
        register long _arg1 __asm__("r5") = (long) (arg1); \
        register long _ret __asm__("r3");                  \
                                                           \
        __asm__ volatile("brki r14, 0x8\n"                 \
                         : "=r"(_ret)                      \
                         : "r"(_num), "r"(_arg1)           \
                         : _NOLIBC_SYSCALL_CLOBBERLIST);   \
        _ret;                                              \
    })

#define my_syscall2(num, arg1, arg2)                         \
    ({                                                       \
        register long _num __asm__("r12") = (num);           \
        register long _arg1 __asm__("r5") = (long) (arg1);   \
        register long _arg2 __asm__("r6") = (long) (arg2);   \
        register long _ret __asm__("r3");                    \
                                                             \
        __asm__ volatile("brki r14, 0x8\n"                   \
                         : "=r"(_ret)                        \
                         : "r"(_num), "r"(_arg1), "r"(_arg2) \
                         : _NOLIBC_SYSCALL_CLOBBERLIST);     \
        _ret;                                                \
    })

#define my_syscall3(num, arg1, arg2, arg3)                               \
    ({                                                                   \
        register long _num __asm__("r12") = (num);                       \
        register long _arg1 __asm__("r5") = (long) (arg1);               \
        register long _arg2 __asm__("r6") = (long) (arg2);               \
        register long _arg3 __asm__("r7") = (long) (arg3);               \
        register long _ret __asm__("r3");                                \
                                                                         \
        __asm__ volatile("brki r14, 0x8\n"                               \
                         : "=r"(_ret)                                    \
                         : "r"(_num), "r"(_arg1), "r"(_arg2), "r"(_arg3) \
                         : _NOLIBC_SYSCALL_CLOBBERLIST);                 \
        _ret;                                                            \
    })

#define my_syscall4(num, arg1, arg2, arg3, arg4)                          \
    ({                                                                    \
        register long _num __asm__("r12") = (num);                        \
        register long _arg1 __asm__("r5") = (long) (arg1);                \
        register long _arg2 __asm__("r6") = (long) (arg2);                \
        register long _arg3 __asm__("r7") = (long) (arg3);                \
        register long _arg4 __asm__("r8") = (long) (arg4);                \
        register long _ret __asm__("r3");                                 \
                                                                          \
        __asm__ volatile("brki r14, 0x8\n"                                \
                         : "=r"(_ret)                                     \
                         : "r"(_num), "r"(_arg1), "r"(_arg2), "r"(_arg3), \
                           "r"(_arg4)                                     \
                         : _NOLIBC_SYSCALL_CLOBBERLIST);                  \
        _ret;                                                             \
    })

#define my_syscall5(num, arg1, arg2, arg3, arg4, arg5)                    \
    ({                                                                    \
        register long _num __asm__("r12") = (num);                        \
        register long _arg1 __asm__("r5") = (long) (arg1);                \
        register long _arg2 __asm__("r6") = (long) (arg2);                \
        register long _arg3 __asm__("r7") = (long) (arg3);                \
        register long _arg4 __asm__("r8") = (long) (arg4);                \
        register long _arg5 __asm__("r9") = (long) (arg5);                \
        register long _ret __asm__("r3");                                 \
                                                                          \
        __asm__ volatile("brki r14, 0x8\n"                                \
                         : "=r"(_ret)                                     \
                         : "r"(_num), "r"(_arg1), "r"(_arg2), "r"(_arg3), \
                           "r"(_arg4), "r"(_arg5)                         \
                         : _NOLIBC_SYSCALL_CLOBBERLIST);                  \
        _ret;                                                             \
    })

#define my_syscall6(num, arg1, arg2, arg3, arg4, arg5, arg6)              \
    ({                                                                    \
        register long _num __asm__("r12") = (num);                        \
        register long _arg1 __asm__("r5") = (long) (arg1);                \
        register long _arg2 __asm__("r6") = (long) (arg2);                \
        register long _arg3 __asm__("r7") = (long) (arg3);                \
        register long _arg4 __asm__("r8") = (long) (arg4);                \
        register long _arg5 __asm__("r9") = (long) (arg5);                \
        register long _arg6 __asm__("r10") = (long) (arg6);               \
        register long _ret __asm__("r3");                                 \
                                                                          \
        __asm__ volatile("brki r14, 0x8\n"                                \
                         : "=r"(_ret)                                     \
                         : "r"(_num), "r"(_arg1), "r"(_arg2), "r"(_arg3), \
                           "r"(_arg4), "r"(_arg5), "r"(_arg6)             \
                         : _NOLIBC_SYSCALL_CLOBBERLIST);                  \
        _ret;                                                             \
    })

/* startup code */
void __attribute__((weak, noreturn)) __nolibc_entrypoint __no_stack_protector
_start(void)
{
    __asm__ volatile("add   r19, r0, r0\n"
                     "ori   r5,  r1, 0\n"
                     "andi  r1,  r1, -8\n"
                     "addik r1,  r1, -8\n"
                     "brlid r15, _start_c\n"
                     "nop\n");
    __nolibc_entrypoint_epilogue();
}

#endif /* _NOLIBC_ARCH_MICROBLAZE_H */
