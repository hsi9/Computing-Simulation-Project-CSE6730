package edu.gatech.cse6730.airportsim

import java.util.TreeSet

object Simulator extends EventHandler {
  private var currentTime: Double = 0
  private var isRunning: Boolean = false
  private val eventList: TreeSet[Event] = new TreeSet[Event]()

  def run(): Unit = {
    isRunning = true
    while (isRunning && !eventList.isEmpty()) {
        val event = eventList.pollFirst()
        currentTime = event.time
        event.handler.handle(event)
    }
  }

  override def handle(event: Event): Unit = {
    event.eventType match {
      case SimulatorEvent.STOP_EVENT =>
        isRunning = false;
        println(s"Simulator stopping at time: ${event.time}")
      case _ =>
        println("Invalid event type")
    }
  }

  def schedule(event: Event): Unit = {
    event.time = event.time + getCurrentTime
    eventList.add(event)
  }

  def stop(): Unit = {
    isRunning = false
  }

  def stopAt(time: Double) = {
    schedule(SimulatorEvent(time, this, SimulatorEvent.STOP_EVENT))
  }

  def getCurrentTime: Double = {
    currentTime
  }
}
