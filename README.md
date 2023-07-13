# RESONEUT-G4 

## Requirements
 * CMAKE
 * GEANT4 11.0
 * ROOT-CERN
 * GCC 

### TO-DO
 * Add an configuration file reader json. 
 * Add masslookup class to the source.
 * Add a particle rxn generator (kinematics).
   * How do I create two particles for one event?
 * Complete geometry setup. 

### How to use
 1. Always run the executables from the parent resoneut-g4 directory to avoid path errors.

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


