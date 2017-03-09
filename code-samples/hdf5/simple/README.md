# Basic HDF5 Test Code

This example set demonstrates HDF5 use where all datasets are primitive 2D arrays.

## Generate the H5 File:

```bash
python test.py
```

Open the resulting HDF5 file in HDFView.


## Read one Dataset from the H5 File:

```bash
# Adjust the H5 library linking for your OS platform!
c++ -std=c++11  -L/usr/local/opt/hdf5/lib -lhdf5 -lhdf5_cpp test.cpp
```