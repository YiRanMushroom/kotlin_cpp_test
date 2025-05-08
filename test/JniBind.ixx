export module Easy.Scripting.JniBind;

export import jni_basic_types;

namespace Easy {
    using namespace jni;

    namespace Script {
        export inline JNIEnv *env;
        export inline JavaVM *pjvm;

        export class ECLib {
        public:
            inline static constexpr auto JniType = Class{
                "com/easy/Lib",
                Static{
                    Method{"Init", Return{}, Params{}},
                    Method{"GetClass", Return{JTClass}, Params{jstring{}}},
                    Method{"PrintClassInfo", Return{}, Params{JTClass}},
                    Method{
                        "GetMethodFromClassAndFunctionName",
                        Return{JTMethod}, Params{JTClass, jstring{}, Array{JTClass}}
                    },
                    Method{
                        "CallStaticMethod",
                        Return{JTObject}, Params{JTMethod, Array{JTObject}}
                    },
                },
            };

            inline static constexpr auto StaticRef = jni::StaticRef<JniType>();

            inline static GlobalObject<JTClass> JClassInstance;

            void Init();

            void Shutdown();
        };
    }
}
