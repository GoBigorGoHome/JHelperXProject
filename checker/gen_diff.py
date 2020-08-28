from typing import Sequence
from cyaron import *
from build import *
from tqdm import tqdm
from functools import partial
import random


# 洛谷 cyaron 的文档
# https://github.com/luogu-dev/cyaron/wiki/
# String.random(N, charset="#.")
# How to generate a permutation of 1..n.
# p = list(range(1, n + 1))
# random.shuffle(p)
# How to generate a sequence of integers.
# seq = Sequence(lambda i, f: randint(1, 10))
# s = seq.get(1, 100)
# 生成无重边、无自环的连通无向图
# graph = Graph.UDAG(n, m, self_loop=False, repeated_edges=False)
# input_file.input_writeln(graph.to_str(output=Edge.unweighted_edge))


def ri(f):
    return map(int, f.readline().split())


def rf(f):
    return map(float, f.readline().split())


def rcmp(epsilon):
    with open(ac_output) as ac_out:
        x, = rf(ac_out)

    with open(my_output, "r") as my_out:
        y, = rf(my_out)
        error = abs(y - x) / max(1, abs(y))
        if error > epsilon:
            print("expected: {:.8f}, found: {:.8f},  error = {:.8f}".format(x, y, error))
            return False
        return True


def special_judge():
    with open(input_file_path, "r") as input:
        n, = ri(input)
        colors = []
        for i in range(n):
            colors += ri(input)
        assert len(colors) == 2 * n

    with open(ac_output) as ac_out:
        m, = ri(ac_out)

    with open(my_output, "r") as my_out:
        t, = ri(my_out)
        if t != m:
            print("maximum beauty differs")
            return False

        index = [x - 1 for x in ri(my_out)]
        if len(index) != 2 * n:
            print("length differs")
            return False
        if len(set(index)) != 2 * n:
            print("duplicate index")
            return False
        for i in range(0, 2 * n, 2):
            if index[i] ^ 1 != index[i + 1]:
                print("index error")
                return False
        for i in range(2, 2 * n, 2):
            assert (colors[index[i]] ^ colors[index[i - 1]]) % (1 << m) == 0
        assert (colors[index[0]] ^ colors[index[2 * n - 1]]) % (1 << m) == 0
        return True


def gen_input():
    input_file = IO(input_file_path)  # .out是临时文件
    n = randint(1, 5)
    input_file.input_writeln(n)
    input_file.input_writeln(String.random(n, charset='01?'))


def duipai(**kwargs):
    build_all()
    spj = kwargs.get('spj', None)
    for i in tqdm(range(1000)):
        gen_input()
        if os.system(run_both):
            print("Runtime Error!")
            break
        if spj is not None:
            if not spj():
                break
        elif os.system(diff_to_nul):
            os.system(diff)
            break


def stress_compare():
    build_all()
    while True:
        gen_input()
        ret = os.system(run_both)
        if ret != 0:
            print("RUNTIME ERROR", ret)
            break
        else:
            print("OK")


def stress_my():
    generate("Debug")
    build_my()
    while True:
        gen_input()
        ret = os.system(run_my)
        if ret != 0:
            print("RUNTIME ERROR", ret)
            break
        else:
            print("OK")


gen_input()
# duipai()
# duipai(spj=partial(rcmp, 1E-6))
# duipai(spj=special_judge)
# stress_compare()
# stress_my()
# build_and_run_once()
# regenerate()
