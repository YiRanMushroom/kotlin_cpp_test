#include <cassert>
#include <iostream>

import jni_bind;
import jni_basic_types;
import Easy.Scripting.JniBind;

void providedNativePrintln
(JNIEnv *env, jclass clazz, jstring message) {
    const char *str = env->GetStringUTFChars(message, nullptr);
    std::cout << "Native print: " << str << std::endl;
    env->ReleaseStringUTFChars(message, str);
}

using namespace jni;

int main() {
    JavaVMInitArgs vm_args;
    vm_args.version = Jni_Version_1_6;
    JavaVMOption options[1];
    options[0].optionString = const_cast<char *>("-Djava.class.path=./easy-core-lib-1.0.jar");
    vm_args.nOptions = 1;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = Jni_False;

    using namespace Easy::Script;

    jint rc = JNI_CreateJavaVM(&pjvm, (void **) &env, &vm_args);
    if (rc != Jni_Ok) {
        return -1;
    }
    auto jvm = std::make_unique<jni::JvmRef<jni::kDefaultJvm>>(pjvm);


    constexpr static auto ETLib = ECLib::JniType;

    constexpr StaticRef<ETLib> libStaticRef;
    libStaticRef.Call<"Init">();

    LocalObject easyLibClassObj = libStaticRef.Call<"GetClass">("com.easy.Lib");
    libStaticRef.Call<"PrintClassInfo">(easyLibClassObj);
    LocalObject jstringClassObj = libStaticRef.Call<"GetClass">("java.lang.String");
    LocalObject jclassClassObj = libStaticRef.Call<"GetClass">("java.lang.Class");

    LocalArray<jobject, 1, JTClass> paraArray{3};
    paraArray.Set(0, static_cast<jobject>(easyLibClassObj));
    paraArray.Set(1, static_cast<jobject>(jstringClassObj));
    paraArray.Set(2, static_cast<jobject>(jclassClassObj));

    auto targetMethod = libStaticRef.Call<"GetMethodFromClassAndFunctionName">(
        easyLibClassObj, "GetMethodFromClassAndFunctionName",
        paraArray);

    LocalString targetMethodName{"GetMethodFromClassAndFunctionName"};

    LocalArray<jobject, 1, JTObject> anotherParaArray{3};
    anotherParaArray.Set(0, static_cast<jobject>(easyLibClassObj));
    anotherParaArray.Set(1, static_cast<jobject>(static_cast<jstring>(targetMethodName)));
    anotherParaArray.Set(2, static_cast<jobject>(paraArray));

    auto targetMethod2 = libStaticRef.Call<"CallStaticMethod">(targetMethod, anotherParaArray);

    GlobalObject<JTMethod> targetMethodGlobalRef{PromoteToGlobal{}, static_cast<jobject>(targetMethod)};

    auto obj2 = std::move(targetMethodGlobalRef);

    assert(targetMethod == targetMethod2);
}

void app() {
    using namespace Easy::Script;

    static constexpr jni::Class URLClass{
        "java/net/URL",
        jni::Constructor{},
        jni::Constructor{jstring{}},
        jni::Method{"toString", jni::Return{jstring{}}, jni::Params{}}
    };

    jni::LocalObject<URLClass> urlObj{"file:./test-kotlin-cpp-1.0-SNAPSHOT.jar"};
    std::cout << "URL: " << urlObj.Call<"toString">().Pin().ToString() << std::endl;

    static constexpr Class _forWard_Class = Class{"java/lang/Class"};

    static constexpr Class Clazz{
        "java/lang/Class",
        Static{
            Method{
                "forName",
                jni::Return{_forWard_Class}, Params{jstring{}}
            }
        },
        jni::Method{"toString", jni::Return{jstring{}}, jni::Params{}}
    };

    static constexpr jni::Class URLClassLoaderClass{
        "java/net/URLClassLoader",
        // setParent
        jni::Constructor{},
        jni::Constructor{jni::Array{URLClass}},
        jni::Method{"loadClass", jni::Return{Clazz}, Params{jstring{}}},
    };

    LocalArray<jobject, 1, URLClass> urls{1};
    urls.Set(0, static_cast<jobject>(urlObj));

    LocalObject<URLClassLoaderClass> urlClassLoaderObj{urls};

    auto clazz = urlClassLoaderObj.Call<"loadClass">("com.easy.Lib");

    std::cout << "Class: " << clazz.Call<"toString">().Pin().ToString() << std::endl;

    JNINativeMethod method{
        const_cast<char *>("nativePrintln"),
        const_cast<char *>("(Ljava/lang/String;)V"),
        reinterpret_cast<void *>(&providedNativePrintln)
    };

    jint result = env->RegisterNatives(
        static_cast<jclass>(static_cast<jobject>(clazz)),
        &method,
        1
    );

    assert(result == Jni_Ok);

    auto loadedClass = static_cast<jclass>(static_cast<jobject>(clazz));

    jmethodID methodId = env->GetStaticMethodID(loadedClass, "nativePrintln", "(Ljava/lang/String;)V");

    if (methodId == nullptr) {
        std::cerr << "Failed to get method ID" << std::endl;
        if (env->ExceptionCheck()) {
            env->ExceptionDescribe();
            env->ExceptionClear();
        }
        return;
    }

    jstring testString = env->NewStringUTF("Test from native code");

    env->CallStaticVoidMethod(loadedClass, methodId, testString);

    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }

    env->DeleteLocalRef(testString);
}
