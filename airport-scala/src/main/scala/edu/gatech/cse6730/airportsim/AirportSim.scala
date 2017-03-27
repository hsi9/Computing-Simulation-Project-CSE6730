package edu.gatech.cse6730.airportsim

import mpi._
import hdf.hdf5lib.H5._
import hdf.hdf5lib._
import net.jcazevedo.moultingyaml._
import net.jcazevedo.moultingyaml.DefaultYamlProtocol._
import scopt._

case class AirportSimCliArgs(configFile: String = "",
                             dataFile: String = "")

object AirportSim {
  val argsParser = new scopt.OptionParser[AirportSimCliArgs]("airport-sim") {
    head("airport-sim", "0.1.0")
    note("airport-sim is an airport simulator\n")
    help("help") text("prints this usage text")

    opt[String]("config").required().action {
      (x, c) => c.copy(configFile = x)
    }.text("YAML config filepath")

    opt[String]("data").action {
      (x, c) => c.copy(dataFile = x)
    }.text("HDF5 input data filepath")
  }

  def computePiInParallel(): Unit = {
    val (rank, size) = (MPI.COMM_WORLD.getRank(), MPI.COMM_WORLD.getSize())

    if (rank == 0) {
      println("\nComputing PI in parallel...")
    }

    println(s"Hello from rank: ${rank}")

    // http://blogs.cisco.com/performance/java-bindings-for-open-mpi
    val nint = 100
    val h = 1.0 / nint
    val sum = (rank+1).to(nint).by(size).map { i =>
      val x = h * (i - 0.5)
      (4.0 / (1.0 + x * x))
    }.sum

    val (sBuf, rBuf) = (Array[Double](h * sum), Array[Double](0))

    MPI.COMM_WORLD.reduce(sBuf, rBuf, 1, MPI.DOUBLE, MPI.SUM, 0);
    if (rank == 0) {
      println(s"PI: ${rBuf(0)}")
    }
  }

  def main(args: Array[String]): Unit = {
    argsParser.parse(args, AirportSimCliArgs()) match {
      case Some(cliArgs) =>
        MPI.Init(args)

        if (MPI.COMM_WORLD.getRank() == 0) {
          val lax = Airport("LAX", 10, 10, (33.9416, 118.4085))
          val landingEvent = AirportEvent(5, lax, AirportEvent.PLANE_ARRIVES);
          Simulator.schedule(landingEvent)
          Simulator.stopAt(50)
          Simulator.run()

          // test HDF5
          val fileId = H5Fopen(cliArgs.dataFile, HDF5Constants.H5F_ACC_RDONLY, HDF5Constants.H5P_DEFAULT)

          // test yaml config
          val config = SimulatorConfig.fromFile(cliArgs.configFile)
          println(s"\nParsed SimulatorConfig: ${config}")
        }

        computePiInParallel()

        MPI.Finalize()

      case None =>
        sys.exit(1)
    }
  }
}
