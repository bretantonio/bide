# Binary XML Compiler for AndroidManifest.xml

This plan outlines the implementation of a C++ based compiler that converts a standard `AndroidManifest.xml` text file into the Android Binary XML (AXMLE) format. This is a crucial component for the BIDE IDE to build APKs.

## User Review Required

> [!IMPORTANT]
> The AXMLE format requires a String Pool and specific chunk-based structure. This implementation will focus on the core manifest compilation needed for APK building.

## Proposed Changes

### Native Layer (C++)

#### [NEW] [AXmlCompiler.hpp](file:///C:/Users/mrkeo/AndroidStudioProjects/BIDE/app/src/main/cpp/AXmlCompiler.hpp)
Defines the structures and classes for String Pool management and AXMLE chunk writing.

#### [NEW] [AXmlCompiler.cpp](file:///C:/Users/mrkeo/AndroidStudioProjects/BIDE/app/src/main/cpp/AXmlCompiler.cpp)
Implements the binary XML compilation logic, including:
- String pool generation.
- XML tag/attribute encoding.
- Resource ID mapping (basic support).

#### [MODIFY] [native-lib.cpp](file:///C:/Users/mrkeo/AndroidStudioProjects/BIDE/app/src/main/cpp/native-lib.cpp)
Adds the JNI entry point `compileManifest` to expose the compiler to Java.

#### [MODIFY] [CMakeLists.txt](file:///C:/Users/mrkeo/AndroidStudioProjects/BIDE/app/src/main/cpp/CMakeLists.txt)
Includes the new source files in the build process and potentially adds an XML parser dependency (e.g., `tinyxml2`).

### Java Layer

#### [MODIFY] [MainActivity.java](file:///C:/Users/mrkeo/AndroidStudioProjects/BIDE/app/src/main/java/com/binclab/ide/MainActivity.java)
Adds the `compileManifest` native declaration and a simple test case to verify the compilation.

## Verification Plan

### Automated Tests
- I will implement a unit test in C++ or a simple verification in Java that compares the output of the compiler with a known binary manifest or checks if it's readable by Android's `AssetManager`.

### Manual Verification
- Deploy the app and trigger a manifest compilation, then check if the resulting binary file has the correct `0x00080003` magic header.
