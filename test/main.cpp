#include <cassert>
#include <iostream>
#include <jni_bind.h>

JNIEXPORT void JNICALL providedNativePrintln
(JNIEnv *env, jclass clazz, jstring message) {
    const char *str = env->GetStringUTFChars(message, nullptr);
    std::cout << "Native print: " << str << std::endl;
    env->ReleaseStringUTFChars(message, str);
}

int main() {
    // 1. 设置JVM启动参数
    JavaVMInitArgs vm_args;
    vm_args.version = JNI_VERSION_1_6;
    JavaVMOption options[1];
    options[0].optionString = const_cast<char *>("-Djava.class.path=.");
    vm_args.nOptions = 1;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = JNI_FALSE;

    // 2. 创建JVM
    JavaVM *pjvm;
    JNIEnv *env;
    jint rc = JNI_CreateJavaVM(&pjvm, (void **) &env, &vm_args);
    if (rc != JNI_OK) {
        return -1;
    }

    auto jvm = std::make_unique<jni::JvmRef<jni::kDefaultJvm>>(pjvm);

    static constexpr jni::Class StringClass{
        "java/lang/String",
        jni::Constructor{},
        jni::Constructor{jstring{}},
        jni::Method{"toString", jni::Return{jstring{}}, jni::Params{}},
    };

    jni::LocalObject<StringClass> stringObj("Hello");
    jni::LocalString str{stringObj.Call<"toString">()};
    std::cout << "String: " << str.Pin().ToString() << std::endl;

    static constexpr jni::Class URLClass{
        "java/net/URL",
        jni::Constructor{},
        jni::Constructor{jstring{}},
        jni::Method{"toString", jni::Return{jstring{}}, jni::Params{}}
    };

    jni::LocalObject<URLClass> urlObj{"file:./test-kotlin-cpp-1.0-SNAPSHOT.jar"};
    std::cout << "URL: " << urlObj.Call<"toString">().Pin().ToString() << std::endl;

    using namespace jni;

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

    assert(result == JNI_OK);

    auto loadedClass = static_cast<jclass>(static_cast<jobject>(clazz));

    jmethodID methodId = env->GetStaticMethodID(loadedClass, "nativePrintln", "(Ljava/lang/String;)V");

    if (methodId == nullptr) {
        std::cerr << "Failed to get method ID" << std::endl;
        if (env->ExceptionCheck()) {
            env->ExceptionDescribe();
            env->ExceptionClear();
        }
        return -1;
    }

    // 2. 创建测试字符串
    jstring testString = env->NewStringUTF("Test from native code");

    // 3. 调用静态方法
    env->CallStaticVoidMethod(loadedClass, methodId, testString);

    // 检查是否有异常
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }

    // 清理本地引用
    env->DeleteLocalRef(testString);
}

void app() {}
