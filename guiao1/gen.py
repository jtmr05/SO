#
# generate a file with lim lines
# used to test cp performance
#

lim = 10 ** 6

def fill(filename):
    file = open(filename, 'w', encoding='utf8')
    for i in range(1,lim):
        file.write("abcdefghijklmnopqrstuvwxyz\n")
    file.close()

fill("resources/lines.txt")