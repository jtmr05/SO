#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

void pids(){
    printf("%d is my father's pid and %d is mine.\n", getppid(), getpid());
}

void fork_and_pids(){

    const pid_t child = fork();

    if(child == 0)
        printf("I'm a child and my pid is %d, while my father's is %d.\n", getpid(), getppid());
    else
        printf("I'm the dad and my pid is %d, my kid's is %d and my father's is %d.\n",
            getpid(), child, getppid());
}

void children_tenfold_seq(){

    for(int i = 0; i < 10; i++){

        const pid_t child = fork();

        if(child == 0){
            pids();
            _exit(i + 1);
        }
        else{
            int status = 0;
            const pid_t dead_kid = wait(&status);
            printf("Child %d exited with exit code %d.\n\n", dead_kid, WEXITSTATUS(status));
        }
    }
}

void children_tenfold_conc(){

    for(int i = 0; i < 10; i++){

        const pid_t child = fork();

        if(child == 0){
            pids();
            _exit(i + 1);
        }
    }

    for(int i = 0; i < 10; i++){
        int status = 0;
        const pid_t dead_kid = wait(&status);
        printf("Child %d exited with exit code %d.\n\n", dead_kid, WEXITSTATUS(status));
    }
}

bool contains_number(int target, int **matrix, int L, int C){

    if(!matrix)
        return false;

    for(int i = 0; i < L; i++){

        if(fork() == 0){

            if(matrix[i]){

                for(int j = 0; j < C; j++){
                    if(matrix[i][j] == target)
                        _exit(1);
                }
            }

            _exit(0);
        }
    }

    bool found = false;

    for(int i = 0; i < L; i++){

        int status = 0;
        const pid_t dead_kid = wait(&status);
        if(WEXITSTATUS(status) == 1){
            found = true;
            printf("Child %d found the target %d.\n", dead_kid, target);
            break;
        }
    }

    return found;
}

bool contains_number_all(int target, int **matrix, uint L, uint C){

    if(!matrix)
        return false;

    for(uint i = 0; i < L; i++){

        if(fork() == 0){

            if(matrix[i]){

                for(uint j = 0; j < C; j++){
                    if(matrix[i][j] == target)
                        _exit((int) i);
                }
            }

            _exit((int) L + 1); // dont return negative number
        }
    }

    bool found = false;
    bool arr[L];

    for(uint i = 0; i < L; i++)
        arr[i] = false;

    for(uint i = 0; i < L; i++){

        int status = 0;
        wait(&status);
        const uint index = WEXITSTATUS(status);

        if(index < L){
            found = true;
            arr[index] = true;
        }
    }

    printf("Lines where %d was found at:\n", target);
    for(uint i = 0; i < L; i++){
        if(arr[i])
            printf("\t=> %u\n", i);
    }

    return found;
}

void matrix_gen(bool all){

    const size_t lines = 10, collumns = 10000;

    int **matrix = calloc(lines, sizeof *matrix);

    if(matrix){

        bool do_stuff = true;

        for(uint i = 0; i < lines; i++)
            if(!(matrix[i] = malloc(sizeof **matrix * collumns)))
                do_stuff = false;

        if(do_stuff){

            srand((uint) time(NULL));

            const int target = rand() % (ssize_t) collumns;

            for(uint i = 0; i < lines; i++)
                for(uint j = 0; j < collumns; j++)
                    matrix[i][j] = rand() % (ssize_t) collumns;


            if(!all)
                contains_number(target, matrix, lines, collumns);
            else
                contains_number_all(target, matrix, lines, collumns);
        }
    }

    for(uint i = 0; i < lines; i++)
        free(matrix[i]);

    free(matrix);
}

int main(int argc, const char** argv){

    if(argc > 1){

        switch(atoi(argv[1])){

        case 1:
            pids();
            break;

        case 2:
            fork_and_pids();
            break;

        case 3:
            children_tenfold_seq();
            break;

        case 4:
            children_tenfold_conc();
            break;

        case 5:
            matrix_gen(false);
            break;

        case 6:
            matrix_gen(true);
            break;

        default:
            printf("Invalid option.\n");
            break;
        }
    }
    else
        printf("No option provided.\n");


    return 0;
}
