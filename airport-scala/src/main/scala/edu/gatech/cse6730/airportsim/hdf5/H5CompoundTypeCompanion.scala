package edu.gatech.cse6730.airportsim.hdf5

import hdf.hdf5lib.H5._
import hdf.hdf5lib._
import java.nio.{ ByteBuffer, ByteOrder }

/*
  This trait is to be extended by the companion object of the model case class
*/
trait H5CompoundTypeCompanion[T] {
  def h5Type: H5CompoundType
  def fromBytes(databuf: ByteBuffer,
                pos: Int): T

  def loadFromH5File(fileId: Long, dsetpath: String): List[T] = {
    // open the dataset
    val datasetId = H5.H5Dopen(fileId, dsetpath, HDF5Constants.H5P_DEFAULT)
    val dataspaceId = H5.H5Dget_space(datasetId)

    // get size of array (number of Airplanes)
    val dims = Array[Long](1)
    H5.H5Sget_simple_extent_dims(dataspaceId, dims, null)

    // allocate memory for read buffer.
    val rawBuffer = Array.fill[Byte](dims(0).toInt * h5Type.dataSize)(0)

    // read raw bytes in
    H5.H5Dread(
      datasetId,
      h5Type.memTypeId,
      HDF5Constants.H5S_ALL,
      HDF5Constants.H5S_ALL,
      HDF5Constants.H5P_DEFAULT,
      rawBuffer
    )

    // wrap inside ByteBuffer with correct byte-ordering
    val inBuf = ByteBuffer.wrap(rawBuffer)
    inBuf.order(ByteOrder.nativeOrder())

    // read the ByteBuffer contents and build Airplane objects
    val tlist = 0.to((dims(0)-1).toInt).toList.map { idx =>
      fromBytes(inBuf, idx * h5Type.dataSize)
    }

    // cleanup
    H5.H5Dclose(datasetId)
    H5.H5Sclose(dataspaceId)

    tlist
  }

  def loadFromH5File(filepath: String, dsetpath: String): List[T] = {
    // open the file
    val fileId = H5.H5Fopen(filepath, HDF5Constants.H5F_ACC_RDONLY, HDF5Constants.H5P_DEFAULT)
    val tlist = loadFromH5File(fileId, dsetpath)
    H5.H5Fclose(fileId)
    tlist
  }
}
