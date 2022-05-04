#include <stdio.h>
#include <fcntl.h>


int main(void){

    const char* fifo = "fifo";
    if(mkfifo(fifo, 0660) == -1){
        fprintf(stderr, "Error creating fifo.\n");
        return -1;
    }

    return 0;
}