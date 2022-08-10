#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int main() {

    int wbuf[34];
    int rbuf[34];

    for (int i = 0; i < 34 ; i++) {
        wbuf[i] = i + 2;
    }

    while (1) {
        int p[2];
        pipe(p);


        int pid = fork();
        if(pid != 0){
            close(p[0]);
            write(p[1], wbuf, sizeof(wbuf));
            wait(0);
            exit(0);
        } else {
            close(p[1]);

            int j = 0;
            int n = read(p[0], rbuf, sizeof(rbuf));

            int prime = rbuf[0];
            if(n == 0 || prime == 0) {
                exit(0);
            }

            printf("prime %d\n", prime);
            memset(wbuf, 0, sizeof(wbuf));

            for (int i = 1; i < 34 ; i++) {
                if((rbuf[i] % prime) != 0) {
                    wbuf[j++] = rbuf[i];
                }
            }

        }

    }

    exit(0);
}