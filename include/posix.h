#ifndef __schism_posix_h
#define __schism_posix_h

// header file giving access to posix APIs
// whichever file is including this is making use of POSIX directly. Is it wise? Should they go
// through a simpler (and thus more portable) platform layer? Have you considered portability issues?
//
// Windows supports posix, to a limit.

#include <fcntl.h>
#include <stdio.h> // for fdopen
#include <sys/stat.h>
#include <stdlib.h>

#ifdef WIN32
// Windows puts all the posix names outside of the C standard namespace. We bring them back
// in using macros.

#include <direct.h> // _getcwd
#include <io.h> // _open, _close, _unlink
#include <ctype.h> // _tolower

#define chdir _chdir
#define getcwd _getcwd

#define strdup _strdup

#define close _close
#define open _open
#define unlink _unlink

#define fdopen _fdopen
#define mkdir(__path, __mode) _mkdir(__path)
#define chmod _chmod

#define putenv _putenv

#define S_ISDIR(__st_mode) (__st_mode & S_IFDIR)
#define S_ISREG(__st_mode) (__st_mode & S_IFREG)

static inline int strcasecmp(const char* s1, const char* s2)
{
  while (*s1 && *s2 && _tolower(*s1) == _tolower(*s2)) {
    ++s1;
    ++s2;
  }
  return *s1 == '\0' && *s2 == '\0';
}

static inline int strncasecmp(const char* s1, const char* s2, size_t n)
{
  while (n && *s2 && _tolower(*s1) == _tolower(*s2)) {
    ++s1;
    --n;
    ++s2;
  }
  return n == 0 && *s2 == '\0';
}

#else
#include <unistd.h>
#endif

#endif