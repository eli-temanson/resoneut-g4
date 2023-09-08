
import subprocess
import os
import json
import csv
import numpy as np
import matplotlib.pyplot as plt

# define the shell command used to extract the efficiency
cmd = 'root -b -q -l -e ".x analysis/counter.C"'

# Read Existing JSON File
file = open('assets/input_eff.json','r')
data = json.load(file)
file.close()

eff = []
en = []

# loop over the range of excitation energies
# e_original = data["Isotopes"][0]["States"][0]["Energy"]
# threshold = data["Isotopes"][0]["Thresholds"][0]["Energy"] + 0.5

e0 = 0.200 # keV start
emax = 1.5
N = 50
dE = (emax-e0)/N
energy = e0 - dE

for eindex in range(N+1) :
    # set the new excitation energy in the nuclear_states.json
    energy += dE
    data["beam"][0]["energyMeV"] = energy 
    print(data["beam"][0]["energyMeV"])

    file = open('assets/input_eff.json','w')
    json.dump(data, file, indent=2) 
    file.close()

    # run the geant4 program
    os.system('./efficiency/build/efficiency assets/input_eff.json')

    # call the shell command record the efficiency
    result = subprocess.run(cmd, shell=True, capture_output=True).stdout
    # print(float(result))
    eff.append(float(result))
    en.append(energy)

    # repeat

# put the excitation energy back to the original value
file = open('assets/input_eff.json','w')
data["beam"][0]["energyMeV"] = e0
json.dump(data, file, indent=2)
file.close()

# plot or store the efficiency curve
csv_eff = open("analysis/neff_thres_200keV.csv", "w")
writer = csv.writer(csv_eff, delimiter=',')
for i in range(len(eff)):
    writer.writerow((en[i], eff[i]))
    
csv_eff.close()

plt.plot(en, eff, '-bD',  c='blue', mfc='red', mec='k')
plt.show()



