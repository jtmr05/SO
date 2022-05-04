#include "timed_multigrep.h"

static pid_t *pids;
static size_t num_of_pids;
static const unsigned NUM_OF_SECONDS = 10;

static void alarm_handler(int s){
    for(unsigned j = 0; j < num_of_pids; ++j)
        if(pids[j] > 0)
            kill(pids[j], SIGKILL);
}

void timed_multigrep(const char* word, const char** files, unsigned num_of_files){

    pid_t pid;

    pids = malloc(sizeof *pids * num_of_files);
    num_of_pids = num_of_files;

    signal(SIGALRM, &alarm_handler);
    alarm(NUM_OF_SECONDS);

    for(unsigned i = 0; i  < num_of_files; ++i){

        if((pid = fork()) == 0){

            sleep((i + 1) * 8);

            const int dev_null = open("/dev/null", O_WRONLY);
            dup2(dev_null, STDOUT_FILENO);
            close(dev_null);

            execlp("grep", "grep", word, files[i], NULL);
            _exit(1);
        }
        else{
            printf("Child %u with pid %d\n", i, pid);
            pids[i] = pid;
        }
    }

    int status;
    bool found = false;

    for(unsigned i = 0; i < num_of_files && !found; ++i){

        pid = wait(&status);

        if(pid == -1)
            break;

        if(WIFEXITED(status) && WEXITSTATUS(status) == 0){

            for(unsigned j = 0; j < num_of_files; ++j){

                if(pids[j] > 0)
                    kill(pids[j], SIGKILL);

                found = true;
            }
            printf("%s was found by \"%d\"\n", word, pid);

        }
        else
            printf("Child %d did not find \"%s\"\n", pid, word);
    }

    if(!found)
        printf("\"%s\" was not found\n", word);

    free(pids);
}