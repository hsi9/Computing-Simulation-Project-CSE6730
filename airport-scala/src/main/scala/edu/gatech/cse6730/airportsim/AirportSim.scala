package edu.gatech.cse6730.airportsim

object AirportSim {
  def main(args: Array[String]): Unit = {

    val lax = Airport("LAX", 10, 10, (33.9416, 118.4085))
    val landingEvent = AirportEvent(5, lax, AirportEvent.PLANE_ARRIVES);

    Simulator.schedule(landingEvent)
    Simulator.stopAt(50)
    Simulator.run()
  }
}
