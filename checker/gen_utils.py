import random
from tqdm import tqdm
from build_run import generate
from build_run import build_my_
from build_run import my_exe
import os
import shutil
import cyaron


def iseq(n, l_bound, r_bound):
    return cyaron.Sequence(lambda i, f: random.randint(l_bound, r_bound)).get(1, n)


def random_seq(a, b, n):
    return [random.randint(a, b) for _ in range(n)]


def gen_tests(prefix, b, e, gen_test):
    generate("Debug")
    if build_my_():
        print("Build Failed")
        return
    tests_dir = os.path.join("tests", prefix)
    os.makedirs(tests_dir, exist_ok=True)
    for i in tqdm(range(b, e)):
        in_file = os.path.abspath(os.path.join(tests_dir, prefix + str(i) + '.in'))
        out_file = os.path.abspath(os.path.join(tests_dir, prefix + str(i) + '.out'))
        gen_test(in_file, i)
        cmd = my_exe + ' < ' + in_file + ' > ' + out_file
        if os.system(cmd):
            print("Runtime error on test", i)
            return
    shutil.make_archive(tests_dir, 'zip', tests_dir)


def constrained_sum_sample_pos(n, total):
    """Return a randomly chosen list of n positive integers summing to total.
    Each such list is equally likely to occur."""

    dividers = sorted(random.sample(range(1, total), n - 1))
    return [a - b for a, b in zip(dividers + [total], [0] + dividers)]