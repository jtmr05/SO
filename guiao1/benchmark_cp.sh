#!/bin/bash

PROG=bin/a.out
LOG_FILE=resources/cp_bench.txt
LIM=1048576

echo "" > $LOG_FILE
make

for (( i=32; i <= $LIM; i *= 2 ));
do
    echo -e "buffer with size $i has the following performace:" >> $LOG_FILE
    { time $PROG 1 resources/lines.txt resources/lines2.txt $i; } 2>> $LOG_FILE
    echo -e "\n\n" >> $LOG_FILE
done