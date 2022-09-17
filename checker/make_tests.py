import random

from cyaron import IO
from random import randint
from gen_utils import iseq
from gen_utils import gen_tests


def gen_test(in_file, test_id):
    f_ = open(in_file, 'w+')
    f_.close()
    f = IO(in_file)  # .out是临时文件
    # 以下是造数据的代码
    maxn = 1000
    if test_id < 4:
        n = randint(2, 10)
        k = randint(1, 10)
    else:
        n = randint(2, maxn)
        k = randint(1, maxn)

    f.input_writeln(n, k)

    for _ in range(n):
        p = random.sample(range(k + 1), k + 1)
        f.input_writeln(p)


gen_tests("springOuting", 0, 20, gen_test)
