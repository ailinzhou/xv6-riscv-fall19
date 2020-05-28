#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int toChild[2];
    int toParent[2];
    pipe(toChild);
    pipe(toParent);
    if (fork() != 0)
    {
        //It's parent processor
        // read from toParent[0],so close toParent[1]
        close(toParent[1]);
        // write to toChild[1], so close toChild[0]
        close(toChild[0]);
        // write ping
        write(toChild[1], "ping", 4);
        char pong[4];
        fprintf(2, "%d: debug1\n", getpid());
        read(toParent[0], pong, 4);
        fprintf(2, "%d: received %s\n", getpid(), pong);
    }
    else
    {
        //It's parent processor
        // read from toParent[0],so close toParent[1]
        close(toParent[0]);
        // write to toChild[1], so close toChild[0]
        close(toChild[1]);
        char ping[4];
        fprintf(2, "%d: debug2\n", getpid());
        read(toChild[0], ping, 4);
        fprintf(2, "%d: received %s\n", getpid(), ping);
        write(toParent[1], "pong", 4);
    }
    exit();
}
