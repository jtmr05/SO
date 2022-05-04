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


int ex5(const char** commands, size_t number_of_commands){

    const size_t number_of_pipes = number_of_commands - 1;
    int pipe_matrix[number_of_pipes][2];

    for(unsigned p = 0; p < number_of_pipes; ++p)
        pipe(pipe_matrix[p]);

    for(unsigned i = 0; i < number_of_commands; ++i){

        switch(fork()){

        case -1:
            exit(-1);

        case 0:

            if(i == 0)
                dup2(pipe_matrix[i][1], STDOUT_FILENO);

            else if (i == number_of_commands - 1)
                dup2(pipe_matrix[i - 1][0], STDIN_FILENO);

            else{
                dup2(pipe_matrix[i - 1][0], STDIN_FILENO);
                dup2(pipe_matrix[i][1], STDOUT_FILENO);
            }


            for(unsigned p = 0; p < number_of_pipes; ++p)
                for(unsigned j = 0; j < 2; ++j)
                    close(pipe_matrix[p][j]);

            system(commands[i]);
            exit(0);

            break;

        default:
            break;
        }
    }

    for(unsigned p = 0; p < number_of_pipes; ++p)
        for(unsigned j = 0; j < 2; ++j)
            close(pipe_matrix[p][j]);

    for(unsigned w = 0; w < number_of_commands; ++w)
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
            const size_t num_of_commands = 4;
            const char* commands[] = {"grep -v ^# /etc/passwd", "cut -f7 -d:", "uniq", "wc -l"};

            exit_code = ex5(commands, num_of_commands);
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
