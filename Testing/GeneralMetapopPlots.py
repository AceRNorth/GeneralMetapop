import pandas as pd
import os
import subprocess
import numpy as np
import matplotlib.pyplot as plt

#%% Read input parameter set from .csv file
params = pd.read_table("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\Parameters.csv", delimiter=",")
labels = params[["Parameter"]]
labels = labels.drop(index=0)
input_data = params[["set 1"]]
input_data = input_data.drop(index=0)
input_data = input_data.rename(dict(zip(input_data.index, labels["Parameter"])), axis=0)
display(input_data)
input_list = input_data["set 1"].tolist()
input_string = " ".join(str(i) for i in input_list)

# Run C++ model with input data
os.chdir("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\output files")
proc = subprocess.Popen(["C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\GeneralMetapop.exe"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
outs, errs = proc.communicate(input=input_string)

#%% Plot global output (totals) from model
os.chdir("C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\testing\\current version set10 data")

totals = np.loadtxt("Totals1run1.txt", skiprows=2)

times = totals[:, 0]
total_males = totals[:, 1:]

plt.figure()
plt.title("Total males across the area - set 10 run " + str(i))
plt.xlabel("Day")
plt.ylabel("Total number of individuals")
plt.plot(times, total_males[:, 0], label="$M_{WW}$")
plt.plot(times, total_males[:, 1], label="$M_{WD}$")
plt.plot(times, total_males[:, 2], label="$M_{DD}$")
plt.plot(times, total_males[:, 3], label="$M_{WR}$")
plt.plot(times, total_males[:, 4], label="$M_{RR}$")
plt.plot(times, total_males[:, 5], label="$M_{DR}$")
plt.legend()


