/* SPDX-License-Identifier: LGPL-2.1 OR MIT */
/*
 * PowerPC save/restore functions for NOLIBC.
 * Adapted from crtsavres.S and rewritten in inline assembly.
 *
 * Copyright (C) 2025 MikeWang000000
 */

/*-
 * SPDX-License-Identifier: BSD-1-Clause
 *
 * Copyright 2019 Justin Hibbits
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * The PowerPC ABI spec requires the following save/restore functions to be
 * provided:
 *
 * _savefpr_N
 * _restfpr_N
 * _restfpr_N_x
 * _savegpr_N
 * _restgpr_N
 * _restgpr_N_x
 *
 * With N ranging from 14 to 31, to save the nonvolatile registers.
 */

#ifndef _NOLIBC_ARCH_POWERPC_CRTSAVRES_H
#define _NOLIBC_ARCH_POWERPC_CRTSAVRES_H

#if defined(__powerpc__) && !defined(__powerpc64__)

#define __STR(a)           #a
#define __CONCAT2(a, b)    __STR(a) __STR(b)
#define __CONCAT3(a, b, c) __STR(a) __STR(b) __STR(c)

#define _CRTENTRY(name) \
    ".text\n"           \
    ".globl " name "\n" \
    ".weak " name "\n"  \
    ".type " name ",@function\n" name ":\n"

#define SAVEFPR(r)                     \
    _CRTENTRY(__CONCAT2(_savefpr_, r)) \
    "stfd " #r ",(-256 + " #r " * 8)(11)\n"

#define RESTFPR(r)                     \
    _CRTENTRY(__CONCAT2(_restfpr_, r)) \
    "lfd " #r ",(-256 + " #r " * 8)(11)\n"

#define SAVEGPR(r)                     \
    _CRTENTRY(__CONCAT2(_savegpr_, r)) \
    "stw " #r ",(-128 + " #r " * 4)(11)\n"

#define RESTGPR(r)                     \
    _CRTENTRY(__CONCAT2(_restgpr_, r)) \
    "lwz " #r ",(-128 + " #r " * 4)(11)\n"

#define RESTFPR_X(r)                       \
    _CRTENTRY(__CONCAT3(_restfpr_, r, _x)) \
    "lfd " #r ",(-256 + " #r " * 8)(11)\n"

#define RESTGPR_X(r)                       \
    _CRTENTRY(__CONCAT3(_restgpr_, r, _x)) \
    "lwz " #r ",(-128 + " #r " * 4)(11)\n"

__asm__(SAVEFPR(14) SAVEFPR(15) SAVEFPR(16) SAVEFPR(17) SAVEFPR(18) SAVEFPR(19)
            SAVEFPR(20) SAVEFPR(21) SAVEFPR(22) SAVEFPR(23) SAVEFPR(24)
                SAVEFPR(25) SAVEFPR(26) SAVEFPR(27) SAVEFPR(28) SAVEFPR(29)
                    SAVEFPR(30) SAVEFPR(31) "blr\n"

        RESTFPR(14) RESTFPR(15) RESTFPR(16) RESTFPR(17) RESTFPR(18) RESTFPR(19)
            RESTFPR(20) RESTFPR(21) RESTFPR(22) RESTFPR(23) RESTFPR(24)
                RESTFPR(25) RESTFPR(26) RESTFPR(27) RESTFPR(28) RESTFPR(29)
                    RESTFPR(30) RESTFPR(31) "blr\n"

        SAVEGPR(14) SAVEGPR(15) SAVEGPR(16) SAVEGPR(17) SAVEGPR(18) SAVEGPR(19)
            SAVEGPR(20) SAVEGPR(21) SAVEGPR(22) SAVEGPR(23) SAVEGPR(24)
                SAVEGPR(25) SAVEGPR(26) SAVEGPR(27) SAVEGPR(28) SAVEGPR(29)
                    SAVEGPR(30) SAVEGPR(31) "blr\n"

        RESTGPR(14) RESTGPR(15) RESTGPR(16) RESTGPR(17) RESTGPR(18) RESTGPR(19)
            RESTGPR(20) RESTGPR(21) RESTGPR(22) RESTGPR(23) RESTGPR(24)
                RESTGPR(25) RESTGPR(26) RESTGPR(27) RESTGPR(28) RESTGPR(29)
                    RESTGPR(30) RESTGPR(31) "blr\n"

        RESTFPR_X(14) RESTFPR_X(15) RESTFPR_X(16) RESTFPR_X(17) RESTFPR_X(18)
            RESTFPR_X(19) RESTFPR_X(20) RESTFPR_X(21) RESTFPR_X(22)
                RESTFPR_X(23) RESTFPR_X(24) RESTFPR_X(25) RESTFPR_X(26)
                    RESTFPR_X(27) RESTFPR_X(28) RESTFPR_X(29) RESTFPR_X(30)
                        RESTFPR_X(31) "lwz    0,4(11)\n"
                                      "mtlr    0\n"
                                      "mr    1,11\n"
                                      "blr\n"

        RESTGPR_X(14) RESTGPR_X(15) RESTGPR_X(16) RESTGPR_X(17) RESTGPR_X(18)
            RESTGPR_X(19) RESTGPR_X(20) RESTGPR_X(21) RESTGPR_X(22)
                RESTGPR_X(23) RESTGPR_X(24) RESTGPR_X(25) RESTGPR_X(26)
                    RESTGPR_X(27) RESTGPR_X(28) RESTGPR_X(29) RESTGPR_X(30)
                        RESTGPR_X(31) "lwz    0,4(11)\n"
                                      "mtlr    0\n"
                                      "mr    1,11\n"
                                      "blr\n");

#undef __STR
#undef __CONCAT2
#undef __CONCAT3
#undef _CRTENTRY
#undef SAVEFPR
#undef RESTFPR
#undef SAVEGPR
#undef RESTGPR
#undef RESTFPR_X
#undef RESTGPR_X

#endif /* defined(__powerpc__) && !defined(__powerpc64__) */

#endif /* _NOLIBC_ARCH_POWERPC_CRTSAVRES_H */
