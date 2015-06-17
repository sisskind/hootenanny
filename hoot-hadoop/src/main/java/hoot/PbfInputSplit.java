/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package hoot;

public class PbfInputSplit {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected PbfInputSplit(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(PbfInputSplit obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        hootJNI.delete_PbfInputSplit(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public PbfInputSplit() {
    this(hootJNI.new_PbfInputSplit(), true);
  }

  public int getLength() {
    return hootJNI.PbfInputSplit_getLength(swigCPtr, this);
  }

  public String getLocations() {
    return hootJNI.PbfInputSplit_getLocations(swigCPtr, this);
  }

  public int getStart() {
    return hootJNI.PbfInputSplit_getStart(swigCPtr, this);
  }

  public void readFields(String byteArray, int len) {
    hootJNI.PbfInputSplit_readFields(swigCPtr, this, byteArray, len);
  }

  public void setLocations(String locations) {
    hootJNI.PbfInputSplit_setLocations(swigCPtr, this, locations);
  }

  public void setLength(int length) {
    hootJNI.PbfInputSplit_setLength(swigCPtr, this, length);
  }

  public void setStart(int start) {
    hootJNI.PbfInputSplit_setStart(swigCPtr, this, start);
  }

  public SWIGTYPE_p_signed_char writeFields(SWIGTYPE_p_size_t len) {
    long cPtr = hootJNI.PbfInputSplit_writeFields(swigCPtr, this, SWIGTYPE_p_size_t.getCPtr(len));
    return (cPtr == 0) ? null : new SWIGTYPE_p_signed_char(cPtr, false);
  }

}