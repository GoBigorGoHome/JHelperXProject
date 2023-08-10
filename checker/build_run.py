import os
import shutil
import sys
import time

input_file_path = r"in.txt"
my_output = r"my_out.txt"
ac_output = r"ac_out.txt"

source_tree = os.path.abspath("../")
build_tree = os.path.abspath("build")

my_exe = os.path.join(build_tree, "my")
ac_exe = os.path.join(build_tree, "ac")
diff_cmd = "diff"
nul = "/dev/null"
if sys.platform == "win32":
    nul = "NUL"
    diff_cmd = "fc /A /N /W"
run_my = my_exe + r" < " + input_file_path + r" > " + my_output + " 2> " + nul
run_ac = ac_exe + r" <" + input_file_path + r" > " + ac_output + " 2> " + nul
run_ac_ = ac_exe + r" <" + input_file_path
diff = diff_cmd + " " + my_output + " " + ac_output
diff_to_nul = diff + " > " + nul + " 2> " + nul
run_both = run_my + ' && ' + run_ac

generator_mingw = "CodeBlocks - MinGW Makefiles"
generator_ninja = "Ninja"



def remove_build_tree():
    if os.path.exists(build_tree):
        shutil.rmtree(build_tree)


def generate(build_type):
    cmd = "cmake --no-warn-unused-cli " + "-DCMAKE_BUILD_TYPE=" + build_type \
          + " -G \"" + generator_ninja + "\"" \
          + " -S " + source_tree + " -B " + build_tree + " > " + nul
    os.system(cmd)


def generate_(build_type):
    cmd = "cmake --no-warn-unused-cli " + "-DCMAKE_BUILD_TYPE=" + build_type \
          + " -G " + generator_ninja \
          + " -S " + source_tree + " -B " + build_tree
    print(cmd)
    os.system(cmd)


def regenerate_(build_type):
    remove_build_tree()
    generate_(build_type)


def regenerate(build_type):
    remove_build_tree()
    generate(build_type)


def build_all():
    build_cmd = "cmake --build " + build_tree + " --clean-first" + " --target my ac > " + nul
    return os.system(build_cmd)


def build_all_():
    build_cmd = "cmake --build " + build_tree + " --clean-first" + " --target my ac"
    os.system(build_cmd)


def build_my():
    build_cmd = "cmake --build " + build_tree + " --clean-first" + " --target my > " + nul
    os.system(build_cmd)


def build_my_():
    build_cmd = "cmake --build " + build_tree + " --clean-first" + " --target my"
    ret = os.system(build_cmd)
    return ret


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
