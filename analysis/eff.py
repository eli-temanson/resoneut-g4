
import subprocess
import os
import json
import csv
import numpy as np
import matplotlib.pyplot as plt

# define the shell command used to extract the efficiency
cmd = 'root -b -q -l -e ".x analysis/counter.C"'

# Read Existing JSON File
file = open('assets/nuclear_states.json','r')
data = json.load(file)
file.close()

eff = []
ex = []

# loop over the range of excitation energies
e_original = data["Isotopes"][0]["States"][0]["Energy"]
threshold = data["Isotopes"][0]["Thresholds"][0]["Energy"] + 0.5

for eindex in range(20) :
    # set the new excitation energy in the nuclear_states.json
    energy = threshold + (1.5*eindex)/20.0
    data["Isotopes"][0]["States"][0].update({"Energy" : energy}) 
    print(data["Isotopes"][0]["States"][0]["Energy"])

    file = open('assets/nuclear_states.json','w')
    json.dump(data, file, indent=2) 
    file.close()

    # run the geant4 program
    os.system('./kinematics-det/build/kinematics-det assets/input.json')

    # call the shell command record the efficiency
    result = subprocess.run(cmd, shell=True, capture_output=True).stdout
    # print(float(result))
    eff.append(float(result))
    ex.append(energy)

    # repeat

# put the excitation energy back to the original value
file = open('assets/nuclear_states.json','w')
data["Isotopes"][0]["States"][0].update({"Energy" : e_original}) #8.699
json.dump(data, file, indent=2)
file.close()

# plot or store the efficiency curve
csv_eff = open("analysis/eff.csv", "w")
writer = csv.writer(csv_eff, delimiter=',')
for i in range(len(eff)):
    writer.writerow((ex[i], eff[i]))
    
csv_eff.close()

plt.plot(ex, eff)
plt.show()