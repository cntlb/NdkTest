package com.example.ndktest;

import android.util.Log;

/**
 * @author jmu
 * @since 2017/8/13
 */

public class NativeMemory {
    private static final String TAG = "NativeMemory";
    static {
        System.loadLibrary("native-lib");
    }


    public static native void c_leek();
    public static native void c_leek_recycled();
    public static native void native_oom(int count);

    /**
     * java oom测试
     * @param mb mega byte
     */
    public static void java_alloc(int mb){
        byte[] buff = new byte[mb*1024*1024];
        Log.e(TAG, "java_alloc: "+buff.length);
    }
    public static native void native_malloc(int mb);

    /**
     * when count=0 approaching to 512:
     * JNI ERROR (app bug): local reference table overflow (max=512)
     */
    public static native void local_ref_jclass(int count);
    public static native void local_ref_jclass_recycled(int count);
    public static native void local_ref_jmethodId(int count);

    public static native void new_object_array(int count);
}
