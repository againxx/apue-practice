#include <stdio.h>
#include <poll.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <unistd.h>

#define NQ       3
#define MAXMSZ   512
#define KEY      0x123

struct threadinfo {
    int qid;
    int fd;
};

struct mymesg {
    long mtype;
    char mtext[MAXMSZ];
};

void* helper(void* arg) {
    int n;
    struct mymesg m;
    struct threadinfo* tip = arg;

    for (;;) {
        memset(&m, 0, sizeof(m));
        if ((n = msgrcv(tip->qid, &m, MAXMSZ, 0, MSG_NOERROR)) < 0)
            perror("msgrcv error");
        if (write(tip->fd, m.mtext, n) < 0)
            perror("write error");
    }
}

int main() {
    int i, n, err;
    int fd[2];
    int qid[NQ];
    struct pollfd pfd[NQ];
    struct threadinfo ti[NQ];
    pthread_t tid[NQ];
    char buf[MAXMSZ];

    for (i = 0; i < NQ; ++i) {
        if ((qid[i] = msgget((KEY + i), IPC_CREAT|0666)) < 0) {
            perror("msgget error");
        }

        printf("queue ID %d is %d\n", i, qid[i]);
        if (socketpair(PF_UNIX, SOCK_DGRAM, 0, fd) < 0)
            perror("socketpair error");

        pfd[i].fd = fd[0];
        pfd[i].events = POLLIN;
        ti[i].fd = fd[1];
        ti[i].qid = qid[i];
        if ((err = pthread_create(&tid[i], NULL, helper, &ti[i])) != 0) {
            perror("pthread create error");
            exit(EXIT_FAILURE);
        }
    }

    for (;;) {
        if (poll(pfd, NQ, -1) < 0)
            perror("poll error");
        for (i = 0; i < NQ; ++i) {
            if (pfd[i].revents & POLLIN) {
                if ((n = read(pfd[i].fd, buf, sizeof(buf))) < 0)
                    perror("read error");
                buf[n] = 0;
                printf("queue id %d, message %s\n", qid[i], buf);
            }
        }
    }
    return 0;
}
