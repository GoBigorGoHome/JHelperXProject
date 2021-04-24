import os
import shutil
import time

input_file_path = r"in.txt"
my_output = r"my_out.txt"
ac_output = r"ac_out.txt"

source_tree = os.path.abspath("../")
build_tree = os.path.abspath("build")

my_exe = os.path.join(build_tree, "my.exe")
ac_exe = os.path.join(build_tree, "ac.exe")

run_my = my_exe + r" < " + input_file_path + r" > " + my_output + " 2> NUL"
run_ac = ac_exe + r" <" + input_file_path + r" > " + ac_output + " 2> NUL"
run_ac_ = ac_exe + r" <" + input_file_path
diff = r"fc /A /N /W " + my_output + " " + ac_output
diff_to_nul = diff + " > NUL 2> NUL"
run_both = run_my + ' && ' + run_ac

generator_mingw = "CodeBlocks - MinGW Makefiles"
generator_ninja = "Ninja"


def remove_build_tree():
    if os.path.exists(build_tree):
        shutil.rmtree(build_tree)


def generate(build_type):
    cmd = "cmake --no-warn-unused-cli " + "-DCMAKE_BUILD_TYPE=" + build_type \
          + " -G \"" + generator_ninja + "\"" \
          + " -S " + source_tree + " -B " + build_tree + " > NUL"
    os.system(cmd)


def regenerate(build_type):
    remove_build_tree()
    generate(build_type)


def build_all():
    build_cmd = "cmake --build " + build_tree + " --clean-first" + " --target my ac > NUL"
    os.system(build_cmd)


def build_all_():
    build_cmd = "cmake --build " + build_tree + " --clean-first" + " --target my ac"
    os.system(build_cmd)


def build_my():
    build_cmd = "cmake --build " + build_tree + " --clean-first" + " --target my > NUL"
    os.system(build_cmd)


def build_my_():
    build_cmd = "cmake --build " + build_tree + " --clean-first" + " --target my"
    os.system(build_cmd)


def compare():
    return os.system(diff)


def build_and_run_once():
    build_all()
    os.system(run_both)
    compare()


def run_hack():
    start_time = time.time()
    os.system(run_ac)
    print("took", time.time() - start_time, "to run")
