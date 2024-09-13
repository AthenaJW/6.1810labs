#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[])
{
    // 1. identify the command xargs is using and move them to an array of size n+1
    char *command[argc+1];
    command[argc] = 0;
    for (int i = 0; i < argc-1; i++) {
        command[i] = argv[i+1];
    }
    char *path = argv[1];
    // 2. open read standard input
    char buffer = ' ';
    char read_input[256], *p;
    p = read_input;

    // 3. read character at a time until a newline appears
    while (read(0, &buffer, 1) != 0) {
        while (buffer != '\n') {
            *p = buffer;
            p++;
            read(0, &buffer, 1);
        } 
        int pid = fork();
        if (pid == 0) {
            command[argc-1] = read_input;
            //printf("%s", read_input);
            exec(path, command);
        } else {
            wait(0);
            p = read_input;
            memset(p, 0, sizeof(read_input));
        }
    }
    return exit(0);
}
