from random import randint
from build import *
from tqdm import tqdm
from gen import *
import build
import concurrent.futures


def random_seq(a, b, n):
    return [randint(a, b) for _ in range(n)]


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
    ret = build_all()
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


def gen_tests(prefix, b, e):
    build.generate("Debug")
    build.build_my_()
    tests_dir = os.path.join("tests", prefix)
    os.makedirs(tests_dir, exist_ok=True)
    for i in range(b, e):
        in_file = os.path.abspath(os.path.join(tests_dir, prefix + str(i) + '.in'))
        out_file = os.path.abspath(os.path.join(tests_dir, prefix + str(i) + '.out'))
        gen_test(in_file, i)
        cmd = build.my_exe + ' < ' + in_file + ' > ' + out_file
        os.system(cmd)
    shutil.make_archive(tests_dir, 'zip', tests_dir)
