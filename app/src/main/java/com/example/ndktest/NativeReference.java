package com.example.ndktest;

import android.util.Log;

/**
 * @author Linbing Tang
 * @since 17-8-11.
 */
public class NativeReference {
  private static final String TAG = "NativeReference";
  static {
    System.loadLibrary("native-lib");
  }

  /**
   * JNI ERROR (app bug): local reference table overflow (max=512)
   */
  public static native void no_delete_local_ref(int refCount);

  /**
   * fix {@link #no_delete_local_ref(int)}
   */
  public static native void delete_local_ref(int refCount);

  /**
   * call back {@link #justCallIt()} invokeCount
   */
  public static native void call_java_void_method(int invokeCount);

  /**
   * JNI ERROR (app bug): local reference table overflow (max=512)
   *
   * invoke {@link #getString()}
   */
  public static native void call_java_object_method(int invokeCount);

  /**
   * fix {@link #call_java_object_method(int)}
   */
  public static native void call_java_object_method_recycled(int invokeCount);

  /**
   * JNI ERROR (app bug): local reference table overflow (max=512)
   * call back {@link #consume(String)}
   */
  public static native void call_java_void_method_param(int invokeCount);

  /**
   * fix {@link #call_java_void_method_param(int)}
   */
  public static native void call_java_void_method_param_recycled(int invokeCount);

  public static native void java_pass_param(String str, int invokeCount);



  public static void consume(String str){
    Log.e(TAG, "consume: "+str);
  }

  /**
   * {@link #call_java_void_method}回调
   */
  public static void justCallIt(){
    for (int i = 0; i < 100; i++) {
      Log.e(TAG, "justCallIt: "+i);
    }
  }

  /**
   * {@link #call_java_object_method}, {@link #call_java_object_method_recycled(int)}回调
   */
  public static String getString(){
    return "hello world";
  }
}
