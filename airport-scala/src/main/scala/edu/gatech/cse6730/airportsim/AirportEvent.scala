package edu.gatech.cse6730.airportsim

object AirportEvent {
  val PLANE_ARRIVES = 0
  val PLANE_LANDED = 1
  val PLANE_DEPARTS = 2
}

case class AirportEvent(var time: Double,
                        handler: EventHandler,
                        eventType: Int) extends Event