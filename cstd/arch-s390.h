/* SPDX-License-Identifier: LGPL-2.1 OR MIT */
/*
 * s390 specific definitions for NOLIBC
 */

#ifndef _NOLIBC_ARCH_S390_H
#define _NOLIBC_ARCH_S390_H

#include "compiler.h"

/* Syscalls for s390:
 *   - registers are 64-bit
 *   - syscall number is passed in r1
 *   - arguments are in r2-r7
 *   - the system call is performed by calling the svc instruction
 *   - syscall return value is in r2
 *   - r1 and r2 are clobbered, others are preserved.
 *
 * Link s390 ABI: https://github.com/IBM/s390x-abi
 *
 */

#define my_syscall0(num)                                                      \
    ({                                                                        \
        register long _num __asm__("1") = (num);                              \
        register long _rc __asm__("2");                                       \
                                                                              \
        __asm__ volatile("svc 0\n" : "=d"(_rc) : "d"(_num) : "memory", "cc"); \
        _rc;                                                                  \
    })

#define my_syscall1(num, arg1)                            \
    ({                                                    \
        register long _num __asm__("1") = (num);          \
        register long _arg1 __asm__("2") = (long) (arg1); \
                                                          \
        __asm__ volatile("svc 0\n"                        \
                         : "+d"(_arg1)                    \
                         : "d"(_num)                      \
                         : "memory", "cc");               \
        _arg1;                                            \
    })

#define my_syscall2(num, arg1, arg2)                      \
    ({                                                    \
        register long _num __asm__("1") = (num);          \
        register long _arg1 __asm__("2") = (long) (arg1); \
        register long _arg2 __asm__("3") = (long) (arg2); \
                                                          \
        __asm__ volatile("svc 0\n"                        \
                         : "+d"(_arg1)                    \
                         : "d"(_arg2), "d"(_num)          \
                         : "memory", "cc");               \
        _arg1;                                            \
    })

#define my_syscall3(num, arg1, arg2, arg3)                   \
    ({                                                       \
        register long _num __asm__("1") = (num);             \
        register long _arg1 __asm__("2") = (long) (arg1);    \
        register long _arg2 __asm__("3") = (long) (arg2);    \
        register long _arg3 __asm__("4") = (long) (arg3);    \
                                                             \
        __asm__ volatile("svc 0\n"                           \
                         : "+d"(_arg1)                       \
                         : "d"(_arg2), "d"(_arg3), "d"(_num) \
                         : "memory", "cc");                  \
        _arg1;                                               \
    })

#define my_syscall4(num, arg1, arg2, arg3, arg4)                         \
    ({                                                                   \
        register long _num __asm__("1") = (num);                         \
        register long _arg1 __asm__("2") = (long) (arg1);                \
        register long _arg2 __asm__("3") = (long) (arg2);                \
        register long _arg3 __asm__("4") = (long) (arg3);                \
        register long _arg4 __asm__("5") = (long) (arg4);                \
                                                                         \
        __asm__ volatile("svc 0\n"                                       \
                         : "+d"(_arg1)                                   \
                         : "d"(_arg2), "d"(_arg3), "d"(_arg4), "d"(_num) \
                         : "memory", "cc");                              \
        _arg1;                                                           \
    })

#define my_syscall5(num, arg1, arg2, arg3, arg4, arg5)                     \
    ({                                                                     \
        register long _num __asm__("1") = (num);                           \
        register long _arg1 __asm__("2") = (long) (arg1);                  \
        register long _arg2 __asm__("3") = (long) (arg2);                  \
        register long _arg3 __asm__("4") = (long) (arg3);                  \
        register long _arg4 __asm__("5") = (long) (arg4);                  \
        register long _arg5 __asm__("6") = (long) (arg5);                  \
                                                                           \
        __asm__ volatile("svc 0\n"                                         \
                         : "+d"(_arg1)                                     \
                         : "d"(_arg2), "d"(_arg3), "d"(_arg4), "d"(_arg5), \
                           "d"(_num)                                       \
                         : "memory", "cc");                                \
        _arg1;                                                             \
    })

#define my_syscall6(num, arg1, arg2, arg3, arg4, arg5, arg6)               \
    ({                                                                     \
        register long _num __asm__("1") = (num);                           \
        register long _arg1 __asm__("2") = (long) (arg1);                  \
        register long _arg2 __asm__("3") = (long) (arg2);                  \
        register long _arg3 __asm__("4") = (long) (arg3);                  \
        register long _arg4 __asm__("5") = (long) (arg4);                  \
        register long _arg5 __asm__("6") = (long) (arg5);                  \
        register long _arg6 __asm__("7") = (long) (arg6);                  \
                                                                           \
        __asm__ volatile("svc 0\n"                                         \
                         : "+d"(_arg1)                                     \
                         : "d"(_arg2), "d"(_arg3), "d"(_arg4), "d"(_arg5), \
                           "d"(_arg6), "d"(_num)                           \
                         : "memory", "cc");                                \
        _arg1;                                                             \
    })

/* startup code */
void __attribute__((weak, noreturn)) __nolibc_entrypoint __no_stack_protector
_start(void)
{
    __asm__ volatile(
#ifdef __s390x__
        "lgr %r2, %r15\n" /* save stack pointer to %r2, as arg1 of _start_c */
        "aghi %r15, -160\n" /* allocate new stackframe */
#else
        "lr %r2, %r15\n"
        "ahi %r15, -96\n"
#endif
        "xc 0(8,%r15), 0(%r15)\n" /* clear backchain */
        "brasl %r14, _start_c\n"  /* transfer to c runtime */
    );
    __nolibc_entrypoint_epilogue();
}

#endif /* _NOLIBC_ARCH_S390_H */
