
import subprocess
import os
import json

# define the shell command used to extract the efficiency
cmd = 'root -b -q -l -e ".x analysis/counter.C"'

# loop over the range of excitation energies

    # set the new excitation energy in the nuclear_states.json

    # run the geant4 program

    # call the shell command record the efficiency

    # repeat
result = subprocess.run(cmd, shell=True, capture_output=True).stdout

print(float(result))

# put the excitation energy back to the original value

# plot or store the efficiency curve