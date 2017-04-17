package edu.gatech.cse6730.airportsim

import scala.util.Random

case class Airplane(id: Int,
                    name: String,
                    manufacturer: String,
                    speed: Double,
                    capacity: Int) {
  private var numPassengers = 0
  private val randGen = new scala.util.Random(this.id * this.capacity)

  def loadPassengers(): Int = {
    this.numPassengers = randGen.nextInt(this.capacity)
    this.numPassengers
  }

  def unloadPassengers() : Int = {
    val numDeplaning = this.numPassengers
    this.numPassengers = 0
    numDeplaning
  }
}

object Airplane {
  def defaultPlane(id: Int): Airplane = {
    new Airplane(id, "787-8 Dreamliner", "Boeing", 17.35479, 242)
  }
  // http://www.boeing.com/commercial/787/by-design/#/all-model-performance-summary
  // Speed is in KILOMETRES PER MINUTE. (Simulator runs on minutes)

  def apply(hdf_plane: hdf5.Airplane): Airplane = {
    new Airplane(hdf_plane.id,
                 hdf_plane.name,
                 hdf_plane.manufacturer,
                 hdf_plane.speed,
                 hdf_plane.capacity)
  }
}
