#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

#define STALE 30  // client's name cannot be older than this (sec)

/*
 * Wait for a client connection to arrive, and accept it
 * We also obtain the client's user ID from the pathname
 * that it must bind before calling us.
 * Returns new fd if all OK, <0 on error
 */
int serv_accept(int listenfd, uid_t* uidptr) {
    int clifd, err, rval;
    socklen_t len;
    time_t staletime;
    struct sockaddr_un un;
    struct stat statbuf;
    char* name;

    // allocate enough space for longest name plus terminating null
    if ((name = malloc(sizeof(un.sun_path) + 1)) == NULL) return -1;
    len = sizeof(un);
    if ((clifd = accept(listenfd, (struct sockaddr*)&un, &len)) < 0) {
        free(name);
        return -2;
    }

    // obtain the client's uid from its calling address
    len -= offsetof(struct sockaddr_un, sun_path);
    memcpy(name, un.sun_path, len);
    name[len] = 0;
    if (stat(name, &statbuf) < 0) {
        rval = -3;
        goto errout;
    }

#ifdef S_ISSOCK
    if (S_ISSOCK(statbuf.st_mode) == 0) {
        rval = -4;
        goto errout;
    }
#endif

    if ((statbuf.st_mode & (S_IRWXG | S_IRWXO)) ||
        (statbuf.st_mode & S_IRWXU) != S_IRWXU) {
        rval = -5;  // if not rwx------
        goto errout;
    }

    staletime = time(NULL) - STALE;
    if (statbuf.st_atime < staletime || statbuf.st_ctime < staletime ||
        statbuf.st_mtime < staletime) {
        rval = -6;
        goto errout;
    }

    if (uidptr != NULL) {
        *uidptr = statbuf.st_uid;
    }
    unlink(name);
    free(name);
    return clifd;

errout:
    err = errno;
    close(clifd);
    free(name);
    errno = err;
    return rval;
}
