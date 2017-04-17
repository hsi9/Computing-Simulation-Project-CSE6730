package edu.gatech.cse6730.airportsim

import mpi._
import hdf.hdf5lib.H5._
import hdf.hdf5lib._
import net.jcazevedo.moultingyaml._
import net.jcazevedo.moultingyaml.DefaultYamlProtocol._
import scopt._

import scala.math._
import scala.util.Random

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
          println(s"Number of MPI processes: ${MPI.COMM_WORLD.getSize()}")
          // load yaml config
          val config = SimulatorConfig.fromFile(cliArgs.configFile)
          println(s"\nParsed SimulatorConfig: ${config}")
          Simulator.setConfig(config)

          // load airplanes from HDF5 file (unless options say not to)
          val airplanes_mapped =
            if (config.planeUsesHdf5Data) {
              val hdf5_airplanes = hdf5.Airplane.loadFromH5File(cliArgs.dataFile, "airplanes/table")
              println(s"\nParsed ${hdf5_airplanes.length} Airplanes from HDF5 file:")
              hdf5_airplanes.foreach { plane =>
                println(plane)
              }
              // hdf5.Airplane to Airplane conversion
              hdf5_airplanes.map(Airplane(_))
            } else {
              1.to(config.planeCount).toList.map { i =>
                Airplane.defaultPlane(i)
              }
            }

          // load airports from HDF5 file
          val hdf5_airports = hdf5.Airport.loadFromH5File(cliArgs.dataFile, "airports/table")
            if (config.logRealTimeEvents) {
              println(s"\nParsed ${hdf5_airports.length} Airports from HDF5 file:")
              hdf5_airports.foreach { airport =>
                println(airport)
              }
            }
          // hdf5.Airport to Airport conversion
          val airports_mapped = hdf5_airports.map { airport =>
            new Airport(airport.id,
                        airport.name,
                        airport.city,
                        airport.country,
                        airport.iataCode,
                        airport.icaoCode,
                        10,
                        10,
                        (airport.latitude, airport.longitude))
          }
          airports_mapped.foreach { airport =>
            airport.setAirportList(airports_mapped)
          }
          Airport.setConfig(config)

          val randGen = new scala.util.Random
          config.planeDistribution match {
            case PlaneDistribution.ONE_AIRPORT =>
              for (i <- 0 to config.planeCount) {
                val airplane = airplanes_mapped(randGen.nextInt(airplanes_mapped.length))
                val airport = airports_mapped(0)
                Simulator.schedule(AirportEvent(randGen.nextInt(5), airport, AirportEvent.PLANE_ARRIVES, airplane))
              }
            case PlaneDistribution.UNIFORM =>
              for(i <- 0 to config.planeCount) {
                val airplane = airplanes_mapped(randGen.nextInt(airplanes_mapped.length))
                val airport = airports_mapped(randGen.nextInt(airports_mapped.length))
                Simulator.schedule(AirportEvent(randGen.nextInt(5), airport, AirportEvent.PLANE_ARRIVES, airplane))
              }
          }

          Simulator.stopAt(config.runningTime)

          if (config.logStatistics) {
            for (i <- 1 to floor(config.runningTime / config.logInterval).toInt ) {
              Simulator.stopAt(config.logInterval)
              Simulator.run()
              airports_mapped.foreach { airport =>
                airport.logStats()
              }
            }
          }

          Simulator.run()

          if (config.logTraceViewer) {
            airports_mapped.foreach { airport =>
              airport.logTrace()
            }
          }
        }

        // computePiInParallel()
        MPI.Finalize()

      case None =>
        sys.exit(1)
    }
  }
}
