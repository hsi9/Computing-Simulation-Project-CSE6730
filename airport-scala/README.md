# Distributed Airport Simulator in Scala


## Software Requirements:

* Java 8+ (Java SDK 1.8+)
* Scala Build Tool (sbt): You may install sbt from your operating system's package management system of choice, OR you may run the included `./sbt` in place of "sbt" in all the example commands below.


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

## Build and Run

```bash
# Unlike Maven, do NOT run `sbt package` because it does not produce a "fat jar" (does not contain Scala libs)
localhost:airport-scala local$ sbt assembly

localhost:airport-scala local$ java -jar target/scala-2.11/airportsim-assembly-0.1.0.jar
5.0: Plane arrived at airport
15.0: Plane lands at airport
25.0: Plane departs from airport
45.0: Plane arrived at airport
Simulator stopping at time: 50.0
```
