#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"



void dfs(char* path, char* targetFile) {
    int fd;
    char *p;
    struct dirent de;
    struct stat st;
    char buf[512];

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) {
        case T_FILE:
            {
                // Find first character after last slash.
                for(p=path+strlen(path); p >= path && *p != '/'; p--)
                    ;
                p++;
                // Return blank-padded name.
                if(strlen(p) >= DIRSIZ) {
                    break;
                } else {
                    if(strcmp(p, targetFile) == 0) {
                        printf("%s\n", path);
                    }
                }
                
            }

        break;
        case T_DIR:
            {
                if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                    printf("ls: path too long\n");
                    break;
                }

                strcpy(buf, path);
                p = buf+strlen(buf);
                *p++ = '/';
                while(read(fd, &de, sizeof(de)) == sizeof(de)){
                    if(de.inum == 0)
                        continue;
                    memmove(p, de.name, DIRSIZ);
                    p[DIRSIZ] = 0;
                    if(stat(buf, &st) < 0){
                        printf("ls: cannot stat %s\n", buf);
                        continue;
                    }
                    if (strcmp(p, ".") == 0 || strcmp(p, "..") == 0) {
                        continue;
                    }
                    dfs(buf, targetFile);
                }
            }
        break;

    }
    close(fd);
}

int main(int argc, char* argv[]) {
    
    if(argc != 3) {
        printf("error : invalid argument");
        printf("usage : find <dir> <file>\n");
        exit(0);
    }

    dfs(argv[1], argv[2]);

    exit(0);
}