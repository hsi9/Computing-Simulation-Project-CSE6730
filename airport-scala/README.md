# Distributed Airport Simulator in Scala


## Software Requirements:

* Java 8+ (Java JDK 1.8+)
* Scala Build Tool (sbt): You may install sbt from your operating system's package management system of choice, OR you may run the included `./sbt` in place of "sbt" in all the example commands below.
* OpenMPI 2.0+ with Java bindings
* HDF5 1.10+ with Java bindings
* CMake 3.1+ for building HDF5



### Building OpenMPI

OpenMPI can be installed by your OS's package manager of choice, but most likely the default installation will not contain Java bindings.  Thus, we must build OpenMPI from scratch with the Java bindings.

The instructions for building and installing OpenMPI are as follows:


```bash
# List the current working directory
$ pwd
~/openmpi-2.0.2

# You may choose to install to another location; `/usr/local/opt/open-mpi` is chosen here for a self-contained installation.
$ ./configure --prefix=/usr/local/opt/open-mpi --enable-java --enable-mpi-java

# Build the libraries
$ make

# Install them to the specified prefix path
$ make install

$ ls /usr/local/opt/open-mpi/
bin	etc	include	lib	share
```

After installation completes, notice the following Java-related MPI libraries that have been built:

```bash
$ ls /usr/local/opt/open-mpi/lib/*java*
/usr/local/opt/open-mpi/lib/libmpi_java.20.dylib	/usr/local/opt/open-mpi/lib/libmpi_java.dylib		/usr/local/opt/open-mpi/lib/libmpi_java.la

$ ls /usr/local/opt/open-mpi/lib/*jar
/usr/local/opt/open-mpi/lib/mpi.jar
```

`mpi.jar` and `libmpi_java.dylib` will be needed later on.



### Building HDF5

Similarly, HDF5 can be installed by your OS's package manager of choice; but most likely the default installation will not contain Java bindings.

```bash
# List the current working directory
$ pwd
~/hdf5-1.10.0-patch1

# Make a build directory to build the stuff in
$ mkdir target && cd target/

# Configure the build
$ cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr/local/opt/hdf5 -DHDF5_BUILD_JAVA=1 -DHDF5_BUILD_TOOLS=1 -DMPI_C_INCLUDE_PATH=/usr/local/opt/open-mpi/include -DMPI_C_LIBRARIES=/usr/local/opt/open-mpi/lib/libmpi.dylib ..

# Build the libraries
$ make

# Install them to the specified prefix path
$ make install

$ ls /usr/local/opt/open-mpi/
bin	etc	include	lib	share
```

After installation completes, notice the following Java-related MPI libraries that have been built:

```bash
$ ls /usr/local/opt/hdf5/lib/*java*
/usr/local/opt/hdf5/lib/libhdf5_java.1.10.0.dylib	/usr/local/opt/hdf5/lib/libhdf5_java.100.0.1.dylib	/usr/local/opt/hdf5/lib/libhdf5_java.dylib

$ ls /usr/local/opt/hdf5/lib/*jar
/usr/local/opt/hdf5/lib/jarhdf5-1.10.0.jar		/usr/local/opt/hdf5/lib/slf4j-nop-1.7.5.jar
/usr/local/opt/hdf5/lib/slf4j-api-1.7.5.jar		/usr/local/opt/hdf5/lib/slf4j-simple-1.7.5.jar
```

`jarhdf5-1.10.0.jar` and `libhdf5_java.dylib` will be needed later on.




## Build and Run the Code

With the OpenMPI and HDF5 Java bindings installed, the Airport Simulator can now be built.  The jars are needed at build-time to expose the bindings in Java/Scala:

```bash
# Do NOT run `sbt package` because it does not produce a "fat jar" (does not contain Scala libs)
localhost:airport-scala local$ MPI_JARPATH=/usr/local/opt/open-mpi/lib/mpi.jar HDF_JARPATH=/usr/local/opt/hdf5/lib/jarhdf5-1.10.0.jar sbt assembly
```

During runtime, the shared libraries `libmpi_java.dylib` and `libhdf5_java.dylib` are needed for Java bindings to work with the C libraries.  Thus, `java.library.path` must be specified:

```bash
# Run on 4 MPI processes
localhost:airport-scala local$ /usr/local/opt/open-mpi/bin/mpirun -np 4  java -jar -Djava.library.path=/usr/local/opt/open-mpi/lib:/usr/local/opt/hdf5/lib  target/scala-2.11/airportsim-assembly-0.1.0.jar
```




## Test Out the Code in a REPL

While developing code, it is useful to test it out in a REPL:

```bash
localhost:airport-scala local$ sbt console
[info] Loading project definition from /Users/local/cse6730/airport-scala/project
[info] Set current project to airportsim (in build file:/Users/local/cse6730/airport-scala/)
[info] Compiling 8 Scala sources to /Users/local/cse6730/airport-scala/target/scala-2.11/classes...
[info] Starting scala interpreter...
[info]
Welcome to Scala 2.11.8 (Java HotSpot(TM) 64-Bit Server VM, Java 1.8.0_05).
Type in expressions for evaluation. Or try :help.

scala> import edu.gatech.cse6730.airportsim._
import edu.gatech.cse6730.airportsim._

scala> val plane = Airplane("Boeing", 200, 500.0)
plane: edu.gatech.cse6730.airportsim.Airplane = Airplane(Boeing,200,500.0)

scala>
```
