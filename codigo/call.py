import os
import subprocess

for filename in os.listdir(os.getcwd()):   
    print (filename)
    proc = subprocess.Popen(["./main.cpp", filename])
    proc.wait()
