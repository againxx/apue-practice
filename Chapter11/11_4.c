#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

struct foo {
    int a, b, c, d;
};

void printfoo(const char* s, const struct foo* fp) {
    printf("%s", s);
    printf("  structure at 0x%lx\n", (unsigned long)fp);
    printf("  foo.a = %d", fp->a);
    printf("  foo.b = %d", fp->b);
    printf("  foo.c = %d", fp->c);
    printf("  foo.d = %d", fp->d);
}

int main() {
    int err;
    pthread_t tid1, tid2;
    struct foo* fp;

    err = pthread_create(&tid1, NULL, thr_fu1, NULL);
    if (err != 0) {
        fprintf(stderr, "can't create thread 1");
        exit(1);
    }
    err = pthread_join(tid1, (void **)&fp);
    return 0;
}
