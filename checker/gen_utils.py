import random
from tqdm import tqdm
from build_run import generate, regenerate
from build_run import build_my_
from build_run import my_exe
import os
import shutil
import cyaron


def iseq(n, l_bound, r_bound):
    return cyaron.Sequence(lambda i, f: random.randint(l_bound, r_bound)).get(1, n)


def random_seq(a, b, n):
    return [random.randint(a, b) for _ in range(n)]


def gen_uoj_tests(prefix, n, time_limit, gen_test):
    cwd = os.getcwdb()
    print("Current Working Directory:", cwd)
    generate("Release")
    if build_my_():
        print("Build Failed")
        return
    tests_dir = os.path.join("tests", prefix)
    os.makedirs(tests_dir, exist_ok=True)
    for i in tqdm(range(1, n + 1)):
        in_file = os.path.abspath(os.path.join(tests_dir, "data" + str(i) + '.in'))
        out_file = os.path.abspath(os.path.join(tests_dir, "data" + str(i) + '.out'))
        gen_test(in_file, i)
        cmd = my_exe + ' < ' + in_file + ' > ' + out_file
        if os.system(cmd):
            print("Runtime error on test", i)
            return

    conf = dict()
    conf["use_builtin_judger"] = "on"
    conf["use_builtin_checker"] = "ncmp"
    conf["input_suf"] = "in"
    conf["output_suf"] = "out"
    conf["input_pre"] = "data"
    conf["output_pre"] = "data"
    conf["use_builtin_checker"] = "lcmp"
    conf["memory_limit"] = 1024
    conf["output_limit"] = 128
    conf["time_limit"] = time_limit
    conf["n_sample_tests"] = 0
    conf["n_tests"] = n
    conf["n_ex_tests"] = 0

    config_file = os.path.join(tests_dir, 'problem.conf')
    with open(config_file, 'w') as f:
        for k, v in conf.items():
            print(k, v, file=f)
    shutil.make_archive(tests_dir, 'zip', tests_dir)


def gen_tests(zip_name, file_name_prefix, n, gen_test):
    cwd = os.getcwdb()
    print("Current Working Directory:", cwd)
    generate("Release")
    if build_my_():
        print("Build Failed")
        return
    tests_dir = os.path.join("tests", zip_name)
    if os.path.exists(tests_dir):
        shutil.rmtree(tests_dir)
    os.makedirs(tests_dir)
    for i in tqdm(range(1, n + 1)):
        in_file = os.path.abspath(os.path.join(tests_dir, file_name_prefix + str(i) + '.in'))
        out_file = os.path.abspath(os.path.join(tests_dir, file_name_prefix + str(i) + '.out'))
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
