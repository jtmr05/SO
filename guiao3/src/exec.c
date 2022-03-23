#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "bash.h"
#include "controller.h"

void ls(){
    const char *args[] = {"ls", "-l", NULL};
    const int exit_code = execvp(args[0], args);
    fprintf(stderr, "Exited with error code %d.\n", exit_code);
}

void ls_child(){

    const pid_t pid = fork();

    if(pid == 0){
        printf("I'm the child.\n");
        ls();
    }
    else{
        int status;
        wait(&status);
        if(WIFEXITED(status))
            printf("Child has finished with exit code %d.\n", WEXITSTATUS(status));
        else
            fprintf(stderr, "Error occurred.\n");
    }
}

void concurr_execs(int size, char* arr[]){

    for(int i = 0; i < size; ++i){

        if(fork() == 0){
            const int exit_code = execlp(arr[i], arr[i], NULL);
            fprintf(stderr, "Exited with error code %d.\n", exit_code);
        }
    }

    for(int i = 0; i < size; ++i){
        int status;
        wait(&status);
        if(WIFEXITED(status))
            printf("Child has finished with exit code %d.\n", WEXITSTATUS(status));
        else
            fprintf(stderr, "Error occurred.\n");
    }
}



int main(int argc, char** argv){

    if(argc > 1){

        switch(atoi(argv[1])){

        case 1:
            puts("ls -l");
            ls();
            break;

        case 2:
            puts("ls -l");
            ls_child();
            break;

        case 3:{
            if(argc > 2){

                const int size = argc - 2;
                char* execs[argc - 2];

                for(int i = 2, j = 0; i < argc; ++i, ++j)
                    execs[j] = argv[i];

                concurr_execs(size, execs);
            }
            else
                fprintf(stderr, "Not enough args.\n");
            break;
        }

        case 4:{
            if(argc > 2){
                for(int i = 2; i < argc; ++i)
                    mysystem(argv[i]);
            }
            else
                fprintf(stderr, "Not enough args.\n");
            break;
        }

        case 5:
            bash();
            break;

        case 6:{
            if(argc > 2)
                controller(argc - 2, argv + 2);
            else
                fprintf(stderr, "Not enough args.\n");
            break;
        }

        default:
            fprintf(stderr, "Unknown option.");
            break;
        }
    }

    return 0;
}
