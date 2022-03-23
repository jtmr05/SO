#!/bin/bash


echo -e """#!/bin/bash

RANGE=\$((5))

random(){
    VALUE=\$((\$((\$RANDOM%\$RANGE))))
    #echo \$VALUE
    echo "\$0: \$VALUE" >> nums.txt
    return \$VALUE
}

random""" > a.sh

cp a.sh b.sh
cp a.sh c.sh
chmod +x a.sh
chmod +x b.sh
chmod +x c.sh
