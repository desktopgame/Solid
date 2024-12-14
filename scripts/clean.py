import glob
import os

build_scripts = glob.glob("./**/CMakeCache.txt", recursive=True)
for build_script in build_scripts:
    os.remove(build_script)