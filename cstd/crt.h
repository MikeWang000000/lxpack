/* SPDX-License-Identifier: LGPL-2.1 OR MIT */
/*
 * C Run Time support for NOLIBC
 * Copyright (C) 2023 Zhangjin Wu <falcon@tinylab.org>
 * Copyright (C) 2025 MikeWang000000
 */

#ifndef _NOLIBC_CRT_H
#define _NOLIBC_CRT_H

#include "arch.h"

void _start(void);

void _start_c(long *sp);
__attribute__((weak,used))
#if __nolibc_has_feature(undefined_behavior_sanitizer)
__attribute__((no_sanitize("function")))
#endif
void _start_c(long *sp)
{
    long argc;
    char **argv;
    char **envp;
    int exitcode;

    /* silence potential warning: conflicting types for 'main' */
    int _main(int, char **, char **) __asm__("main");

    argc = *sp;
    argv = (void *) (sp + 1);
    envp = argv + argc + 1;

    exitcode = _main(argc, argv, envp);

    my_syscall1(__NR_exit, exitcode & 255);
}

#endif /* _NOLIBC_CRT_H */
