from os import listdir
from os.path import isfile, join
import sys
import subprocess


if __name__ == "__main__":
    dir_raw = sys.argv[1]
    dir_executable = sys.argv[2]
    files_raw = [f for f in listdir(dir_raw) if isfile(join(dir_raw, f))]

    for file in files_raw:
        file_wo_cpp = file.split(".cpp")[0]
        subprocess.run(["g++-12", "-o", f"{dir_executable}/{file_wo_cpp}", join(dir_raw, file), "-std=c++20"])
    
    files_executable = [f for f in listdir(dir_executable) if isfile(join(dir_executable, f))]

    for file in files_executable:
        # print([join(dir_executable, file)])
        subprocess.run([join(dir_executable, file)])