export module jni_basic_types;

export import jni_bind;

namespace jni {
    // JT+ClassName is a constexpr Class of jni_bind function.
    export constexpr Class JTString{
        "java/lang/String",
        jni::Constructor{},
        jni::Constructor{jstring{}},
    };

    export constexpr Class JTClass{
        "java/lang/Class"
    };

    export constexpr Class JTMethod{
        "java/lang/reflect/Method"
    };

    export constexpr Class JTObject{
        "java/lang/Object",
        jni::Constructor{},
    };
}
