package edu.gatech.cse6730.airportsim.hdf5

import hdf.hdf5lib.H5._
import hdf.hdf5lib._
import java.nio.ByteBuffer

object Airport extends H5CompoundTypeCompanion[Airport] {
  // The order of the H5ColumnDescriptions must follow the order of the fields in the corresponding case class
  override def h5Type = H5CompoundType(List(
    H5ColumnDescription("id", 1, HDF5Constants.H5T_STD_I32BE, HDF5Constants.H5T_NATIVE_INT, H5CompoundType.INTEGERSIZE),
    H5ColumnDescription("name", 1, HDF5Constants.H5T_C_S1, HDF5Constants.H5T_C_S1, H5CompoundType.MAXSTRINGSIZE),
    H5ColumnDescription("city", 1, HDF5Constants.H5T_C_S1, HDF5Constants.H5T_C_S1, H5CompoundType.MAXSTRINGSIZE),
    H5ColumnDescription("country", 1, HDF5Constants.H5T_C_S1, HDF5Constants.H5T_C_S1, H5CompoundType.MAXSTRINGSIZE),
    H5ColumnDescription("iata", 1, HDF5Constants.H5T_C_S1, HDF5Constants.H5T_C_S1, H5CompoundType.MAXSTRINGSIZE),
    H5ColumnDescription("icao", 1, HDF5Constants.H5T_C_S1, HDF5Constants.H5T_C_S1, H5CompoundType.MAXSTRINGSIZE),
    H5ColumnDescription("latitude", 1, HDF5Constants.H5T_IEEE_F64BE, HDF5Constants.H5T_NATIVE_DOUBLE, H5CompoundType.DOUBLESIZE),
    H5ColumnDescription("longitude", 1, HDF5Constants.H5T_IEEE_F64BE, HDF5Constants.H5T_NATIVE_DOUBLE, H5CompoundType.DOUBLESIZE),
    H5ColumnDescription("altitude", 1, HDF5Constants.H5T_IEEE_F64BE, HDF5Constants.H5T_NATIVE_DOUBLE, H5CompoundType.DOUBLESIZE)
  ))

  override def fromBytes(databuf: ByteBuffer,
                pos: Int): Airport = {
    Airport(
      databuf.getInt(pos + h5Type.getOffset(0)),
      H5CompoundType.getStringFromBuffer(databuf, pos + h5Type.getOffset(1)),
      H5CompoundType.getStringFromBuffer(databuf, pos + h5Type.getOffset(2)),
      H5CompoundType.getStringFromBuffer(databuf, pos + h5Type.getOffset(3)),
      H5CompoundType.getStringFromBuffer(databuf, pos + h5Type.getOffset(4)),
      H5CompoundType.getStringFromBuffer(databuf, pos + h5Type.getOffset(5)),
      databuf.getDouble(pos + h5Type.getOffset(6)),
      databuf.getDouble(pos + h5Type.getOffset(7)),
      databuf.getDouble(pos + h5Type.getOffset(8))
    )
  }
}

case class Airport(id: Int,
                   name: String,
                   city: String,
                   country: String,
                   iataCode: String,
                   icaoCode: String,
                   latitude: Double,
                   longitude: Double,
                   altitude: Double)
