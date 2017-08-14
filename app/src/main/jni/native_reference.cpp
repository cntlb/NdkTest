#include <jni.h>
#include "mclog.h"
#include <stdio.h>

#define LOG_TAG "native-lib.cpp"

#define NativeReference "com/example/ndktest/NativeReference"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_com_example_ndktest_NativeReference_no_1delete_1local_1ref(JNIEnv *env, jclass type,
                                                                jint refCount) {
    for (int i = 0; i < refCount; i++) {
        //没有使用任何变量接收返回值, 有返回值的函数在汇编中都会至少使用
        //一个寄存器接收,因此会在local reference table创建到java对象的引用
        //另一方面理解可以认为访问了java对象就会在local reference table创建引用
        //而这条调用语句又是匿名的(不接收返回值),因此也无法释放引用
        env->NewStringUTF("Hello from C++");
    }
}

JNIEXPORT void JNICALL
Java_com_example_ndktest_NativeReference_delete_1local_1ref(JNIEnv *env, jclass type,
                                                            jint refCount) {
    for (int i = 0; i < refCount; i++) {
        jstring pJstring = env->NewStringUTF("Hello from C++");
        env->DeleteLocalRef(pJstring);
    }
}


JNIEXPORT void JNICALL
Java_com_example_ndktest_NativeReference_call_1java_1void_1method(JNIEnv *env, jclass type,
                                                                  jint invokeCount) {
    static jmethodID id = env->GetStaticMethodID(type, "justCallIt", "()V");
    for (int i = 0; i < invokeCount; ++i) {
        //基本数据类型的返回值无需释放
        //jboolean, jbyte, jchar, jshort, jint, jlong, jfloat, jdouble, void
        env->CallStaticVoidMethod(type, id);
    }
}

JNIEXPORT void JNICALL
Java_com_example_ndktest_NativeReference_call_1java_1object_1method(JNIEnv *env, jclass type,
                                                                    jint invokeCount) {

    static jmethodID id = env->GetStaticMethodID(type, "getString", "()Ljava/lang/String;");
    for (int i = 0; i < invokeCount; ++i) {
        //jobject返回值类型的应当及时释放
        //jobject jclass jstring jarray jobjectArray
        // jbooleanArray jbyteArray jcharArray jshortArray jintArray jlongArray jfloatArray jdoubleArray
        // jthrowable jweak;
        env->CallStaticObjectMethod(type, id);
    }
}


JNIEXPORT void JNICALL
Java_com_example_ndktest_NativeReference_call_1java_1object_1method_1recycled(JNIEnv *env,
                                                                              jclass type,
                                                                              jint invokeCount) {

    static jmethodID id = env->GetStaticMethodID(type, "getString", "()Ljava/lang/String;");
    for (int i = 0; i < invokeCount; ++i) {
        jstring str = (jstring) env->CallStaticObjectMethod(type, id);
        env->DeleteLocalRef(str);
    }
}


JNIEXPORT void JNICALL
Java_com_example_ndktest_NativeReference_call_1java_1void_1method_1param(JNIEnv *env, jclass type,
                                                                         jint invokeCount) {

    static jmethodID id = env->GetStaticMethodID(type, "consume", "(Ljava/lang/String;)V");
    for (int i = 0; i < invokeCount; ++i) {
        char buff[100];
        sprintf(buff, "%s  %d", "Hello from C++", i);
        jstring str = env->NewStringUTF(buff);
        //jni创建的java对象通过调用java方法传递, java层并不会对其进行垃圾回收
        //使用完不释放, 可能导致 local reference table overflow
        env->CallStaticVoidMethod(type, id, str);
    }
}


JNIEXPORT void JNICALL
Java_com_example_ndktest_NativeReference_call_1java_1void_1method_1param_1recycled(JNIEnv *env,
    jclass type, jint invokeCount) {
    static jmethodID id = env->GetStaticMethodID(type, "consume", "(Ljava/lang/String;)V");
    for (int i = 0; i < invokeCount; ++i) {
        char buff[100];//局部变量
        sprintf(buff, "%s  %d", "Hello from C++", i);
        jstring str = env->NewStringUTF(buff);
        env->CallStaticVoidMethod(type, id, str);
        //及时删除引用
        env->DeleteLocalRef(str);

        //buff是c++局部变量, 不受DeleteLocalRef的影响
        //同java局部变量,在局部作用范围以外就会被自动释放
        LOGE("jni out: %s", buff)//不影响buff, 照样使用
        //释放栈内存变量buff
    }
}

JNIEXPORT void JNICALL
Java_com_example_ndktest_NativeReference_java_1pass_1param(JNIEnv *env, jclass type,
    jstring str, jint invokeCount){
    for (int i = 0; i < invokeCount; ++i) {
        const char *chars = env->GetStringUTFChars(str, nullptr);
        LOGE("string:%s  %d", chars, i)
        //Returns a pointer to an array of bytes representing the string
        // in modified UTF-8 encoding. This array is valid until it
        // is released by ReleaseStringUTFChars().
        env->ReleaseStringUTFChars(str, chars);
        LOGE("after release string:%s  %d", chars, i)//chars is valid
    }
}

#ifdef __cplusplus
}
#endif
