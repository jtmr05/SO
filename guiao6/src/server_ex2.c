#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(void){

    int code = 0;

    char const *fifo = "fifo";
    if(mkfifo(fifo, 0660) == -1){
        code = 1;
        goto FREE0;
    }


    const int fifo_fd = open(fifo, O_RDONLY);
    if(fifo_fd < 0){
        code = 2;
        goto FREE1;
    }


    const int out_fd = open("log.txt", O_WRONLY | O_TRUNC | O_CREAT, 0660);
    if(out_fd < 0){
        code = 3;
        goto FREE2;
    }

    size_t const size = 1024;
    ssize_t bytes_read = 0;

    char *buffer = calloc(sizeof *buffer, size);
    if(!buffer){
        code = 4;
        goto FREE3;
    }

    while(1)
        while((bytes_read = read(fifo_fd, buffer, size)) > 0){
            write(out_fd, buffer, (size_t) bytes_read);
            write(out_fd, "\n", 1);
        }


    free(buffer);
FREE3:
    close(out_fd);
FREE2:
    close(fifo_fd);
FREE1:
    unlink(fifo);
FREE0:

    return code;
}