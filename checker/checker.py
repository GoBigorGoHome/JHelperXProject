from utils import *


def special_judge():
    with open(input_file_path, "r") as input:
        n, = ri(input)
        print(n)
        a = list(ri(input))
        print(a)

    with open(my_output, "r") as my_out:
        res = my_out.readline()
        if res == 'No\n':
            return True
        b = list(ri(my_out))
        print(b)
        for i in range(n):
            if a[i] == b[i]:
                return False
    return True
