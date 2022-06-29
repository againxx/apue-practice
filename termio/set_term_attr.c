#include <stdio.h>
#include <string.h>
#include <termio.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFSIZE 4096

int main() {
    struct termios tp;
    if (tcgetattr(STDIN_FILENO, &tp) == -1) {
        perror("Unable to get terminal attribute");
        exit(EXIT_FAILURE);
    }
    tp.c_lflag &= ~ECHO;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &tp) == -1) {
        perror("Unable to set terminal attribute");
        exit(EXIT_FAILURE);
    }
    char buf[BUFSIZE];
    while (1) {
        scanf("%s", buf);
        puts(buf);
    }
    return 0;
}
