package edu.gatech.cse6730.airportsim.hdf5

import hdf.hdf5lib.H5._
import hdf.hdf5lib._
import java.nio.ByteBuffer

object Airplane extends H5CompoundTypeCompanion[Airplane] {
  // The order of the H5ColumnDescriptions must follow the order of the fields in the corresponding case class
  override def h5Type = H5CompoundType(List(
    H5ColumnDescription("id", 1, HDF5Constants.H5T_STD_I32BE, HDF5Constants.H5T_NATIVE_INT, H5CompoundType.INTEGERSIZE),
    H5ColumnDescription("name", 1, HDF5Constants.H5T_C_S1, HDF5Constants.H5T_C_S1, H5CompoundType.MAXSTRINGSIZE),
    H5ColumnDescription("manufacturer", 1, HDF5Constants.H5T_C_S1, HDF5Constants.H5T_C_S1, H5CompoundType.MAXSTRINGSIZE),
    H5ColumnDescription("speed", 1, HDF5Constants.H5T_IEEE_F64BE, HDF5Constants.H5T_NATIVE_DOUBLE, H5CompoundType.DOUBLESIZE),
    H5ColumnDescription("capacity", 1, HDF5Constants.H5T_STD_I32BE, HDF5Constants.H5T_NATIVE_INT, H5CompoundType.INTEGERSIZE)
  ))

  override def fromBytes(databuf: ByteBuffer,
                pos: Int): Airplane = {
    Airplane(
      databuf.getInt(pos + h5Type.getOffset(0)),
      H5CompoundType.getStringFromBuffer(databuf, pos + h5Type.getOffset(1)),
      H5CompoundType.getStringFromBuffer(databuf, pos + h5Type.getOffset(2)),
      databuf.getDouble(pos + h5Type.getOffset(3)),
      databuf.getInt(pos + h5Type.getOffset(4))
    )
  }
}

case class Airplane(id: Int,
                    name: String,
                    manufacturer: String,
                    speed: Double,
                    capacity: Int)
