package edu.gatech.cse6730.airportsim

import org.json4s._
import org.json4s.native.JsonMethods._
import org.json4s.JsonDSL.WithDouble._

import scala.collection.mutable.ArrayBuffer
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
  import Airport._

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
    //60 is the number of minutes in a degree;
    //1.1515 is the number of statute miles in a nautical mile;
    //and 1.609344 is the number of kilometres in a mile.
    //One nautical mile is the length of one minute of latitude at the equator.
    //Source: http://stackoverflow.com/questions/389211/geospatial-coordinates-and-distance-in-kilometers
    //Result is in KILOMETRES.
  }

  override def handle(event: Event): Unit = {
    val airEvent = event.asInstanceOf[AirportEvent]

    airEvent.eventType match {
      case AirportEvent.PLANE_ARRIVES =>
        inTheAir = inTheAir + 1
        if (Airport.config.logRealTimeEvents) println(s"${Simulator.getCurrentTime}: Plane requesting to land at ${this.icaoCode}")
        if (runwayFree) {
          Simulator.schedule(AirportEvent(runwayTimeToLand, this, AirportEvent.PLANE_LANDED, airEvent.plane))
        } else {
          runwayQueue += ((Simulator.getCurrentTime, AirportEvent.PLANE_LANDED, airEvent.plane))
          Simulator.schedule(AirportEvent(0, this, AirportEvent.RUNWAY_EVENT, null))
        }
        if (Airport.config.logTraceViewer) {
          Airport.traceEventList += new TraceEvent(s"${icaoCode}: plane arrived",
                                           List(s"airport", s"PLANE_"),
                                           "i",
                                           Simulator.getCurrentTime,
                                           id)
        }

      case AirportEvent.PLANE_DEPARTS =>
        onTheGround = onTheGround + 1
        if (Airport.config.logRealTimeEvents) println(s"${Simulator.getCurrentTime}: Plane requesting to take off from ${this.icaoCode}")
        numDeparted = numDeparted + airEvent.plane.loadPassengers
        if (runwayFree) {
          Simulator.schedule(AirportEvent(0, this, AirportEvent.PLANE_TAKES_OFF, airEvent.plane))
        } else {
          runwayQueue += ((Simulator.getCurrentTime, AirportEvent.PLANE_TAKES_OFF, airEvent.plane))
          Simulator.schedule(AirportEvent(0, this, AirportEvent.RUNWAY_EVENT, null))
        }
        if (Airport.config.logTraceViewer) {
          Airport.traceEventList += new TraceEvent(s"${icaoCode}: plane departed",
                                           List(s"airport", s"PLANE_"),
                                           "i",
                                           Simulator.getCurrentTime,
                                           id)
        }

      case AirportEvent.PLANE_LANDED =>
        inTheAir = inTheAir - 1
        if (Airport.config.logRealTimeEvents) println(s"${Simulator.getCurrentTime}: Plane lands at ${this.icaoCode}")
        numArrived = numArrived + airEvent.plane.unloadPassengers
        Simulator.schedule(AirportEvent(requiredTimeOnGround, this, AirportEvent.PLANE_DEPARTS, airEvent.plane))
        if (Airport.config.logTraceViewer) {
          Airport.traceEventList += new TraceEvent(s"${icaoCode}: plane landed",
                                           List(s"airport", s"PLANE_"),
                                           "i",
                                           Simulator.getCurrentTime,
                                           id)
        }

      case AirportEvent.PLANE_TAKES_OFF =>
        onTheGround = onTheGround - 1
        if (Airport.config.logRealTimeEvents) println(s"${Simulator.getCurrentTime}: Plane takes off from ${this.icaoCode}")
        var destination = this.airportList(this.randGen.nextInt(this.airportList.length))
        while (destination.id == this.id) {
          destination = this.airportList(this.randGen.nextInt(this.airportList.length))
        }
        Simulator.schedule(AirportEvent(distanceTo(destination), this, AirportEvent.PLANE_ARRIVES, airEvent.plane))
        if (Airport.config.logTraceViewer) {
          Airport.traceEventList += new TraceEvent(s"${icaoCode}: plane took off",
                                           List(s"airport", s"PLANE_"),
                                           "i",
                                           Simulator.getCurrentTime,
                                           id)
        }

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

  def logStats(): Unit = {
    if (Airport.config.shortLogs) {
      println(s"${Simulator.getCurrentTime} ${id} ${numArrived} ${numDeparted} ${timeGrounded} ${timeCircling}")
    } else {
      println(s"${Simulator.getCurrentTime}: Airport ${id} ${icaoCode}: ${numArrived} arrived, ${numDeparted} departed, ${timeGrounded} min grounded, ${timeCircling} min circling")
    }
  }

  def logTrace(): Unit = {
    val json =
      ("traceEvents" -> traceEventList.map { e =>
        ("name" -> e.name) ~
        ("cat" -> e.category.mkString(",") ) ~
        ("ph" -> e.eventType) ~
        ("ts" -> e.timestamp * 60000) ~ // times are in ms
        ("pid" -> e.airportID)
      }) ~
      ("displayTimeUnit" -> "ms") // trace-viewer only supports ms or ns

    println(pretty(render(json)))
  }
}

object Airport{
  private var config: SimulatorConfig = SimulatorConfig.defaultConfig()
  private val traceEventList = ArrayBuffer.empty[TraceEvent]

  def setConfig(newConfig: SimulatorConfig): Unit = { config = newConfig }
}
