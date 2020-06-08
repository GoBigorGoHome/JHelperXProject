import os
import shutil
import time

input_file_path = r"in.txt"
my_output = r"my_out.txt"
ac_output = r"ac_out.txt"

source_tree = r"C:\Users\zjsdu\Downloads\jhelper-example-project-master\checker"
build_tree = r"C:\Users\zjsdu\Downloads\jhelper-example-project-master\checker\build"

my_exe = os.path.join(build_tree, "my.exe")
ac_exe = os.path.join(build_tree, "ac.exe")

cmd1 = my_exe + r" < " + input_file_path + r" > " + my_output
cmd2 = ac_exe + r" <" + input_file_path + r" > " + ac_output
diff = r"fc /A /N " + my_output + " " + ac_output
diff_to_nul = diff + " > NUL"
cmd = cmd1 + ' && ' + cmd2

generator_mingw = "CodeBlocks - MinGW Makefiles"
generator_ninja = "Ninja"
generate_build_system = "cmake" + " --no-warn-unused-cli" + " -DCMAKE_BUILD_TYPE=Release" + " -DDUIPAI=1" + \
                        " -G \"" + generator_ninja + "\"" + " -S " + source_tree + " -B " + \
                        build_tree + " > NUL"


def clear_folder(folder):
    for filename in os.listdir(folder):
        file_path = os.path.join(folder, filename)
        try:
            if os.path.isfile(file_path) or os.path.islink(file_path):
                os.unlink(file_path)
            elif os.path.isdir(file_path):
                shutil.rmtree(file_path)
        except Exception as e:
            print('Failed to delete %s. Reason: %s' % (file_path, e))


def clean_generate():
    clear_folder(build_tree)
    os.system(generate_build_system)


def generate():
    os.system(generate_build_system)


def build_all():
    # generate()
    build_project = "cmake" " --build " + build_tree + " --clean-first" + " --target my ac > NUL"
    os.system(generate_build_system + " && " + build_project)


def build_my():
    # generate()
    build_project = "cmake" " --build " + build_tree + " --clean-first" + " --target my > NUL"
    os.system(generate_build_system + " && " + build_project)


def compare():
    return os.system(diff)


def build_and_run_once():
    build_all()
    os.system(cmd)
    compare()


def run_hack():
    start_time = time.time()
    os.system(cmd2)
    print("took", time.time() - start_time, "to run")
