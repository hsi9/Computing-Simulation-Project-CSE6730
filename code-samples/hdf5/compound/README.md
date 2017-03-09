# Compound Datatype HDF5 Test Code

This example set demonstrates HDF5 use where all datasets are 1D arrays of structs.

Note that for 10M atoms, the disk overhead is ~40MB compared to the
struct-of-arrays approach to writing the HDF5 file.  This is not necessarily a problem
until trajectory snapshotting is taken into account (the simulation runs in picosecond
timesteps; the entire simulation runs in the order of hundreds of nanoseconds to milliseconds,
and snaphots are taken every 10 to 50 steps).

## Generate the H5 File:

```bash
python test.py
```

Open the resulting HDF5 file in HDFView.


## Read one Dataset from the H5 File:

```bash
# Adjust the H5 library linking for your OS platform!
c++ -std=c++11  -L/usr/local/opt/hdf5/lib -lhdf5 -lhdf5_cpp test.cpp

./a.out
```