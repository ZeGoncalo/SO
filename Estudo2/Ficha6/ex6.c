#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <string.h>
#include <ctype.h>
int main(int argc, char* argv[]){
    int sv[2];
    socketpair(AF_UNIX, SOCK_STREAM, 0, sv);
    int pid=(int)fork();
    if(pid==-1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if(pid>0){
        close(sv[1]);
        const char *msgs[] = {"ola\n", "tudo bem?\n", "fim\n"};
        for (int i = 0; i < 3; i++) {
            if (write(sv[0], msgs[i], strlen(msgs[i])) == -1) {
                perror("write (pai)");
                exit(EXIT_FAILURE);
            }
        }
        shutdown(sv[0], SHUT_WR);
        char buf[1024];
        int n = read(sv[0], buf, sizeof(buf));
        if(n>0){
            write(STDOUT_FILENO,buf,n);
        }
        wait(NULL);
        close(sv[0]);
    }
    if(pid==0){
        close(sv[0]);
        int n;
        char buf[1024];
        while((n=read(sv[1],buf,sizeof(buf)))>0){
            for (int i=0;i<n;i++){
                buf[i]=toupper(buf[i]);
            }
            write(sv[1],buf,n);
        }
        close(sv[1]);
    }
    exit(EXIT_SUCCESS);
}