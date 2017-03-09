package edu.gatech.cse6730.airportsim

import java.util.concurrent.atomic.AtomicInteger

object Event {
  val nextId = new AtomicInteger(0)
}

trait Event extends Comparable[Event] {
  var time: Double
  val handler: EventHandler
  val eventType: Int

  val eventId = Event.nextId.incrementAndGet()

  override def compareTo(other: Event): Int = {
    time.compareTo(other.time) match {
      case timeCmp if timeCmp != 0 =>
        timeCmp
      case _ =>
        eventId.compareTo(other.eventId)
    }
  }
}
