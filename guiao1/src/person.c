#include "person.h"

typedef struct person {
    int age;
    char name[PERSON_NAME_LEN];     //must use a static array or else
} *Person;                          //you'll end writing a pointer, which will become invalid
                                    //in the future and that means seg fault

Person new_person(int age, const char* name){
    Person p = malloc(sizeof *p);
    if(p){
        p->age = age;
        strncpy(p->name, name ? name : "", PERSON_NAME_LEN - 1); //save space for '\0'
    }
    return p;
}

void delete_person(Person p){
    free(p);
}

int add_person(int age, const char* name, const char* filename){

    int exit_code = 0;
    const int fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0660);

    if(fd == -1)
        exit_code = FAILED_TO_OPEN_FILE;
    else{

        Person p = new_person(age, name);
        if(p){
            exit_code = write(fd, p, sizeof *p) > -1 ? exit_code : IO_ERROR;

            if(exit_code == 0){
                char str[40];
                const ssize_t offset = lseek(fd, 0, SEEK_CUR);

                if(offset >= 0){
                    snprintf(str, 40, "registo %llu\n", ((size_t) offset / sizeof *p) - 1);
                    exit_code = write(STDOUT_FILENO, str, strlen(str)) > -1 ? exit_code : IO_ERROR;
                }
                else
                    exit_code = IO_ERROR;
            }
        }
        else
            exit_code = OUT_OF_MEMORY;

        delete_person(p);
        close(fd);
    }

    return exit_code;
}


int update_age(int age, const char* name, const char* filename){

    int exit_code = 0;
    const int fd = open(filename, O_RDWR);

    if(fd == -1)
        exit_code = FAILED_TO_OPEN_FILE;
    else{

        Person p = new_person(0, NULL);

        if(p){

            int bytes_read = 0;

            do{
                bytes_read = read(fd, p, sizeof *p);
            }
            while(bytes_read > 0 && strcmp(p->name, name) != 0);

            if(bytes_read > 0){
                p->age = age;
                if(lseek(fd, - (long) (sizeof *p), SEEK_CUR) >= 0)
                    exit_code = write(fd, p, sizeof *p) > -1 ? exit_code : IO_ERROR;
                else
                    exit_code = IO_ERROR;
            }
            else if(bytes_read < 0)
                exit_code = IO_ERROR;

            delete_person(p);
        }
        close(fd);
    }

    return exit_code;
}

int get_age(const char* name, const char* filename){

    int exit_code = 0;
    const int fd = open(filename, O_RDONLY);

    if(fd == -1)
        exit_code = FAILED_TO_OPEN_FILE;
    else{

        Person p = new_person(0, NULL);

        if(p){

            int read_bytes = 0;

            do{
                read_bytes = read(fd, p, sizeof *p);
            }
            while(read_bytes > 0 && strcmp(name, p->name) != 0);

            if(read_bytes < 0)
                exit_code = IO_ERROR;
            else if(strcmp(name, p->name) == 0){
                char str_age[256];
                snprintf(str_age, 256, "%s's age is %d\n", name, p->age);
                write(STDOUT_FILENO, str_age, strlen(str_age));
            }

            free(p);
        }

        close(fd);
    }

    return exit_code;
}

int update_age_index(int age, int index, const char* filename){

    int exit_code = 0;
    const int fd = open(filename, O_RDWR);

    if(fd == -1)
        exit_code = FAILED_TO_OPEN_FILE;
    else{

        if(index < 0)
            exit_code = BAD_INDEX;
        else{
            Person p = new_person(0, "");

            if(p){
                size_t offset = (size_t) index * sizeof *p;
                exit_code = lseek(fd, offset, SEEK_SET) > -1 ? exit_code : IO_ERROR;
                exit_code = read(fd, p, sizeof *p) == sizeof *p ? exit_code : IO_ERROR;

                p->age = age;
                exit_code = lseek(fd, - (long) (sizeof *p), SEEK_CUR) > -1 ? exit_code : IO_ERROR;

                if(exit_code == 0)
                    exit_code = write(fd, p, sizeof *p) == sizeof *p ? exit_code : IO_ERROR;

                delete_person(p);
            }
            else
                exit_code = OUT_OF_MEMORY;
        }

        close(fd);
    }

    return exit_code;
}
