#include "exs.h"

int cp(const char* src, const char* dst, size_t buffer_size){

    int exit_code = 0;

    const int src_fd = open(src, O_RDONLY);

    if(src_fd == -1)
        exit_code = FAILED_TO_OPEN_FILE;
    else{

        const int dst_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0640);

        if(dst_fd == -1){
            close(src_fd);
            exit_code = FAILED_TO_OPEN_FILE;
        }
        else{

            char *buffer = malloc(sizeof *buffer * buffer_size);
            if(buffer){
                int bytes_read = 0;

                while((bytes_read = read(src_fd, buffer, buffer_size)) > 0)
                    exit_code = write(dst_fd, buffer, (unsigned int) bytes_read) > -1 ? exit_code : IO_ERROR;

                if(bytes_read < 0)
                    exit_code = IO_ERROR;
            }
            else
                exit_code = OUT_OF_MEMORY;

            close(src_fd);
            close(dst_fd);
        }
    }

    return exit_code;
}

int cat(){

    int exit_code = 0;

    char buffer[DEFAULT_BUFFER_SIZE];
    int bytes_read = 0;

    while((bytes_read = read(STDIN_FILENO, buffer, DEFAULT_BUFFER_SIZE)) > 0)
        exit_code = write(STDOUT_FILENO, buffer, (unsigned int) bytes_read) > -1 ? exit_code : IO_ERROR;

    if(bytes_read < 0)
        exit_code = IO_ERROR;

    return exit_code;
}

/** reads up to size - 1 bytes the read string with a null byte */
ssize_t readln(int fd, char *line, size_t size){

    *line = '\0';                               //in case nothing is read
    int bytes_read = 0;

    if(size > 0)
        bytes_read = read(fd, line, size - 1); //reserve 1 byte for null char

    if(bytes_read > 0){
        char* ptr = strchr(line, '\n');

        if(!ptr)
            line[bytes_read] = '\0';
        else
            *ptr = '\0';
    }

    return bytes_read;
}

/*
ssize_t readln(int fd, char *line, size_t size){

    size_t i;

    for(i = 0; i < size - 1; i++){
        if(read(fd, &line[i], 1) == 1){
            if(line[i] == '\n')
                break;
        }
        else
            break;
    }
    line[i] = '\0';

    return (ssize_t) i;
}
*/

int println(const char* file){
    int exit_code = 0;

    const int fd = open(file, O_RDONLY);

    if(fd == -1)
        exit_code = FAILED_TO_OPEN_FILE;
    else{
        char buffer[DEFAULT_BUFFER_SIZE];

        if(readln(fd, buffer, DEFAULT_BUFFER_SIZE) > 0){
            write(STDOUT_FILENO, buffer, strlen(buffer));
            write(STDOUT_FILENO, "\n", 1);
        }

        close(fd);
    }

    return exit_code;
}

int nl(){

    int exit_code = 0;
    unsigned int current_line = 1;

    size_t size = DEFAULT_BUFFER_SIZE;
    char* buffer = malloc(sizeof *buffer * size);
    char* ptr = buffer;

    while(buffer && (readln(STDIN_FILENO, ptr, size) > 0)){
        size_t len = strlen(buffer);

        if(len == size - 1){
            buffer = realloc(buffer, sizeof *buffer * 2 * size);
            ptr = buffer + size - 1;
            size *= 2;
        }
        else{
            const size_t line_info_size = 16;
            char line_info[line_info_size];

            snprintf(line_info, line_info_size, "\t %u. ", current_line++);
            write(STDOUT_FILENO, line_info, strlen(line_info));
            write(STDOUT_FILENO, buffer, len);
            write(STDOUT_FILENO, "\n", 1);
            ptr = buffer;
        }
    }

    if(!buffer)
        exit_code = OUT_OF_MEMORY;

    free(buffer);

    return exit_code;
}