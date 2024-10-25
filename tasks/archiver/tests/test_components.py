from os import listdir
from os.path import isfile, join
import sys
import subprocess


if __name__ == "__main__":
    dir = sys.argv[1]
    files = [f for f in listdir(dir) if isfile(join(dir, f))]

    for file in files:
        subprocess.run([join(dir, file)])