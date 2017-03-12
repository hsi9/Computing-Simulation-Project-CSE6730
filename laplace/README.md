# The La+ Program

La+ (pronounced as "Laplace") is a basic MD simulation package.


# Dependencies

* [git](https://git-scm.com/)
* [HDF5](https://support.hdfgroup.org/HDF5/) 1.8+
* An MPI installation, either [OpenMPI](https://www.open-mpi.org/) or [MPICH](https://www.mpich.org/)
* [Doxygen](http://www.stack.nl/~dimitri/doxygen/) 1.8.13+ for generating source code documentation
* A C++14 compiler (i.e. g++ 5.2+, clang 3.7+).

All other library dependencies are downlaoded and built during compile time.

These packages should be installable by the package manager that comes with your operating system.

**Note** that the compiler must generate code that is ABI-compatible with the aforementioned installed libraries.  For example: On Mac OS X, libHDF5 comes as a package that is built by clang.  If I try to compile code that links against a clang-built libHDF5 using the g++, I will most likely get a linker error.  Thus it is preferable to use the default compiler that comes with your OS.



# Build

## CentOS 7



```bash
$ pwd
/laplace

# Create a separate directory for building in order to not pollute the source tree
$ mkdir target && cd target/

# Prepare the build system
# If mpicc and mpic++ are in $PATH, then the MPI_* flags are not necessary
# Set CMAKE_BUILD_TYPE to `Debug` for debugging
# Add --graphviz=graph.dot to generate a source dependency graph
$ cmake -DCMAKE_BUILD_TYPE=Release -DLAPLACE_TESTS=1 -DLAPLACE_DOCS=1 -DMPI_C_LIBRARIES=/usr/lib64/openmpi/lib/libmpi.so -DMPI_C_INCLUDE_PATH=/usr/include/openmpi-x86_64 -DMPI_CXX_LIBRARIES=/usr/lib64/openmpi/lib/libmpi_cxx.so -DMPI_CXX_INCLUDE_PATH=/usr/include/openmpi-x86_64 ..

# Build libraries and programs
# Add -j<N> for parallelized build, where N is the number of threads
$ make -j5

# Build doxygen docs
$ make docs

# Run all the tests (either command works)
$ ctest -VV
$ make test

# Run program
$ bin/la+ -h
$ mpirun -np 4 bin/la+ mdrun --config ../config.yaml

# Install the built libraries, binaries, docs into
# <prefix>/lib, <prefix>/bin, and <prefix>/share, respectively
# <prefix> is usually /usr/local, but may be / on Docker containers
$ make install

# Alternatively, install of performing, a direct `make install`, create an installation package.
# It will attempt to generate a .tar.gz, a .deb, and .rpm package,
# depending on whether or not the tools are available.
# Note that all targets must be made first (i.e. `make docs`) prior to calling `make package`
$ make package

# On CentOS, install the built rpm using yum
$ yum --nogpgcheck localinstall <package>.rpm

# To uninstall (on CentOS)
$ yum remove laplace
```


## Ubuntu 16

The instructions are largely the same on Ubuntu, but with different
commands for installing and uninstalling packages:

```bash
# Install the built deb, then install the required dependencies
$ dpkg -i <package>.deb && apt-get install -f

# To uninstall
$ apt-get remove laplace
```


## Mac OS X

The instructions are largely the same on Mac OS X.  The dependencies can be installed by [HomeBrew](https://brew.sh/)
