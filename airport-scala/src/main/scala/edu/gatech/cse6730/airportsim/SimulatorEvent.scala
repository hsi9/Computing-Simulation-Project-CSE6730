package edu.gatech.cse6730.airportsim;

object SimulatorEvent {
  val STOP_EVENT = 0
}

case class SimulatorEvent(var time: Double,
                          handler: EventHandler,
                          eventType: Int) extends Event
