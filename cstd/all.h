/* SPDX-License-Identifier: LGPL-2.1 OR MIT */
/*
 * All standard C / POSIX functions used by lxpack.
 * Copyright (C) 2025 MikeWang000000
 */
#ifndef _NOLIBC_ALL_H
#define _NOLIBC_ALL_H

#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/memfd.h>
#include <linux/mman.h>
#include <linux/stat.h>
#include <linux/unistd.h>

#include "arch.h"
#include "crt.h"
#include "inttypes.h"

__attribute__((weak)) void *memset(void *s, int c, size_t n)
{
    char *p = s;

    while (n--) {
        /* prevent gcc from recognizing memset() here */
        __asm__ volatile("");
        *(p++) = c;
    }
    return s;
}

__attribute__((weak)) void *memcpy(void *dest, const void *src, size_t n)
{
    size_t pos = 0;

    while (pos < n) {
        /* prevent gcc from recognizing memcpy() here */
        __asm__ volatile("");
        ((char *) dest)[pos] = ((const char *) src)[pos];
        pos++;
    }
    return dest;
}

__attribute__((weak)) void raise(int)
{
    __builtin_trap();
}

#define MAP_FAILED ((void *) -1)

#define close(fd) my_syscall1(__NR_close, (fd))

#if defined(__NR_dup2)
#define dup2(oldfd, newfd) my_syscall2(__NR_dup2, (oldfd), (newfd))
#else
#define dup2(oldfd, newfd) my_syscall3(__NR_dup3, (oldfd), (newfd), 0)
#endif

#define execve(pathname, argv, envp) \
    my_syscall3(__NR_execve, (pathname), (argv), (envp))

#define fchmod(fd, mode) my_syscall2(__NR_fchmod, (fd), (mode))

#if defined(__NR_fcntl)
#define fcntl(fd, op, arg) my_syscall3(__NR_fcntl, (fd), (op), (arg))
#else
#define fcntl(fd, op, arg) my_syscall3(__NR_fcntl64, (fd), (op), (arg))
#endif

#if defined(__NR_ftruncate)
#define ftruncate(fd, length) my_syscall2(__NR_ftruncate, (fd), (length))
#else
#define ftruncate(fd, length) my_syscall2(__NR_ftruncate64, (fd), (length))
#endif

#define getpid() my_syscall0(__NR_getpid)

#define memfd_create(name, flags) \
    my_syscall2(__NR_memfd_create, (name), (flags))

#if defined(__NR_mmap2)
#define mmap(addr, length, prot, flags, fd, offset)                   \
    ({                                                                \
        long _ret = my_syscall6(__NR_mmap2, (addr), (length), (prot), \
                                (flags), (fd), (offset) >> 12);       \
        (_ret < 0 && _ret >= -4095) ? MAP_FAILED : (void *) _ret;     \
    })
#elif defined(__s390__) || defined(__s390x__)
#define mmap(addr, length, prot, flags, fd, offset)                        \
    ({                                                                     \
        long _args[] = {(long) (addr),  (long) (length), (long) (prot),    \
                        (long) (flags), (long) (fd),     (long) (offset)}; \
        long _ret = my_syscall1(__NR_mmap, (long) _args);                  \
        (_ret < 0 && _ret >= -4095) ? MAP_FAILED : (void *) _ret;          \
    })
#else
#define mmap(addr, length, prot, flags, fd, offset)                           \
    ({                                                                        \
        long _ret = my_syscall6(__NR_mmap, (addr), (length), (prot), (flags), \
                                (fd), (offset) >> 12);                        \
        (_ret < 0 && _ret >= -4095) ? MAP_FAILED : (void *) _ret;             \
    })
#endif

#define msync(addr, length, flags) \
    my_syscall3(__NR_msync, (addr), (length), (flags))

#define munmap(addr, length) my_syscall2(__NR_munmap, (addr), (length))

#define openat(dirfd, pathname, flags, mode) \
    my_syscall4(__NR_openat, (dirfd), (pathname), (flags), (mode))

#define unlinkat(dirfd, pathname, flags) \
    my_syscall3(__NR_unlinkat, (dirfd), (pathname), (flags))

#endif /* _NOLIBC_ALL_H */
