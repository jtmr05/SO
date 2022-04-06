#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>


int ex1_2(){

    int file_ds[2] = {};
    pipe(file_ds);

    const size_t size = 1024;
    char buffer[size];
    memset(buffer, 0, size * sizeof *buffer);


    switch(fork()){

    case -1:
        return -1;

    case 0: {
        close(file_ds[1]);

        ssize_t bytes_read;

        while((bytes_read = read(file_ds[0], buffer, size)) > 0)
            write(STDOUT_FILENO, buffer, (size_t) bytes_read);

        write(STDOUT_FILENO, "\n", 1);

        close(file_ds[0]);
        _exit(0);
    }

    default: {
        close(file_ds[0]);

        ssize_t bytes_read = 0;

        do{
            if(bytes_read < (ssize_t) size){
                const char* msg = "Your input: ";
                write(STDOUT_FILENO, msg, strlen(msg));
            }

            bytes_read = read(STDIN_FILENO, buffer, size);

            if(bytes_read > 0)
                write(file_ds[1], buffer, (size_t) bytes_read);

        }
        while(bytes_read > 0);

        close(file_ds[1]);
        break;
    }

    }
    return 0;
}

int ex3(){

    int file_ds[2] = {};
    pipe(file_ds);

    size_t size = 1024;
    char buffer[size];
    memset(buffer, 0, size * sizeof *buffer);

    switch(fork()){

    case -1:
        return -1;

    case 0:
        close(file_ds[1]);

        dup2(file_ds[0], STDIN_FILENO);

        execlp("wc", "wc", NULL);
        _exit(1);

    default: {
        close(file_ds[0]);

        ssize_t bytes_read = 0;

        do{

            bytes_read = read(STDIN_FILENO, buffer, size);

            if(bytes_read > 0)
                write(file_ds[1], buffer, (size_t) bytes_read);

        }
        while(bytes_read > 0);

        close(file_ds[1]);
        break;
    }

    }
    return 0;
}

int ex4(){

    int file_ds[2] = {};
    pipe(file_ds);

    unsigned fork_count = 0;
    bool is_writer = false;


    FORK:
    is_writer = !is_writer;

    switch(fork()){

    case -1:
        exit(-1);

    case 0:
        if(is_writer){
            close(file_ds[0]);

            dup2(file_ds[1], STDOUT_FILENO);
            close(file_ds[1]);

            execlp("ls", "ls", "/etc", NULL);
        }
        else{
            close(file_ds[1]);

            dup2(file_ds[0], STDIN_FILENO);
            close(file_ds[0]);

            execlp("wc", "wc", "-l", NULL);
        }

        _exit(1);

    default:
        if(++fork_count < 2)
            goto FORK;

        close(file_ds[0]); close(file_ds[1]);

        for(int i = 0; i < 2; ++i)
            wait(NULL);

        break;
    }

    return 0;
}


int ex5(){

    int pipe1[2] = {}, pipe2[2] = {}, pipe3[2] = {};
    pipe(pipe1); pipe(pipe2); pipe(pipe3);

    for(int i = 0; i < 4; ++i){

        switch(fork()){

        case -1:
            exit(-1);

        case 0:
            switch(i){

            case 0:
                dup2(pipe1[1], STDOUT_FILENO);

                close(pipe1[0]); close(pipe1[1]); close(pipe2[0]); close(pipe2[1]); close(pipe3[0]); close(pipe3[1]);

                execlp("grep", "grep", "-v", "^#", "/etc/passwd", NULL);
                _exit(1);

            case 1:
                dup2(pipe1[0], STDIN_FILENO);
                dup2(pipe2[1], STDOUT_FILENO);

                close(pipe1[0]); close(pipe1[1]); close(pipe2[0]); close(pipe2[1]); close(pipe3[0]); close(pipe3[1]);

                execlp("cut", "cut", "-f7", "-d:", NULL);
                _exit(1);

            case 2:
                dup2(pipe2[0], STDIN_FILENO);
                dup2(pipe3[1], STDOUT_FILENO);

                close(pipe1[0]); close(pipe1[1]); close(pipe2[0]); close(pipe2[1]); close(pipe3[0]); close(pipe3[1]);

                execlp("uniq", "uniq", NULL);
                _exit(1);

            case 3:
                dup2(pipe3[0], STDIN_FILENO);

                close(pipe1[0]); close(pipe1[1]); close(pipe2[0]); close(pipe2[1]); close(pipe3[0]); close(pipe3[1]);

                execlp("wc", "wc", "-l", NULL);
                _exit(1);
            }

        default:
            break;
        }
    }

    close(pipe1[0]); close(pipe1[1]); close(pipe2[0]); close(pipe2[1]); close(pipe3[0]); close(pipe3[1]);

    for(int i = 0; i < 4; ++i)
        wait(NULL);

    return 0;
}


int main(int argc, const char** argv){

    int exit_code = 0;

    if(argc  > 1){

        switch(atoi(argv[1])){

        case 1:
        case 2:
            exit_code = ex1_2();
            break;

        case 3:
            exit_code = ex3();
            break;

        case 4:
            exit_code = ex4();
            break;

        case 5:
            exit_code = ex5();
            break;

        default:
            break;
        }
    }
    else{
        exit_code = 1;
        fprintf(stderr, "Not enough args.\n");
    }

    return exit_code;
}