#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void){

    int code = 0;



    const char* fifo = "fifo";

    const int fifo_fd = open(fifo, O_RDONLY);
    if(fifo_fd < 0){
        code = 1;
        goto FREE0;
    }

    const size_t size = 1024;
    char *buffer = malloc(sizeof *buffer * size);
    if(!buffer){
        code = 2;
        goto FREE1;
    }

    ssize_t b_read = 0;
    while((b_read = read(fifo_fd, buffer, size)) > 0)
        write(STDOUT_FILENO, buffer, (size_t) b_read);

    free(buffer);
FREE1:
    close(fifo_fd);
FREE0:

    return code;
}