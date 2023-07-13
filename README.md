# RESONEUT-G4 

I prefer to use Visual Studio Code
 * run: Ctrl + Shift + P to select CMake configure and the CMake build, use Release not Debug.
 * run: cmake --build ./build --target clean, this will clean up the build directory. 
 * run: rsync -auvn --delete SRC/ DES/ to sync the project around as dry run, remove the -n option to actually sync files.


## Requirements
 * CMAKE
 * GEANT4 11.0
 * ROOT-CERN
 * GCC 

### TO-DO
 * Add an configuration file reader json. 
 * Add masslookup class to the source.
 * Add a particle rxn generator (kinematics).
 * Complete geometry setup. 

### Referencing Geant4 Example B5, RunAction and run2.mac to produce multiple .root output files
```
/analysis/setFileName data/test1-hist
/analysis/ntuple/setFileName 0 data/test1-tuple
/gun/particle gamma
/gun/energy 662. keV
/run/beamOn 100000
```

### To combine rootfiles with the same Ntuple use the following command
```
./hadd combined-file.root file1.root file2.root
```


