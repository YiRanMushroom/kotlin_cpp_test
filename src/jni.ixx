module;

#include "jni.h"

#include "jnipp.h"

export module jni;

namespace jni {
    export using jni::JNIEnv;
    export using jni::JavaVM;
    export using jni::jclass;
    export using jni::jobject;
    export using jni::jarray;
    export using jni::Array;
    export using jni::byte_t;
    export using jni::Class;
    export using jni::Object;
    export using jni::Enum;
    export using jni::env;
    export using jni::Exception;
    export using jni::method_t;
    export using jni::field_t;
    export using jni::InitializationException;
    export using jni::Vm;
}

namespace jni {
    export class UrlType : public Object {
    public:
        explicit UrlType(Object obj) : Object(obj) {}

        static std::string getValueSig() {
            return "Ljava/net/URL;";
        }
    };
}
