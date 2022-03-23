#include "controller.h"

uint monitor(const char* command);

void controller(int argc, const char** args){

    pid_t pids[argc];

    for(int i = 0; i < argc; ++i)

        if((pids[i] = fork()) == 0)
            _exit((int) monitor(args[i]));

    uint counters[argc];

    for(int i = 0; i < argc; ++i){
        int status;
        waitpid(pids[i], &status, 0);
        if(WIFEXITED(status))
            counters[i] = WEXITSTATUS(status);
    }

    for(int i = 0; i < argc; ++i)
        printf("%s %d\n", args[i], counters[i]);
}

uint monitor(const char* command){

    bool returned_zero = false;
    uint counter = 0;

    while(!returned_zero){

        const pid_t pid = fork();
        if(pid == 0){
            execlp(command, command, NULL);
            _exit(-1);
        }
        else{

            int status;
            waitpid(pid, &status, 0);

            if(WIFEXITED(status)){
                if(WEXITSTATUS(status) == 0)
                    returned_zero = true;
                else if(WEXITSTATUS(status) == 255)
                    return 0;
            }
            counter++;
        }
    }

    return counter;
}