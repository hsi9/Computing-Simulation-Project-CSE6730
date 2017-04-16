package edu.gatech.cse6730.airportsim

import mpi._
import hdf.hdf5lib.H5._
import hdf.hdf5lib._
import net.jcazevedo.moultingyaml._
import net.jcazevedo.moultingyaml.DefaultYamlProtocol._
import scopt._

import scala.collection.mutable.ArrayBuffer
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
          // test yaml config
          val config = SimulatorConfig.fromFile(cliArgs.configFile)
          println(s"\nParsed SimulatorConfig: ${config}")

          var airplanes : List[hdf5.Airplane] = List.empty
          var airplanes_mapped : List[Airplane] = List.empty
          // load airplanes from HDF5 file
          if (config.planeUsesHdf5Data) {
            airplanes = hdf5.Airplane.loadFromH5File(cliArgs.dataFile, "airplanes/table")
            println(s"\nParsed Airplanes from HDF5 file:")
            airplanes.foreach { plane =>
              println(plane)
            }
            // hdf5.Airplane to Airplane conversion
            airplanes_mapped = airplanes.map { plane =>
              new Airplane(plane.id,
                            plane.name,
                            plane.manufacturer,
                            plane.speed,
                            plane.capacity)
            }
          } else {
            var airplanes_mapped_buffer = new ArrayBuffer[Airplane]
            for (i <- 1 to config.planeCount) {
              airplanes_mapped_buffer += new Airplane(i, "787-8 Dreamliner", "Boeing", 17.35479, 242)
              // http://www.boeing.com/commercial/787/by-design/#/all-model-performance-summary
              // Speed is in KILOMETRES PER MINUTE. (Simulator runs on minutes)
            }
            airplanes_mapped = airplanes_mapped_buffer.toList
          }

          // load airports from HDF5 file
          val airports = hdf5.Airport.loadFromH5File(cliArgs.dataFile, "airports/table")
          println(s"\nParsed Airports from HDF5 file:")
          airports.foreach { airport =>
            println(airport)
          }
          // hdf5.Airport to Airport conversion
          val airports_mapped = airports.map { airport =>
            new Airport(airport.id,
                        airport.name,
                        airport.city,
                        airport.country,
                        airport.iataCode,
                        airport.icaoCode,
                        10,
                        10,
                        (airport.latitude, airport.longitude),
                        config)
          }
          airports_mapped.foreach { airport =>
            airport.setAirportList(airports_mapped)
          }
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
            for (i <- 1 to (config.runningTime / config.logInterval) ) {
              val logTime = config.logInterval * i
              Simulator.stopAt(logTime)
              Simulator.run()
              airports_mapped.foreach { airport =>
                airport.logStats()
              }
            }
          }

          Simulator.run()

          if (config.logStatistics) {
            airports_mapped.foreach { airport =>
              airport.logStats()
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
