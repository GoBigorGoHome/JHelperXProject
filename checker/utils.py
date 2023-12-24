from tqdm import tqdm
import concurrent.futures
from gen import gen_input
from build_run import ac_output, my_output
from build_run import build_all, build_my
from build_run import build_all_, build_my_
from build_run import run_my, run_both, run_ac_
from build_run import diff, diff_to_nul
from build_run import generate, regenerate_
import os
import time


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
    ret = build_all_()
    if ret:
        print("Build failed!")
        return
    spj = kwargs.get('spj', None)
    for _ in tqdm(range(cnt)):
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


def compare():
    build_all_()
    os.system(run_both)
    # if os.system(run_both):
    #     print("Runtime error!")
    #     return
    os.system(diff)


def stress_my(**kwargs):
    spj = kwargs.get('spj', None)
    regenerate_("Release")
    build_my_()
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


def timed_my():
    then = time.time()
    res = os.system(run_my)
    elapsed = time.time() - then
    return elapsed, res


def stress_my_with_tl(time_limit):
    generate("Release")
    build_my()
    while True:
        gen_input()
        with concurrent.futures.ThreadPoolExecutor(max_workers=1) as executor:
            future = executor.submit(timed_my)
            try:
                elapsed, result = future.result(timeout=time_limit)
                print("finished in", elapsed, "seconds")
            except concurrent.futures.TimeoutError:
                print("Time out!")
                return


def stress_ac(**kwargs):
    spj = kwargs.get('spj', None)
    generate("Debug")
    build_all()
    while True:
        gen_input()
        ret = os.system(run_ac_)
        if ret != 0:
            print("RUNTIME ERROR", ret)
            break
        elif spj is not None:
            if not spj():
                print("Wrong Answer")
                return
        else:
            print("OK")
