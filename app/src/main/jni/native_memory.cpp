#include <jni.h>
#include <malloc.h>
#include "mclog.h"

#define LOG_TAG "native-lib.cpp"

#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT void JNICALL
Java_com_example_ndktest_NativeMemory_c_1leek(JNIEnv *env, jclass type) {
    //内存泄露
//    char* buff = (char *) malloc(1024 * 1024);
    char* ptr = (char *) operator new(1024 * 1024);
    LOGE("ptr = %p", (void*)ptr)
}

JNIEXPORT void JNICALL
Java_com_example_ndktest_NativeMemory_c_1leek_1recycled(JNIEnv *env, jclass type) {
    char* ptr = (char *) operator new(1024 * 1024);
    delete(ptr);//及时回收不再使用内存
}

JNIEXPORT void JNICALL
Java_com_example_ndktest_NativeMemory_native_1oom(JNIEnv *env, jclass type, jint count) {
    jstring str;//局部变量
    for (int i = 0; i < count; ++i) {
        //不断使用 Local Reference table,导致JNI ERROR (app bug): local reference table overflow (max=512)
        str = env->NewStringUTF("0");
    }
}

JNIEXPORT void JNICALL
Java_com_example_ndktest_NativeMemory_native_1malloc(JNIEnv *env, jclass type, jint mb) {
    //dynamic alloc memory mb * 1024 * 1024 bytes in heep
    char *ptr = (char *) malloc((size_t) (mb * 1024 * 1024));
    LOGE("allocate heep memory %dmb", mb)
    //release
    delete(ptr);
}

JNIEXPORT void JNICALL
Java_com_example_ndktest_NativeMemory_local_1ref_1jclass(JNIEnv *env, jclass type, jint count) {
    for (int i = 0; i < count; ++i) {
        //jclass同样可以导致local reference table overflow
        env->FindClass("com/example/ndktest/MainActivity");
    }
}
JNIEXPORT void JNICALL
Java_com_example_ndktest_NativeMemory_local_1ref_1jclass_1recycled(JNIEnv *env, jclass type, jint count) {
    for (int i = 0; i < count; ++i) {
        jclass klass = env->FindClass("com/example/ndktest/MainActivity");
        env->DeleteLocalRef(klass);//用完删除在local reference table的引用
    }
}


JNIEXPORT void JNICALL
Java_com_example_ndktest_NativeMemory_local_1ref_1jmethodId(JNIEnv *env, jclass type, jint count) {
    jclass klass = env->FindClass("com/example/ndktest/MainActivity");
    for (int i = 0; i < count; ++i) {
        //jmethodID不属于jobject族,不会造成local reference table overflow
        //也无法通过DeleteLocalRef删除引用
        jmethodID logId = env->GetStaticMethodID(klass, "log", "(I)V");
        env->CallStaticVoidMethod(klass, logId, i);
        //强制转换类型
        //native code passing in reference to invalid stack indirect reference table or invalid reference
        //! env->DeleteLocalRef((jobject) logId);
    }
}


JNIEXPORT void JNICALL
Java_com_example_ndktest_NativeMemory_new_1object_1array(JNIEnv *env, jclass type, jint count) {
    jclass stringClass = env->FindClass("java/lang/String");
    jstring hello = env->NewStringUTF("hello");
    for (int i = 0; i < count; ++i) {
        jobjectArray array = env->NewObjectArray(5, stringClass, hello);
        //do sth
        env->DeleteLocalRef(array);
    }
    env->DeleteLocalRef(hello);
    env->DeleteLocalRef(stringClass);
}

#ifdef __cplusplus
}
#endif