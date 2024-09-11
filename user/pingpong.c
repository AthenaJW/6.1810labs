#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void main(int argc, char *argv[])
{
    int p2c[2];
    int c2p[2];
    char buffer[1];

    if (pipe(p2c) == -1 || pipe(c2p) == -1){
        write(1, "Error", 5);
        exit(1);
    }
  
    int pid = fork();
    printf("%d", getpid());

    if (pid == 0) {
        read(p2c[0], buffer, 1);
        printf("%d: received ping\n", getpid());
        write(c2p[1], buffer, 1);
    }
    else if (pid < 0) {
        printf("Fork Failed");
    }
    else {
        write(p2c[1], buffer, 1);
        read(c2p[0], buffer, 1);
        printf("%d: received pong\n", getpid());
    }
    exit(0);
}
