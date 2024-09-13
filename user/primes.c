#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void primes_helper(int read_file)
{
    int buffer;
    if (read(read_file, &buffer, 4) == 0) {
        close(read_file);
        return;
    }
    int prime = buffer;
    printf("prime %d\n", prime);
 
    int p_new[2];
    if (pipe(p_new) == -1) {
        write(1, "Error: pipe", 14);
        exit(1);
    }

    int pid = fork();

    if (pid < 0) {
        exit(0);
    } else if (pid != 0) {
        while (read(read_file, &buffer, 4) != 0) {
            if (buffer % prime != 0) {
                write(p_new[1], &buffer, 4);
            }
        }
        close(read_file);
        close(p_new[0]);
        close(p_new[1]);
        wait(0);
    } else {
        close(p_new[1]);
        close(read_file);
        primes_helper(p_new[0]);
        close(p_new[0]);
    }
    exit(0);
    return;
}

int main(int argc, char *argv[])
{
    int last_prime = 280;
    int buffer;

    int p[2];
    if (pipe(p) == -1){
        write(1, "Error", 5);
        exit(1);
    }
    printf("%d %d", p[0], p[1]);
    int pid = fork();
    
    if (pid != 0) {
        close(p[0]);
        for (int n = 2; n < last_prime + 1; n++){
            buffer = n;
            write(p[1], &buffer, 4);
        }
        close(p[1]);
        wait(0);
    } else {
        close(p[1]);
        primes_helper(p[0]);
    }

    exit(0);
    
}
