import matplotlib.pyplot as plt
import numpy as np
import time
import os
import subprocess
import json


dic = {
    "inputAll"      : "./sequences/all",
    "inputParallel" : "./sequences/exaus",
    "smith"         : "./smith-waterman/main",
    "local"         : "./busca-local/main",
    "exaus"         : "./busca-exaustiva/main",
    "localParallel" : "./busca-local-paralelo/main",
    "gpuParallel"   : "./paralelo-gpu/main"
}

def runWithInput(exe, input_file):
    with open(input_file) as file:
        start = time.perf_counter()
        proc = subprocess.run([exe], input=file.read(),text=True, capture_output=True)
        end = time.perf_counter()
    return end-start

def getInfo(algorithm, path):
    dic_temp = {
        "times" : [],
        "sizes_a" : [],
        "sizes_b" : []
    }
    files = os.listdir(path)
    
    for file in files:
        dic_temp["times"].append(runWithInput(algorithm, f"{path}/{file}"))
        with open(f"{path}/{file}") as f:
            split = f.read().splitlines()
        dic_temp["sizes_a"].append(int(split[0]))
        dic_temp["sizes_b"].append(int(split[1]))
        
    return dic_temp

# dicSmith = getInfo(dic["smith"], dic["inputAll"])
# print("smith foi")
# dicLocal = getInfo(dic["local"], dic["inputParallel"])
# print("local foi")
dicExaus = getInfo(dic["exaus"], dic["inputParallel"])
# print("exaustiva foi")
# dicParLocal = getInfo(dic["localParallel"], dic["inputParallel"])
# print("Aqui foi!")

# dicResult = {"Smith"    : dicSmith,
#              "Local"    : dicLocal,
#              "Exaus"    : dicExaus}
# dicResult={             "ParLocal" : dicParLocal,
#              "ParGPU"   : dicParGPU}


import json
with open('datatest.json', 'w') as f:
    json.dump(dicExaus, f, indent=4)

