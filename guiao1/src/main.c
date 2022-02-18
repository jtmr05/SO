#include "exs.h"
#include "person.h"
#include <stdbool.h>

void error_handler(const int error_id){

    char* error_message = "";

    switch(error_id){

    case NO_OPTION:
        error_message = "No option specified.\n";
        break;

    case NOT_ENOUGH_ARGS:
        error_message = "Not enough arguments.\n";
        break;

    case UNKNOWN_OPTION:
        error_message = "Unknown option.\n";
        break;

    case OUT_OF_MEMORY:
        error_message = "Failed to alloc memory.\n";
        break;

    case FAILED_TO_OPEN_FILE:
        error_message = "Failed to open file.\n";
        break;

    case IO_ERROR:
        error_message = "I/O error.\n";
        break;

    case BAD_INDEX:
        error_message = "Bad index.\n";
        break;

    default:
        break;
    }

    if(error_id != 0)
        write(STDERR_FILENO, error_message, strlen(error_message));
}

int main(const int argc, char const *argv[]){

    int exit_code = 0;

    if(argc < 2)
        exit_code = NO_OPTION;
    else{

        char *msg = malloc(sizeof *msg * MSG_SIZE);

        if(msg){
            switch(atoi(argv[1])){

            case 1:
                if(argc < 4)
                    exit_code = NOT_ENOUGH_ARGS;
                else{
                    snprintf(msg, MSG_SIZE, "cp %s %s\n\n", argv[2], argv[3]);
                    write(STDOUT_FILENO, msg, strlen(msg));

                    size_t size = 0;
                    if(argc > 4){
                        int tmp = atoi(argv[4]);
                        size = (tmp > 0) ? (size_t) tmp : DEFAULT_BUFFER_SIZE;
                    }

                    exit_code = cp(argv[2], argv[3], size);
                }
                break;

            case 2:
                snprintf(msg, MSG_SIZE, "cat\n\n");
                write(STDOUT_FILENO, msg, strlen(msg));
                exit_code = cat();
                break;

            case 3:
                if(argc < 3)
                    exit_code = NOT_ENOUGH_ARGS;
                else{
                    snprintf(msg, MSG_SIZE, "println %s\n\n", argv[2]);
                    write(STDOUT_FILENO, msg, strlen(msg));
                    exit_code = println(argv[2]);
                }
                break;

            case 4:
                snprintf(msg, MSG_SIZE, "nl\n\n");
                write(STDOUT_FILENO, msg, strlen(msg));
                exit_code = nl();
                break;

            case 5:
                if(argc < 4)
                    exit_code = NOT_ENOUGH_ARGS;
                else{

                    int i = 2;
                    char op;
                    bool found = false;
                    const char *fn = "resources/people.bin";

                    while(i < argc && !found)
                        if(argv[i][0] == '-'){
                            op = argv[i][1];
                            found = true;
                        }
                        else
                            i++;


                    if(found){
                        i = (i == 2) ? i+1 : 2;

                        switch(op){

                        case 'i':
                            if(argc < 5)
                                exit_code = NOT_ENOUGH_ARGS;
                            else{
                                snprintf(msg, MSG_SIZE, "person -%c %s %s\n\n", op, argv[i], argv[i+1]);
                                write(STDOUT_FILENO, msg, strlen(msg));
                                exit_code = add_person(atoi(argv[i+1]), argv[i], fn);
                            }
                            break;

                        case 'u':
                            if(argc < 5)
                                exit_code = NOT_ENOUGH_ARGS;
                            else{
                                snprintf(msg, MSG_SIZE, "person -%c %s %s\n\n", op, argv[i], argv[i+1]);
                                write(STDOUT_FILENO, msg, strlen(msg));
                                exit_code = update_age(atoi(argv[i+1]), argv[i], fn);
                            }
                            break;

                        case 'g':
                            if(argc < 4)
                                exit_code = NOT_ENOUGH_ARGS;
                            else{
                                snprintf(msg, MSG_SIZE, "person -%c %s\n\n", op, argv[i]);
                                write(STDOUT_FILENO, msg, strlen(msg));
                                exit_code = get_age(argv[i], fn);
                            }
                            break;

                        case 'o':
                            if(argc < 4)
                                exit_code = NOT_ENOUGH_ARGS;
                            else{
                                snprintf(msg, MSG_SIZE, "person -%c %s %s\n\n", op, argv[i], argv[i+1]);
                                write(STDOUT_FILENO, msg, strlen(msg));
                                exit_code = update_age_index(atoi(argv[i+1]), atoi(argv[i]), fn);
                            }
                            break;

                        default:
                            exit_code = UNKNOWN_OPTION;
                            break;
                        }
                    }
                }

                break;

            default:
                exit_code = UNKNOWN_OPTION;
                break;
            }

            free(msg);
        }
        else
            exit_code = OUT_OF_MEMORY;
    }

    error_handler(exit_code);

    return exit_code;
}