module;

#include <jni_bind.h>

export module jni_bind;

export {
    using ::JNIEnv;
    using ::jclass;
    using ::jstring;
    using ::jobject;
    using ::JavaVMInitArgs;
    constexpr auto Jni_Version_1_6 = JNI_VERSION_1_6;
    using ::JavaVMOption;
    constexpr auto Jni_False = JNI_FALSE;
    constexpr auto Jni_True = JNI_TRUE;
    constexpr auto Jni_Ok = JNI_OK;
    using ::JavaVM;
    using ::jint;
    using ::jlong;
    using ::jbyte;
    using ::jboolean;
    using ::jchar;
    using ::jfloat;
    using ::jdouble;
    using ::jshort;
    using ::jobjectArray;
    using ::jclass;
    using ::jmethodID;
    using ::jfieldID;
    using ::jthrowable;
    using ::jvalue;
    using ::JNI_CreateJavaVM;
    using ::JNINativeMethod;
}

export using jni::operator==;
export using jni::operator!=;

export namespace jni {
    using jni::JvmRef;
    using jni::kDefaultJvm;
    using jni::LocalString;
    using jni::GlobalString;
    using jni::LocalObject;
    using jni::GlobalObject;
    using jni::LocalArray;
    using jni::Class;
    using jni::Method;
    using jni::Static;
    using jni::Constructor;
    using jni::Field;
    using jni::Return;
    using jni::Params;
    using jni::Array;
    using jni::StaticRef;
    using jni::PromoteToGlobal;
    using jni::AdoptGlobal;

}