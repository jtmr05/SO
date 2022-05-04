#include <unistd.h>

int main(void){

    const char* fifo = "fifo";
    unlink(fifo);

    return 0;
}