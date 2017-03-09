organization := "edu.gatech.cse6730"
version := "0.1.0"
scalaVersion := "2.11.8"

val CommonsIoVersion = "2.5"
val LogbackVersion = "1.1.3"
val JodaTimeVersion = "2.9.4"

resolvers ++= Seq(
    "Local Maven Repository" at "file://" + Path.userHome.absolutePath + "/.m2/repository"
  )

libraryDependencies ++= Seq(
	"org.scala-lang" % "scala-library" % "2.11.8",
    "commons-io" % "commons-io" % CommonsIoVersion,
    "ch.qos.logback" % "logback-classic" % LogbackVersion % "runtime",
    "joda-time" % "joda-time" % JodaTimeVersion
  )

enablePlugins(JavaAppPackaging)
