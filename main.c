#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int status;
    __pid_t myPid;
    printf("\nstarting....\n");
    int pid = fork();

    if (pid == -1)
    {
        printf("\n errore nella fork...");
        return (1);
    }

    if (pid == 0)
    {
        // sono sul processo figlio
        printf("\n(PID = %d) ID:%d Ciao... sono il processo figlio..\n", pid, (int)getpid());
        for (int i = 3; i >= 0; i--)
        {

            printf("%d ", i);
            fflush(stdout);
            sleep(1);
        }

        char *argv[] = {"./ciao.txt", NULL};
        char *envp[] = {NULL};
        execve("./hal", argv, envp);
    }
    else
    {
        if (waitpid(pid, &status, 0) == pid)
        {
            if (status != 0)
            {
                printf("PAPI: ohi... qualcosa è andato storto: ho ricevuto un segnale dimmerda! %d", status);
            }
            else
            {
                printf("\n(PID = %d) :Ciao... sono il paparno di %d...\n", status, pid);
                for (int i = 11; i < 20; i++)
                {
                    printf("%d ", i);
                    fflush(stdout);
                    sleep(1);
                }
            }
        }
    }
}
