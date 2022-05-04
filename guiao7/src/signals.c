#include "timed_multigrep.h"


static unsigned ctrl_c_count = 0;
static unsigned seconds = 0;



static void incr_count(int s){
    printf("%u seconds\n", seconds);
    ++ctrl_c_count;
}

static void leave(int s){
    printf("CTRL + C pressed %u times\n", ctrl_c_count);
    exit(0);
}

static void clock_time(int s){
    ++seconds;
    alarm(1);
}

static void ex1(void){

    alarm(1);

    signal(SIGALRM, clock_time);
    signal(SIGINT, incr_count);
    signal(SIGQUIT, leave);

    for(;;);
}

static void multigrep(const char* word, const char** files, unsigned num_of_files){

    pid_t pids[num_of_files];
    pid_t pid;

    for(unsigned i = 0; i  < num_of_files; ++i){

        if((pid = fork()) == 0){

            //sleep((i + 1) * 5);

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

        if(WIFEXITED(status) && WEXITSTATUS(status) == 0)

            for(unsigned j = 0; j < num_of_files; ++j){

                if(pids[j] > 0)
                    kill(pids[j], SIGKILL);

                found = true;
            }
    }

    if(found)
        printf("\"%s\" was found by %d\n", word, pid);
    else
        printf("\"%s\" not found\n", word);
}


int main(int argc, const char** argv){

    if(argc < 2){
        fprintf(stderr, "Missing arguments\n");
        return 1;
    }

    switch(atoi(argv[1])){

    case 1:
        ex1();
        break;

    case 2:
        if(argc < 4)
            fprintf(stderr, "Missing arguments\n");
        multigrep(argv[2], argv + 3, (size_t) argc - 3);
        break;

    case 3:
        if(argc < 4)
            fprintf(stderr, "Missing arguments\n");
        timed_multigrep(argv[2], argv + 3, (size_t) argc - 3);
        break;

    default:
        fprintf(stderr, "Unknown option");
        return 2;
    }
}