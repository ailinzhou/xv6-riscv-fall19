#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        fprintf(2, "Usage: sleep timeNumber \n");
        exit();
    }
    char *numberChar = argv[1];
    int number = atoi(numberChar);
    if(number < 0){
        fprintf(2, "Usage: sleep time should greater zero\n");
        exit();
    }
    sleep(number);
    exit();
}
