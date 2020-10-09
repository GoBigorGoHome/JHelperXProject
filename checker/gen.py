from cyaron import *
from utils import *
from typing import Sequence


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

def gen_input():
    input_file = IO(input_file_path)  # .out是临时文件
    n = randint(1, 10)
    a = random_seq(1, n, n)
    b = random_seq(1, n, n)
    a.sort()
    b.sort()
    input_file.input_writeln(n)
    input_file.input_writeln(a)
    input_file.input_writeln(b)
