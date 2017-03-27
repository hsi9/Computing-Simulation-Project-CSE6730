package edu.gatech.cse6730.airportsim

import net.jcazevedo.moultingyaml._
import scala.io.Source

object PlaneDistribution extends Enumeration {
  val UNIFORM, ONE_AIRPORT = Value
}

object RunningOption extends Enumeration {
  val NON_DISTRIBUTED, YAWNS, NULL_MESSAGE = Value
}

case class SimulatorConfig(planeCount: Int,
                           planeDistribution: PlaneDistribution.Value,
                           planeUsesHdf5Data: Boolean,
                           logStatistics: Boolean,
                           logInterval: Int,
                           logTraceViewer: Boolean,
                           runningOption: RunningOption.Value,
                           runningTime: Int)

object SimulatorConfigProtocol extends DefaultYamlProtocol {
  implicit object planeDistributionFormat extends YamlFormat[PlaneDistribution.Value] {
    def write(x: PlaneDistribution.Value) = YamlString(x.toString)
    def read(value: YamlValue) = PlaneDistribution.withName(value.asInstanceOf[YamlString].value)
  }

  implicit object runningOptionFormat extends YamlFormat[RunningOption.Value] {
    def write(x: RunningOption.Value) = YamlString(x.toString)
    def read(value: YamlValue) = RunningOption.withName(value.asInstanceOf[YamlString].value)
  }

  implicit val simulatorConfigFormat = yamlFormat8(SimulatorConfig.apply)
}

object SimulatorConfig {
  import edu.gatech.cse6730.airportsim.SimulatorConfigProtocol._
  def fromFile(filepath: String): SimulatorConfig = {
    Source.fromFile(filepath).mkString.parseYaml.convertTo[SimulatorConfig]
  }
}
