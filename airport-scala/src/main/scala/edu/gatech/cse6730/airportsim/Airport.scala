package edu.gatech.cse6730.airportsim

case class Airport(name: String,
                   runwayTimeToLand: Double,
                   requiredTimeOnGround: Double,
                   gpsCoords: (Double, Double)) extends EventHandler {
  private var inTheAir = 0
  private var onTheGround = 0
  private var freeToLand = true

  private def deg2rad(deg: Double): Double = {
    deg * Math.PI / 180.0
  }

  private def rad2deg(rad: Double): Double = {
    rad * 180.0 / Math.PI
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
        println(s"${Simulator.getCurrentTime}: Plane arrived at airport")
        if (freeToLand) {
          Simulator.schedule(AirportEvent(runwayTimeToLand, this, AirportEvent.PLANE_LANDED))
        }

      case AirportEvent.PLANE_DEPARTS =>
        onTheGround = onTheGround - 1;
        println(s"${Simulator.getCurrentTime}: Plane departs from airport")
        Simulator.schedule(AirportEvent(20/*flightTime*/, this, AirportEvent.PLANE_ARRIVES))

      case AirportEvent.PLANE_LANDED =>
        inTheAir = inTheAir - 1;
        println(s"${Simulator.getCurrentTime}: Plane lands at airport")
        Simulator.schedule(AirportEvent(requiredTimeOnGround, this, AirportEvent.PLANE_DEPARTS))
        inTheAir match {
          case 0 =>
            freeToLand = true
          case _ =>
            Simulator.schedule(AirportEvent(runwayTimeToLand, this, AirportEvent.PLANE_LANDED))
        }
    }
  }
}
