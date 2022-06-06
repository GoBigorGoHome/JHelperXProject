from random import randint
from build_run import *


def random_seq(a, b, n):
    return [randint(a, b) for _ in range(n)]


def gen_tests(prefix, b, e, gen_test):
    generate("Debug")
    build_my_()
    tests_dir = os.path.join("tests", prefix)
    os.makedirs(tests_dir, exist_ok=True)
    for i in range(b, e):
        in_file = os.path.abspath(os.path.join(tests_dir, prefix + str(i) + '.in'))
        out_file = os.path.abspath(os.path.join(tests_dir, prefix + str(i) + '.out'))
        gen_test(in_file, i)
        cmd = my_exe + ' < ' + in_file + ' > ' + out_file
        os.system(cmd)
    shutil.make_archive(tests_dir, 'zip', tests_dir)
