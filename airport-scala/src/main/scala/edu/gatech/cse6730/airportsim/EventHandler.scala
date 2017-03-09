package edu.gatech.cse6730.airportsim;

trait EventHandler {
  def handle(event: Event): Unit
}
