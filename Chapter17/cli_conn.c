#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <stddef.h>

#define CLI_PATH "/var/tmp"
#define CLI_PERM S_IRWXU // rwx for user only

int cli_conn(const char* name) {
    int fd, err, rval;
    socklen_t len;
    struct sockaddr_un un, sun;
    int do_unlink = 0;

    if (strlen(name) >= sizeof(un.sun_path)) {
        errno = ENAMETOOLONG;
        return -1;
    }

    // create a UNIX domain stream socket
    if ((fd = socket(PF_LOCAL, SOCK_STREAM, 0)) < 0)
        return -1;

    memset(&un, 0, sizeof(un));
    un.sun_family = PF_LOCAL;
    sprintf(un.sun_path, "%s%05ld", CLI_PATH, (long)getpid());
    len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);

    unlink(un.sun_path); // in case it already exists
    if (bind(fd, (struct sockaddr*)&un, len) < 0) {
        rval = -2;
        goto errout;
    }

    if (chmod(un.sun_path, CLI_PERM) < 0) {
        rval = -3;
        do_unlink = 1; // we have already create named socket file
        goto errout;
    }

    // fill socket address structure with server's address
    memset(&sun, 0, sizeof(sun));
    sun.sun_family = PF_LOCAL;
    strcpy(sun.sun_path, name);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(sun.sun_path);
    if (connect(fd, (struct sockaddr*)&sun, len) < 0) {
        rval = -4;
        do_unlink = 1;
        goto errout;
    }
    return fd;

errout:
    err = errno;
    close(fd);
    if (do_unlink)
        unlink(un.sun_path);
    errno = err;
    return rval;
}
