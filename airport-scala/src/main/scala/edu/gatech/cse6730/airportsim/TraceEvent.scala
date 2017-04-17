package edu.gatech.cse6730.airportsim

case class TraceEvent(name: String,
                    category: List[String],
                    eventType: String,
                    timestamp: Double,
                    airportID: Int)