/* SPDX-License-Identifier: MIT */
/*
 * LXPACK: a Linux eXecutable PACKer.
 * Copyright (C) 2025 MikeWang000000
 */
#ifdef NOSTDLIB
#include "cstd/all.h"
#else
#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#endif /* NOSTDLIB */
#include "lzmad.h"

extern unsigned char payload_lzma[];
extern unsigned int payload_len, payload_lzma_len;

static const int k_tempfd = 98;
static const int k_execfd = 99;
static const char *const k_tempfd_path = "/proc/self/fd/98";
static const char *const k_execfd_path = "/proc/self/fd/99";


static int open_tempfd_compat(void)
{
    int res, fd;
    long pid;
    size_t i;
    char path[64] = "/dev/shm/.tempzz";

    pid = getpid();
    for (i = 16; pid > 0; pid >>= 4, i++) {
        path[i] = 'a' + (pid & 0x0f);
    }
    path[i] = 0;

    unlinkat(AT_FDCWD, path, 0);
    fd = openat(AT_FDCWD, path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        return -1;
    }

    res = unlinkat(AT_FDCWD, path, 0);
    if (res < 0) {
        close(fd);
        return -1;
    }

    res = dup2(fd, k_tempfd);
    close(fd);
    if (res < 0) {
        return -1;
    }

    return k_tempfd;
}


static int open_execfd_compat(int cloexec)
{
    int res, fd;

    res = fchmod(k_tempfd, S_IRUSR | S_IXUSR);
    if (res < 0) {
        return -1;
    }

    fd = openat(AT_FDCWD, k_tempfd_path, O_RDONLY, 0);
    if (fd < 0) {
        return -1;
    }

    res = dup2(fd, k_execfd);
    close(fd);
    if (res < 0) {
        return -1;
    }

    if (cloexec) {
        res = fcntl(k_execfd, F_SETFD, FD_CLOEXEC);
        if (res < 0) {
            close(k_execfd);
            return -1;
        }
    }

    return k_execfd;
}


static int open_tempfd(void)
{
    int fd, res;

    fd = memfd_create("", MFD_ALLOW_SEALING);
    if (fd < 0) {
        return -1;
    }

    res = dup2(fd, k_tempfd);
    close(fd);
    if (res < 0) {
        return -1;
    }

    return k_tempfd;
}


static int open_execfd(int cloexec)
{
    int res;

    res = dup2(k_tempfd, k_execfd);
    if (res < 0) {
        return -1;
    }

    res = fcntl(k_execfd, F_ADD_SEALS,
                F_SEAL_SEAL | F_SEAL_SHRINK | F_SEAL_GROW | F_SEAL_WRITE);
    if (res < 0) {
        return -1;
    }

    if (cloexec) {
        res = fcntl(k_execfd, F_SETFD, FD_CLOEXEC);
        if (res < 0) {
            close(k_execfd);
            return -1;
        }
    }

    return k_execfd;
}


static ssize_t decompress(int fd, size_t original_size, void *compressed,
                          size_t compressed_size)
{
    int res;
    void *out;
    size_t out_size;

    res = ftruncate(fd, original_size);
    if (res < 0) {
        return -1;
    }

    out = mmap(0, original_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (out == MAP_FAILED) {
        return -1;
    }

    out_size = original_size;
    res = lzma_decompress(compressed, compressed_size, out, &out_size);
    if (res) {
        munmap(out, original_size);
        return -1;
    }

    res = msync(out, original_size, MS_SYNC);
    munmap(out, original_size);
    if (res < 0) {
        return -1;
    }

    return out_size;
}


int main(int argc, char *argv[], char *envp[])
{
    int res, err;
    int compat, cloexec;
    ssize_t outsize;

    (void) argc;

    cloexec = 1;

begin:
    compat = 0;
    res = open_tempfd();
    if (res < 0) {
        compat = 1;
        res = open_tempfd_compat();
        if (res < 0) {
            return 1;
        }
    }

    outsize = decompress(k_tempfd, payload_len, payload_lzma,
                         payload_lzma_len);
    if (outsize < 0) {
        close(k_tempfd);
        return 2;
    }

    res = compat ? open_execfd_compat(cloexec) : open_execfd(cloexec);
    close(k_tempfd);
    if (res < 0) {
        return 3;
    }

    res = execve(k_execfd_path, argv, envp);
#ifdef NOSTDLIB
    err = -res;
#else
    err = errno;
#endif /* NOSTDLIB */
    close(k_execfd);

    if (cloexec && err == ENOENT) {
        cloexec = 0;
        goto begin;
    }

    return 4;
}
