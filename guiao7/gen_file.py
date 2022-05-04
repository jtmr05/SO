#!/usr/bin/python

import random
import sys

lim = 10 ** 6

def get_random() -> int:
    return random.randrange(1, 10 ** 5)

key = get_random()



def fill(filename):
    file = open(filename, 'w', encoding='utf8')
    for i in range(1, lim):

        if get_random() == key:
            file.write("word\n")
        else:
            file.write("abcdefghijklmnopqrstuvwxyz\n")
    file.close()



if len(sys.argv) < 2:
    print("Not enough arguments!")
else:
    fill(sys.argv[1])