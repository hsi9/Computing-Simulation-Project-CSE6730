import sbt._
import Keys._

object AirportSimBuild extends Build {
  val Name = "airportsim"

  lazy val project = Project(Name, file("."))
    .settings(
      publishMavenStyle := true,
      autoScalaLibrary := true,
      isSnapshot := true
    )
}
