package com.example.ndktest;

/**
 * @author jmu
 * @since 2017/8/13
 */

public class NativeMemory {
    static {
        System.loadLibrary("native-lib");
    }


    public static native void c_leek();
    public static native void c_leek_recycled();
    public static native void native_oom(int count);

    /**
     * when count=0 approaching to 512:
     * JNI ERROR (app bug): local reference table overflow (max=512)
     */
    public static native void local_ref_jclass(int count);
    public static native void local_ref_jclass_recycled(int count);
    public static native void local_ref_jmethodId(int count);

}
