#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    int pp2c[2];
    int pc2p[2];

    pipe(pp2c);
    pipe(pc2p);

    int pid = fork();

    char buf[64];

    if(pid == 0) {
        close(pp2c[1]);
        close(pc2p[0]);
        int n = read(pp2c[0], buf, sizeof(buf));

        if(n != 0) {
            printf("%d: received ping\n", getpid());
            write(pc2p[1], buf, sizeof(buf));
        }
    } else {
        close(pc2p[1]);
        close(pp2c[0]);
        write(pp2c[1], buf, sizeof(buf));

        int n = read(pc2p[0], buf, sizeof(buf));
        if(n != 0) {
            printf("%d: received pong\n", getpid());
        }
    }

    exit(0);
}