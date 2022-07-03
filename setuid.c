#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

void printUIDs() {
    uid_t uid = getuid();
    uid_t euid = geteuid();
    printf("Current uid: %d, euid: %d\n", uid, euid);
}

int main(void) {
    printUIDs();

    uid_t uid = getuid();
    uid_t euid = geteuid();
    if (uid == euid) {
        puts("Two uids are the same, not thing to do!\n");
        exit(EXIT_SUCCESS);
    }

    if (seteuid(uid) < 0) {
        fprintf(stderr, "Current uid: %d, euid: %d, %s\n", uid, euid, strerror(errno));
    } else {
        printf("seteuid() uid to %d\n", uid);
        printUIDs();
    }

    if (seteuid(euid) < 0) {
        fprintf(stderr, "Current uid: %d, euid: %d, %s\n", uid, euid, strerror(errno));
    } else {
        printf("seteuid() euid to %d\n", euid);
        printUIDs();
    }

    if (setuid(uid) < 0) {
        fprintf(stderr, "Current uid: %d, euid: %d, %s\n", uid, euid, strerror(errno));
    } else {
        printf("setuid() uid to %d\n", uid);
        printUIDs();
    }

    if (seteuid(euid) < 0) {
        fprintf(stderr, "Current uid: %d, euid: %d, %s\n", uid, euid, strerror(errno));
    } else {
        printf("seteuid() euid to %d\n", euid);
        printUIDs();
    }

    return 0;
}
