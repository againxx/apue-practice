#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>
#include <stddef.h>

#define STALE 30 // client's name cannot be older than this (sec)

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
    if ((name = malloc(sizeof(un.sun_path) + 1)) == NULL)
        return -1;
    len = sizeof(un);
    if ((clifd = accept(listenfd, (struct sockaddr*)&un, &len)) < 0) {
        free(name);
        return -2;
    }

    // obtain the client's uid from its calling address
    len -= offsetof(struct sockaddr_un, sun_path);
}
