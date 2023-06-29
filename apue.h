#ifndef _APUE_H
#define _APUE_H

#define _POSIX_C_SOURCE 200809L

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/termios.h>  // for winsize

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>  // for offsetof
#include <string.h>
#include <unistd.h>
#include <signal.h>  // for SIG_ERR

#define MAXLINE 4096

// Default file access permisions for new files.
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

// Default permisions for new directory
#define DIR_MODE (FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

typedef void Sigfunc(int);  // for signal handlers

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

// Prototype for our own functions.

#endif  // _APUE_H
