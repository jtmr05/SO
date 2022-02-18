#!/bin/bash

PROG=bin/program.exe
LOG_FILE=resources/person_bench_update.txt
UPPER=1500

RANDOM=$$
RANGE=$((70+1))

random(){
    return $(($(($RANDOM%$RANGE))))
}


echo "" > $LOG_FILE
make

for (( i=1; i <= $UPPER; i += 1 ));
do

    echo -e "$i: " >> $LOG_FILE

    random
    age=$?

    { time $PROG 5 $i $age -o; } 2>> $LOG_FILE #> /dev/null
    echo -e "\n\n" >> $LOG_FILE
done