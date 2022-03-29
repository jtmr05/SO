#include "bash.h"


void bash_mkII(){

    size_t size = 16;
    char* in_buffer = malloc(sizeof *in_buffer * size), *ptr = in_buffer;
    int bytes_read;
    uint has_realloc = 1;
    bool exit = false;

    write(STDOUT_FILENO, "$ ", 2);

    while(!exit && (bytes_read = read(STDIN_FILENO, ptr, size / has_realloc)) > 0){

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

            exit = strcmp("exit", in_buffer) == 0;

            if(!exit)
                mysystem(in_buffer);
        }

        if(has_realloc == 1 && !exit)
            write(STDOUT_FILENO, "$ ", 2);
    }

    free(in_buffer);
}

static inline bool is_operand(const char* op){
    return strcmp(op, "<")   == 0 ||
           strcmp(op, ">")   == 0 ||
           strcmp(op, "1>")  == 0 ||
           strcmp(op, ">>")  == 0 ||
           strcmp(op, "1>>") == 0 ||
           strcmp(op, "2>")  == 0 ||
           strcmp(op, "2>>") == 0;
}

static inline bool set_redir(const char* op, const char* file, int* fd, int* std){

    if(!file || is_operand(file))
        return false;

    bool ret = false;

    if(strcmp(op, "<") == 0){
        *std = STDIN_FILENO;
        *fd = open(file, O_RDONLY);
        if(*fd > -1)
            ret = dup2(*fd, STDIN_FILENO) != -1;
    }
    else if(strcmp(op, ">") == 0 || strcmp(op, "1>") == 0){
        *std = STDOUT_FILENO;
        *fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0660);
        if(*fd > -1)
            ret = dup2(*fd, STDOUT_FILENO) != -1;
    }
    else if(strcmp(op, ">>") == 0 || strcmp(op, "1>>") == 0){
        *std = STDOUT_FILENO;
        *fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0660);
        if(*fd > -1)
            ret = dup2(*fd, STDOUT_FILENO) != -1;
    }
    else if(strcmp(op, "2>") == 0){
        *std = STDERR_FILENO;
        *fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0660);
        if(*fd > -1)
            ret = dup2(*fd, STDERR_FILENO) != -1;
    }
    else if(strcmp(op, "2>>") == 0){
        *std = STDERR_FILENO;
        *fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0660);
        if(*fd > -1)
            ret = dup2(*fd, STDERR_FILENO) != -1;
    }

    return ret;
}

int mysystem(const char* command){

    if(fork() == 0){

        bool is_valid = true;
        bool is_set[3] = { false, false, false }; //if std(in|out|err) were redirected
        int fds[3];

        char* str = strdup(command);
        const char* end = str + strlen(str);


        char* ptr = str;
        int arg_count = 0;

        while(ptr < end && *ptr == ' '){
            *ptr = '\0';
            ++ptr;
        }

        if(ptr < end)
            ++arg_count;

        while(ptr < end && (ptr = strchr(ptr, ' '))){
            while(*ptr == ' '){
                *ptr = '\0';
                ++ptr;
            }
            ++arg_count;
        }

        ptr = str;

        char** args = calloc((size_t) arg_count + 1, sizeof *args);
        int c = 0;

        while(ptr && ptr < end && c < arg_count){

            if(!is_operand(ptr))
                args[c++] = ptr;

            else{
                const char* operator = ptr;
                ptr = strchr(ptr, '\0');
                while(ptr && *ptr == '\0' && ptr < end)
                    ++ptr;

                int fd, std;
                if(set_redir(operator, ptr, &fd, &std)){
                    if(is_set[std])
                        close(fds[std]);

                    is_set[std] = true;
                    fds[std] = fd;
                }
                else{
                    is_valid = false;
                    break;
                }
            }

            if(ptr && ptr < end){
                ptr = strchr(ptr, '\0');
                while(ptr && *ptr == '\0' && ptr < end)
                    ++ptr;
            }
        }

        int status = 0;

        while(c < arg_count)
            args[c++] = NULL;



        if(is_valid){

            bool wait_for_child = true;

            if(strcmp(args[c - 1], "&") == 0){
                args[c - 1] = NULL;
                wait_for_child = false;
            }

            if(fork() == 0){

                const int exit_code = execvp(args[0], args);

                char msg[256];
                snprintf(msg, 256, "-bash: command not found\n");
                write(STDERR_FILENO, msg, strlen(msg));

                _exit(exit_code);
            }
            else if(wait_for_child)
                wait(&status);

            for(int i = 0; i < 3; ++i)
                if(is_set[i])
                    close(fds[i]);
        }

        else{
            const char* err_msg = "Invalid command.\n";
            write(STDERR_FILENO, err_msg, strlen(err_msg));
            status = -1;
        }

        free(args);
        free(str);
        _exit(status);
    }
    else{
        int status;
        wait(&status);
        if(WIFEXITED(status))
            return WEXITSTATUS(status) == 255 ? -1 : WEXITSTATUS(status);
        else
            return -1;
    }
}