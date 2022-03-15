#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

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

void free_matrix(int** matrix, size_t lines){
    for(uint i = 0; i < lines; i++)
        free(matrix[i]);

    free(matrix);
}

int** matrix_gen(size_t lines, size_t collumns){

    int **matrix = calloc(lines, sizeof *matrix);

    if(matrix){

        bool alloc_fail = false;

        for(uint i = 0; i < lines; i++)
            if(!(matrix[i] = malloc(sizeof **matrix * collumns)))
                alloc_fail = true;

        if(!alloc_fail){

            srand((uint) time(NULL));

            for(uint i = 0; i < lines; i++)
                for(uint j = 0; j < collumns; j++)
                    matrix[i][j] = rand() % (ssize_t) collumns;
        }
        else{
            free_matrix(matrix, lines);
            matrix = NULL;
        }
    }

    return matrix;
}

void matrix_lookup_switch(bool all){

    const size_t lines = 10, collumns = 10000;
    int** matrix = matrix_gen(lines, collumns);
    const int target = rand() % (ssize_t) collumns;


    if(matrix){
        if(!all)
            contains_number(target, matrix, lines, collumns);
        else
            contains_number_all(target, matrix, lines, collumns);
    }

    free_matrix(matrix, lines);
}

void lookup_in_file(){

    const size_t lines = 10, collumns = 10000;
    int** matrix = matrix_gen(lines, collumns);
    const int target = rand() % (ssize_t) collumns;

    const int fd_write = open("resources/matrix.bin", O_WRONLY | O_CREAT | O_TRUNC, 0660);

    if(matrix){
        if(fd_write > -1){

            for(uint i = 0; i < lines; ++i){
                ssize_t bread;
                if((bread = write(fd_write, matrix[i], sizeof *matrix[i] * collumns)) <= 0)
                    exit(1);
            }

            close(fd_write);



            for(uint i = 0; i < lines; ++i){

                if(fork() == 0){

                    const int fd_read = open("resources/matrix.bin", O_RDONLY | O_BINARY);

                    if(fd_read > -1){
                        lseek(fd_read, i * collumns * sizeof(int), SEEK_SET);
                        int *p = malloc(sizeof *p);

                        uint j;
                        for(j = 0; j < collumns; j++){
                            read(fd_read, p, sizeof *p);
                            if(*p == target){
                                free(p);
                                _exit(1);
                            }
                        }

                        free(p);
                        close(fd_read);
                        _exit(0); // dont return negative number
                    }
                    else
                        perror("Failed to open file.");
                }
            }

            bool found = false;

            for(uint i = 0; i < lines && !found; i++){

                int status = 0;
                const pid_t pid = wait(&status);
                const uint exit_code = WEXITSTATUS(status);

                if(exit_code == 1){
                    found = true;
                    printf("Child %d found the target %d\n", pid, target);
                }
            }

            if(!found)
                printf("Target %d was not found\n", target);

        }
        else
            perror("Failed to open file.");

        free_matrix(matrix, lines);
    }
    else
        perror("Out of memory");
}



void bin_to_csv(){

    int *p = malloc(sizeof *p);

    const int read_fd = open("resources/matrix.bin", O_RDONLY);
    const int write_fd = open("resources/matrix.csv", O_CREAT | O_TRUNC | O_WRONLY, 0660);
    if(read_fd >= 0 && write_fd >= 0){

        int nl = 0;

        while(read(read_fd, p, sizeof *p) > 0){
            if(nl++ == 100)
                write(write_fd, "\n", 1);

            char str[100];
            snprintf(str, 100, "%d", *p);

            write(write_fd, str, strlen(str));

            write(write_fd, ";", 1);
        }

        close(write_fd);
        close(read_fd);
    }


    free(p);
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
            matrix_lookup_switch(false);
            break;

        case 6:
            matrix_lookup_switch(true);
            break;

        case 7:
            lookup_in_file();
            break;

        case 8:
            bin_to_csv();
            break;

        default:
            perror("Invalid option.\n");
            break;
        }
    }
    else
        perror("No option provided.\n");


    return 0;
}
