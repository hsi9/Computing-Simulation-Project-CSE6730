package edu.gatech.cse6730.airportsim

import scala.collection.mutable.Queue
import scala.util.Random

case class Airport(id: Int,
                   name: String,
                   city: String,
                   country: String,
                   iataCode: String,
                   icaoCode: String,
                   runwayTimeToLand: Double,
                   requiredTimeOnGround: Double,
                   gpsCoords: (Double, Double)) extends EventHandler {
  private var inTheAir = 0
  private var onTheGround = 0
  private var runwayFree = true
  private var numArrived = 0
  private var numDeparted = 0

  private val randGen = new scala.util.Random(this.id + numArrived*numDeparted)

  private var runwayQueue = new Queue[(Double, Int, Airplane)]
  private var timeGrounded = 0.0
  private var timeCircling = 0.0
  private var airportList : List[Airport] = List.empty

  private def deg2rad(deg: Double): Double = {
    deg * Math.PI / 180.0
  }

  private def rad2deg(rad: Double): Double = {
    rad * 180.0 / Math.PI
  }

  def setAirportList(newList: List[Airport]) {
    airportList = newList
  }

  def distanceTo(other: Airport): Double = {
    val (lat1, lon1) = gpsCoords
    val (lat2, lon2) = other.gpsCoords

    val theta = lon1 - lon2
    val dist = Math.sin(deg2rad(lat1)) * Math.sin(deg2rad(lat2)) + Math.cos(deg2rad(lat1)) * Math.cos(deg2rad(lat2)) * Math.cos(deg2rad(theta))
    rad2deg(Math.acos(dist)) * 60 * 1.1515 * 1.609344;
  }

  override def handle(event: Event): Unit = {
    val airEvent = event.asInstanceOf[AirportEvent]

    airEvent.eventType match {
      case AirportEvent.PLANE_ARRIVES =>
        inTheAir = inTheAir + 1
        println(s"${Simulator.getCurrentTime}: Plane requesting to land at ${this.icaoCode}")
        if (runwayFree) {
          Simulator.schedule(AirportEvent(runwayTimeToLand, this, AirportEvent.PLANE_LANDED, airEvent.plane))
        } else {
          runwayQueue += ((Simulator.getCurrentTime, AirportEvent.PLANE_LANDED, airEvent.plane))
          Simulator.schedule(AirportEvent(0, this, AirportEvent.RUNWAY_EVENT, null))
        }

      case AirportEvent.PLANE_DEPARTS =>
        onTheGround = onTheGround + 1
        println(s"${Simulator.getCurrentTime}: Plane requesting to take off from ${this.icaoCode}")
        numDeparted = numDeparted + airEvent.plane.loadPassengers
        if (runwayFree) {
          Simulator.schedule(AirportEvent(0, this, AirportEvent.PLANE_TAKES_OFF, airEvent.plane))
        } else {
          runwayQueue += ((Simulator.getCurrentTime, AirportEvent.PLANE_TAKES_OFF, airEvent.plane))
          Simulator.schedule(AirportEvent(0, this, AirportEvent.RUNWAY_EVENT, null))
        }

      case AirportEvent.PLANE_LANDED =>
        inTheAir = inTheAir - 1
        println(s"${Simulator.getCurrentTime}: Plane lands at ${this.icaoCode}")
        numArrived = numArrived + airEvent.plane.unloadPassengers
        Simulator.schedule(AirportEvent(requiredTimeOnGround, this, AirportEvent.PLANE_DEPARTS, airEvent.plane))

      case AirportEvent.PLANE_TAKES_OFF =>
        onTheGround = onTheGround - 1
        println(s"${Simulator.getCurrentTime}: Plane takes off from ${this.icaoCode}")
        var destination = this.airportList(this.randGen.nextInt(this.airportList.length))
        while (destination.id == this.id) {
          destination = this.airportList(this.randGen.nextInt(this.airportList.length))
        }
        Simulator.schedule(AirportEvent(distanceTo(destination), this, AirportEvent.PLANE_ARRIVES, airEvent.plane))

      case AirportEvent.RUNWAY_EVENT =>
        if (runwayQueue.length >= 1) {
          val (queuedTime, queuedEventType, queuedPlane) = runwayQueue.dequeue
          var queuedEventCompletionTime = 0.0
          queuedEventType match {
            case AirportEvent.PLANE_LANDED =>
              queuedEventCompletionTime = runwayTimeToLand
              Simulator.schedule(AirportEvent(queuedEventCompletionTime, this, AirportEvent.PLANE_LANDED, queuedPlane))
              timeCircling = timeCircling + (Simulator.getCurrentTime - queuedTime)

            case AirportEvent.PLANE_TAKES_OFF =>
              queuedEventCompletionTime = 0.0
              Simulator.schedule(AirportEvent(queuedEventCompletionTime, this, AirportEvent.PLANE_TAKES_OFF, queuedPlane))
              timeGrounded = timeGrounded + (Simulator.getCurrentTime - queuedTime)
          }
          if (runwayQueue.length >= 1) {
            Simulator.schedule(AirportEvent(queuedEventCompletionTime, this, AirportEvent.RUNWAY_EVENT, null))
          }
        }
        if (runwayQueue.length == 0) {
          runwayFree = true
        }
    }
  }
}
