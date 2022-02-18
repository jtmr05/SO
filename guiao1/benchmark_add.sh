#!/bin/bash

PROG=bin/program.exe
LOG_FILE=resources/person_bench_add.txt
UPPER=10000

RANDOM=$$
RANGE=$((62+1))
LENGTH=20
ABC="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

random(){
    return $(($(($RANDOM%$RANGE))))
}


echo "" > $LOG_FILE
make

for (( i=1; i <= $UPPER; i += 1 ));
do
    str=""
    for (( j=1; j <= $LENGTH; j += 1 ));
    do
        random
        ind=$?
        str+=${ABC:$ind:1}
    done

    echo -e "$i: " >> $LOG_FILE
    random
    { time $PROG 5 $str $? -i; } 2>> $LOG_FILE #> /dev/null
    echo -e "\n\n" >> $LOG_FILE
done