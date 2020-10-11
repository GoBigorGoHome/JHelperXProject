from random import randint
from build import *
from tqdm import tqdm
from gen import *


def random_seq(a, b, n):
    return [randint(a, b) for i in range(n)]


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


def duipai(cnt, **kwargs):
    build_all()
    spj = kwargs.get('spj', None)
    for i in tqdm(range(cnt)):
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


def stress_my(**kwargs):
    spj = kwargs.get('spj', None)
    generate("Debug")
    build_my()
    while True:
        gen_input()
        ret = os.system(run_my)
        if ret != 0:
            print("RUNTIME ERROR", ret)
            break
        elif spj is not None:
            if not spj():
                print("Wrong Answer")
                return
        else:
            print("OK")
