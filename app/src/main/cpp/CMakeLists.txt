# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.



# OpenCV libs
set(OpenCV_DIR "/home/ale/AndroidStudioProjects/opencv3-android-sdk-with-contrib/OpenCV-android-sdk/sdk/native/jni")
find_package(OpenCV REQUIRED)

# Opengles3, egl----header files and libs.
#include_directories( External/Include )
#find_library( OPENGLES3_LIBRARY GLESv3 "OpenGL ES v3.0 library")
#find_library( EGL_LIBRARY EGL "EGL 1.4 library" )


# glm extensions

# target generation
find_library(log-lib    log)
add_library(native-lib    SHARED    native-lib.cpp)
target_link_libraries(native-lib

        ${log-lib}
        jnigraphics android
        GLESv3 EGL
        #${OPENGLES3_LIBRARY}
        #${EGL_LIBRARY}
        ${OpenCV_LIBS})