#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, const char** argv){

    char const *fifo = "fifo";
    int const fd = open(fifo, O_WRONLY);

    if(fd < 0)
        return -1;

    for(int i = 1; i < argc; ++i)
        write(fd, argv[i], strlen(argv[i]));

    close(fd);

    return 0;
}