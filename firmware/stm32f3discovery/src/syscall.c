// badprog.com
/**
 * @file    syscall.c
 * @brief   Minimal newlib syscall stubs for bare metal.
 *          newlib requires these OS-level hooks to link successfully.
 *          Since there is no OS, we provide no-op implementations.
 */

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdint.h>

int  _close(int file)                              { (void)file; return -1; }
int  _fstat(int file, struct stat *st)             { (void)file; st->st_mode = S_IFCHR; return 0; }
int  _isatty(int file)                             { (void)file; return 1; }
int  _lseek(int file, int ptr, int dir)            { (void)file; (void)ptr; (void)dir; return 0; }
int  _open(const char *name, int flags, int mode)  { (void)name; (void)flags; (void)mode; return -1; }
int  _read(int file, char *ptr, int len)           { (void)file; (void)ptr; (void)len; return 0; }
int  _write(int file, char *ptr, int len)          { (void)file; (void)ptr; return len; }
void _exit(int status)                             { (void)status; while (1) {} }
int  _kill(int pid, int sig)                       { (void)pid; (void)sig; errno = EINVAL; return -1; }
int  _getpid(void)                                 { return 1; }
