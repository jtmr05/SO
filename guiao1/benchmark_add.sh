#!/bin/bash

PROG=bin/a.out
LOG_FILE=resources/person_add_log.txt
UPPER=10000

RANDOM=$$
RANGE=$((62+1))
#LENGTH=20
#ABC="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

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


    str="person$i"
    #for (( j=1; j <= $LENGTH; j += 1 ));
    #do
    #    random
    #    ind=$?
    #    str+=${ABC:$ind:1}
    #done

    random
    age=$?

    $PROG 5 $str $age -i

    # { time $PROG 5 $str $? -i; } 2>> $LOG_FILE #> /dev/null
done