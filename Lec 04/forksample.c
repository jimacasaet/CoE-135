#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    pid_t pid;

    /* spawn a new child process */
    pid = fork();

    if (pid < 0) {  //failed to create a child process
        fprintf(stderr, "Failed to create a child process");
        return 1;
    } else if (!pid) { //child process
        execlp("/bin/pwd", "pwd", NULL);
    } else { //parent process
        wait(NULL); //wait for child to terminate
        printf("I have reaped my child\n");
    }
    return 0;
}