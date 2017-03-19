import sbt._
import Keys._

object AirportSimBuild extends Build {
  val Name = "airportsim"

  lazy val project = Project(Name, file("."))
    .settings(
      publishMavenStyle := true,
      autoScalaLibrary := true,
      isSnapshot := true
    ).settings(
      unmanagedJars in Compile += file( sys.env("MPI_JARPATH") ), // /usr/local/opt/open-mpi/lib/mpi.jar
      unmanagedJars in Compile += file( sys.env("HDF_JARPATH") ), // /usr/local/opt/hdf5-mpi/lib/jarhdf5-1.10.0.jar
      unmanagedBase <<= baseDirectory { base => base / "libs" }
    )
}
