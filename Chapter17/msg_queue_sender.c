#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

#define MAXMSZ 512

struct mymesg {
    long mtype;
    char mtext[MAXMSZ];
};

int main(int argc, char* argv[]) {
    key_t key;
    long qid;
    size_t nbytes;
    struct mymesg msg;
    if (argc != 3) {
        fprintf(stderr, "usage: %s KEY message\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    key = strtol(argv[1], NULL, 0);
    if ((qid = msgget(key, 0)) < 0)
        fprintf(stderr, "can't open queue key %s", argv[1]);
    memset(&msg, 0, sizeof(msg));
    strncpy(msg.mtext, argv[2], MAXMSZ - 1);
    nbytes = strlen(msg.mtext);
    msg.mtype = 1;
    if (msgsnd(qid, &msg, nbytes, 0) < 0)
        perror("can't send message");
    return 0;
}
