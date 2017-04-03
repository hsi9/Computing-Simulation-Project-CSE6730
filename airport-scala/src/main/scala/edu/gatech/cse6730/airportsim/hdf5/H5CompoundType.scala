package edu.gatech.cse6730.airportsim.hdf5

import java.nio.ByteBuffer
import java.nio.charset.Charset
import hdf.hdf5lib.H5._
import hdf.hdf5lib._

case class H5ColumnDescription(name: String,
                               dims: Int,
                               h5Type: Long,
                               nativeType: Long,
                               storageSize: Int)

object H5CompoundType {
  val RANK = 1
  val INTEGERSIZE = 4
  val DOUBLESIZE = 8
  val MAXSTRINGSIZE = 80

  val strTypeId = H5.H5Tcopy(HDF5Constants.H5T_C_S1)
  H5.H5Tset_size(strTypeId, MAXSTRINGSIZE)

  def getStringFromBuffer(databuf: ByteBuffer, start: Int): String = {
    val stringbuf = databuf.duplicate
    stringbuf.position(start)
    stringbuf.limit(start + MAXSTRINGSIZE)
    val bytearr = Array.fill[Byte](stringbuf.remaining)(0);
    stringbuf.get(bytearr)
    new String(bytearr, Charset.forName("UTF-8")).trim()
  }
}

/*
  Type description and helper class for H5 compound types
*/
case class H5CompoundType(columns: List[H5ColumnDescription]) {
  val h5TypeId = createH5Type(false)
  val memTypeId = createH5Type(true)

  def dataSize: Int = {
    columns.map { c => c.storageSize * c.dims }.sum
  }

  def getOffset(memberItem: Int): Int = {
    columns.take(memberItem).map(_.storageSize).sum
  }

  private def createH5Type(nativeType: Boolean): Long = {
    val memTypeId = H5.H5Tcreate(HDF5Constants.H5T_COMPOUND, dataSize)
    (0.to(columns.size-1), columns).zipped.foreach {
      case (idx, column) =>
        val typeId = if (nativeType) column.nativeType else column.h5Type
        H5.H5Tinsert(
          memTypeId,
          column.name,
          getOffset(idx),
          if (typeId == HDF5Constants.H5T_C_S1) H5CompoundType.strTypeId else typeId
        )
    }
    memTypeId
  }
}
