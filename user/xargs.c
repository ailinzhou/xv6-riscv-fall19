#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{

    char buf2[512];
    char buf[MAXARG][MAXARG];
    char *pass[MAXARG];
    // for (int i = 1; i < argc; i++)
    // {
    //     printf("args %s", argv[i]);
    // }
    for (int i = 0; i < MAXARG; i++)
    {
        pass[i] = buf[i];
    }
    int i;
    for (i = 1; i < argc; i++)
    {
        strcpy(buf[i - 1], argv[i]);
    }
    // for (i = 1; i < argc; i++)
    // {
    //     printf("args %s",buf[i - 1]);
    // }
    int n;
    while ((n = read(0, buf2, sizeof(buf2))) > 0)
    {
        int pos = argc - 1;
        char *c = buf[pos];
        for (char *p = buf2; *p; p++)
        {
            if (*p == ' ' || *p == '\n')
            {
                *c = '\0';
                pos++;
                c = buf[pos];
            }
            else
                *c++ = *p;
        }
        *c = '\0';
        pos++;
        pass[pos] = 0;

        if (fork())
        {
            wait();
        }
        else{
            // for(int i = 0;i<MAXARG; i++){
            //     printf("cmd:  %s", pass[i]);
            // }
            exec(pass[0], pass);
        }
    }

    if (n < 0)
    {
        printf("xargs: read data error\n");
        exit();
    }
    exit();
}
