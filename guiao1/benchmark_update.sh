#!/bin/bash

PROG=bin/a.out
LOG_FILE=resources/person_update_log.txt
UPPER=10000

RANDOM=$$
RANGE=$((70+1))

random(){
    return $(($(($RANDOM%$RANGE))))
}


echo "" > $LOG_FILE
make
begin=$(date +%s%3N)


for (( i=1; i <= $UPPER; i += 1 ));
do

    if [ $((i % 100)) -eq 0 ]
    then
        tmp=$(date +%s%3N)
        diff=$((tmp - begin))

        echo "$diff ms" >> $LOG_FILE
        begin=$tmp
    fi

    random
    age=$?

    $PROG 5 $i $age -o

    # { time $PROG 5 $i $age -o; } 2>> $LOG_FILE #> /dev/null
done