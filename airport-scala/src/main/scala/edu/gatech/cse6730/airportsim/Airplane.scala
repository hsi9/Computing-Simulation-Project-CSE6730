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
