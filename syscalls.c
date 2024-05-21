#include "project.h"
#include <unistd.h>

int _write(int fd, const char *data, int len)
{
    return -1;
}

int _read(int fd, char *data, int len)
{
    return -1;
}

int _lseek(int fd, off_t offset, int whence)
{
    return -1;
}

int _isatty(int fd)
{
    return fd >= 0 && fd <= 2;
}

int _close(int fd)
{
    return -1;
}

int _fstat(int fd, void *buf)
{
    return -1;
}
