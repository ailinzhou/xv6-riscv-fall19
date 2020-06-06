#include "kernel/types.h"
#include "user/user.h"
#define READ 0
#define WRITE 1

void doPrimes(int toNext[])
{
    int a = 0;
    int p = 0;
    int rd = read(toNext[0], &a, sizeof(a));
    if (rd)
    {
        p = a;
        printf("prime %d\n", a);
    }
    else
    {
        return;
    }
    int toChild[2];
    pipe(toChild);
    int pid = fork();
    if (pid == 0)
    {
        close(toChild[WRITE]);
        doPrimes(toChild);
    }
    else
    {
        close(toChild[READ]);
        while (read(toNext[0], &a, sizeof(a)))
        {
            if (a % p != 0)
            {
                write(toChild[1], &a, sizeof(a));
            }
        }
        close(toChild[WRITE]);
    }
    close(toNext[READ]);
}

int main(int argc, char *argv[])
{
    int toNext[2];
    pipe(toNext);
    if (fork() > 0)
    {
        close(toNext[READ]);
        // parent processor
        for (int i = 2; i < 36; i++)
        {
            write(toNext[1], &i, sizeof(i));
        }
        close(toNext[WRITE]);
    }
    else
    {
        close(toNext[WRITE]);
        doPrimes(toNext);
    }
    exit();
}
