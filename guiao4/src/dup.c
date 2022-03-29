#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>


#include "bash.h"


void ex1_2_3(){

    const char* fnin = "/etc/man_db.conf";
    const char* fnout = "resources/saida.txt";
    const char* fnerr = "resources/erros.txt";

    const int fdin = open(fnin, O_RDONLY);
    const int fdout = open(fnout, O_WRONLY | O_CREAT | O_TRUNC, 0660);
    const int fderr = open(fnerr, O_WRONLY | O_CREAT | O_TRUNC, 0660);

    const int old_stdin = dup(STDIN_FILENO);
    const int old_stdout = dup(STDOUT_FILENO);
    const int old_stderr = dup(STDERR_FILENO);

    const int new_fdin = dup2(fdin, STDIN_FILENO);
    const int new_fdout = dup2(fdout, STDOUT_FILENO);
    const int new_fderr = dup2(fderr, STDERR_FILENO);

    ssize_t b_read;
    const size_t buffer_size = 4096;
    char buffer[buffer_size];

    if(fork() == 0){
        while((b_read = read(new_fdin, buffer, buffer_size)) > 0)
            write(new_fdout, buffer, (size_t) b_read);

        execlp("wc", "wc", NULL);
        _exit(0);
    }
    else
        wait(NULL);

    dup2(old_stdin, STDIN_FILENO);
    dup2(old_stdout, STDOUT_FILENO);
    dup2(old_stderr, STDERR_FILENO);

    close(fdin);
    close(fdout);
    close(fderr);

    printf("terminei\n");
}

int redir(int argc, const char** argv){

    bool is_valid = true;

    if(argc > 0){

        char* fnin = NULL;
        char* fnout = NULL;

        char* command[argc + 1];
        int c = 0;

        for(int i = 0; i < argc; ++i){

            if(strcmp(argv[i], "-i") == 0)
                if(++i < argc && strcmp(argv[i], "-o") != 0){
                    fnin = argv[i];
                }
                else{
                    is_valid = false;
                    break;
                }

            else if(strcmp(argv[i], "-o") == 0)
                if(++i < argc && strcmp(argv[i], "-i") != 0){
                    fnout = argv[i];
                }
                else{
                    is_valid = false;
                    break;
                }

            else
                command[c++] = argv[i];
        }

        if(is_valid){

            command[c++] = NULL;

            int fd_in = STDIN_FILENO, fd_out = STDOUT_FILENO;

            if(fnin){
                int tmp = open(fnin, O_RDONLY);
                if(tmp > -1)
                    dup2(tmp, fd_in);
            }

            if(fnout){
                int tmp = open(fnout, O_WRONLY | O_TRUNC | O_CREAT, 0660);
                if(tmp > -1)
                    dup2(tmp, fd_out);
            }

            if(fork() == 0){

                execvp(command[0], command);
                _exit(1);
            }
            else
                wait(NULL);

            close(fd_in);
            close(fd_out);

            return 0;
        }

        const char* err_msg = "Invalid arguments found.\n";
        write(STDERR_FILENO, err_msg, strlen(err_msg));
        return -1;
    }

    const char* err_msg = "Not enough arguments.\n";
    write(STDERR_FILENO, err_msg, strlen(err_msg));
    return 1;
}

int main(int argc, const char** argv){

    if(argc > 1){

        switch (atoi(argv[1])){

        case 1:
        case 2:
        case 3:
            ex1_2_3();
            break;

        case 4:
            redir(argc - 2, argv + 2); //ignore option and executable name
            break;

        case 5:
            bash_mkII();
            break;

        default:
            break;
        }
    }
    else{
        const char* msg = "Not enough arguments.\n";
        write(STDERR_FILENO, msg, strlen(msg));
    }


    return 0;
}
