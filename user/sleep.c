#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if(argc != 2) {
        fprintf(2, "invalid argument number : %d \n", argc - 1);
        fprintf(2, "usage : sleep <ticks> \n");
        exit(1);
    }

    int i = atoi(argv[1]);
    if(i == 0) {
        fprintf(2, "invalid sleep ticks : %d \n", i);
        exit(1);
    }

    sleep(i);


    exit(0);
}
