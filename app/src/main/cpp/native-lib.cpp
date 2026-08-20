#include <jni.h>
#include <string>
#include <vector>
#include "AXmlCompiler.hpp"

extern "C" JNIEXPORT jstring JNICALL
Java_com_binclab_ide_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jbyteArray JNICALL
Java_com_binclab_ide_MainActivity_compileManifest(
        JNIEnv* env,
        jobject /* this */,
        jstring xmlContent) {

    const char* nativeXml = env->GetStringUTFChars(xmlContent, nullptr);
    std::string xml(nativeXml);
    env->ReleaseStringUTFChars(xmlContent, nativeXml);

    axml::AXmlCompiler compiler;
    std::vector<uint8_t> binary = compiler.compile(xml);

    jbyteArray result = env->NewByteArray(binary.size());
    env->SetByteArrayRegion(result, 0, binary.size(), reinterpret_cast<const jbyte*>(binary.data()));

    return result;
}