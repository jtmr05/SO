#include "bash.h"


void bash(){

    size_t size = 16;
    char* in_buffer = malloc(sizeof *in_buffer * size), *ptr = in_buffer;
    int bytes_read;
    uint has_realloc = 1;

    write(STDOUT_FILENO, "$ ", 2);

    while((bytes_read = read(STDIN_FILENO, ptr, size / has_realloc)) > 0){


        char *nl;

        if(!(nl = strchr(ptr, '\n'))){                                          //no newline was found
            in_buffer = realloc(in_buffer, sizeof *in_buffer * size * 2);       //buffer not big enough
            ptr = in_buffer + size;
            size *= 2;
            has_realloc = 2;
        }
        else{
            has_realloc = 1;
            ptr = in_buffer;
            *nl = '\0';

            mysystem(in_buffer);
        }

        if(has_realloc == 1)
            write(STDOUT_FILENO, "$ ", 2);
    }

    free(in_buffer);
}

int mysystem(const char* command){

    char* str = strdup(command);
    char* ptr = str;
    int arg_count = 1;

    while((ptr = strchr(ptr, ' '))){
        while(*ptr == ' '){
            *ptr = '\0';
            ptr++;
        }

        arg_count++;
    }

    ptr = str;
    char* args[arg_count + 1];
    args[arg_count] = NULL;

    for(int i = 0; i < arg_count && ptr; ++i){
        args[i] = ptr;
        ptr = strchr(ptr, '\0');
        while(*ptr == '\0')
            ptr++;
    }

    bool wait_for_child = true;

    if(strcmp(args[arg_count - 1], "&") == 0){
        args[arg_count - 1] = NULL;
        wait_for_child = false;
    }


    if(fork() == 0){
        const int exit_code = execvp(args[0], args);

        char msg[256];
        snprintf(msg, 256, "-bash: command not found\n");
        write(STDERR_FILENO, msg, strlen(msg));

        _exit(exit_code);
    }
    else if(wait_for_child){

        free(str);

        int status;
        wait(&status);
        if(WIFEXITED(status))
            return WEXITSTATUS(status) == 255 ? -1 : WEXITSTATUS(status);
        else
            return -1;
    }
    else{
        free(str);
        return 0;
    }
}