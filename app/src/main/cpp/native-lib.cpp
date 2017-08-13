#include <jni.h>
#include <string>
#include <memory.h>
#include <malloc.h>
#include <iostream>
#include <stdio.h>
#include <android/log.h>

#define LOGE(fmt, argv...) __android_log_print(ANDROID_LOG_ERROR, "native-lib.cpp", fmt, ##argv);




extern "C" {
JNIEXPORT void JNICALL
Java_com_example_ndktest_NativeMemory_c_1leek(JNIEnv *env, jclass type) {

//    char* buff = (char *) malloc(1024 * 1024);

    void* pVoid = operator new(1024 * 1024);
    void *pVoid1 = operator new[](10);
    LOGE("ptr = %p", pVoid)
}

JNIEXPORT void JNICALL
Java_com_example_ndktest_NativeMemory_c_1leek_1recycled(JNIEnv *env, jclass type) {
    void* pVoid = operator new(1024 * 1024);
    delete(pVoid);
}

JNIEXPORT void JNICALL
Java_com_example_ndktest_NativeMemory_native_1oom(JNIEnv *env, jclass type, jint count) {
    jstring str;
    env->PushLocalFrame(1000);
    for (int i = 0; i < count; ++i) {
        str = env->NewStringUTF("hello");
    }
}

JNIEXPORT void JNICALL
Java_com_example_ndktest_NativeMemory_local_1ref_1jclass(JNIEnv *env, jclass type, jint count) {
    for (int i = 0; i < count; ++i) {
        env->FindClass("com/example/ndktest/MainActivity");
    }
}
JNIEXPORT void JNICALL
Java_com_example_ndktest_NativeMemory_local_1ref_1jclass_1recycled(JNIEnv *env, jclass type, jint count) {
    for (int i = 0; i < count; ++i) {
        jclass klass = env->FindClass("com/example/ndktest/MainActivity");
        env->DeleteLocalRef(klass);
    }
}


JNIEXPORT void JNICALL
Java_com_example_ndktest_NativeMemory_local_1ref_1jmethodId(JNIEnv *env, jclass type, jint count) {
    jclass klass = env->FindClass("com/example/ndktest/MainActivity");
    for (int i = 0; i < count; ++i) {
        jmethodID logId = env->GetStaticMethodID(klass, "log", "(I)V");
        env->CallStaticVoidMethod(klass, logId, i);
    }
}


JNIEXPORT jstring JNICALL
Java_com_example_ndktest_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
}