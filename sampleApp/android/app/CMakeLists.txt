# Automatically generated CMakeLists, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

cmake_minimum_required(VERSION 3.22)

project(juce_jni_project)

set(BINARY_NAME "juce_jni")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #C/C++拡張機能にビルド設定を読み込ませて関数ジャンプなどをできるようにしたい

add_library("cpufeatures" STATIC "${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c")
set_source_files_properties("${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c" PROPERTIES COMPILE_FLAGS "-Wno-sign-conversion -Wno-gnu-statement-expression")

add_definitions([[-DJUCE_ANDROID=1]] [[-DJUCE_ANDROID_API_VERSION=24]] [[-DJUCE_PUSH_NOTIFICATIONS=1]] [[-DJUCE_PUSH_NOTIFICATIONS_ACTIVITY="com/rmsl/juce/JuceActivity"]] [[-DJUCE_ANDROID_GL_ES_VERSION_3_0=1]] [[-DJUCER_ANDROIDSTUDIO_7F0E4A25=1]] [[-DJUCE_APP_VERSION=1.0.0]] [[-DJUCE_APP_VERSION_HEX=0x10000]])

include_directories( AFTER
    "../../../JuceLibraryCode"
    "/workspace/JUCE/modules"
    "${ANDROID_NDK}/sources/android/cpufeatures"
    "../../../Source"
    "../../../sampleApp/cpp"
)

# include_directories("../../../external/perfetto/sdk")
# add_library(perfetto STATIC ../../../external/perfetto/sdk/perfetto.cc)

# Sourceディレクトリをサブディレクトリとして追加
add_subdirectory(../../../Source/cpp Source)

enable_language(ASM)

if(JUCE_BUILD_CONFIGURATION MATCHES "DEBUG")
    add_definitions([[-DJUCE_PROJUCER_VERSION=0x8000c]] [[-DJUCE_MODULE_AVAILABLE_juce_core=1]] [[-DJUCE_MODULE_AVAILABLE_juce_data_structures=1]] [[-DJUCE_MODULE_AVAILABLE_juce_events=1]] [[-DJUCE_MODULE_AVAILABLE_juce_graphics=1]] [[-DJUCE_MODULE_AVAILABLE_juce_gui_basics=1]] [[-DJUCE_MODULE_AVAILABLE_juce_gui_extra=1]] [[-DJUCE_MODULE_AVAILABLE_juce_opengl=1]] [[-DJUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1]] [[-DJUCE_STRICT_REFCOUNTEDPOINTER=1]] [[-DJUCE_STANDALONE_APPLICATION=1]] [[-DJUCER_ANDROIDSTUDIO_7F0E4A25=1]] [[-DJUCE_APP_VERSION=1.0.0]] [[-DJUCE_APP_VERSION_HEX=0x10000]] [[-DDEBUG=1]] [[-D_DEBUG=1]])
elseif(JUCE_BUILD_CONFIGURATION MATCHES "RELEASE")
    add_definitions([[-DJUCE_PROJUCER_VERSION=0x8000c]] [[-DJUCE_MODULE_AVAILABLE_juce_core=1]] [[-DJUCE_MODULE_AVAILABLE_juce_data_structures=1]] [[-DJUCE_MODULE_AVAILABLE_juce_events=1]] [[-DJUCE_MODULE_AVAILABLE_juce_graphics=1]] [[-DJUCE_MODULE_AVAILABLE_juce_gui_basics=1]] [[-DJUCE_MODULE_AVAILABLE_juce_gui_extra=1]] [[-DJUCE_MODULE_AVAILABLE_juce_opengl=1]] [[-DJUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1]] [[-DJUCE_STRICT_REFCOUNTEDPOINTER=1]] [[-DJUCE_STANDALONE_APPLICATION=1]] [[-DJUCER_ANDROIDSTUDIO_7F0E4A25=1]] [[-DJUCE_APP_VERSION=1.0.0]] [[-DJUCE_APP_VERSION_HEX=0x10000]] [[-DNDEBUG=1]])
else()
    message( FATAL_ERROR "No matching build-configuration found." )
endif()

add_library( ${BINARY_NAME}

    SHARED

    "../../../sampleApp/cpp/Main.cpp"
    "../../../sampleApp/cpp/MainComponent.h"
    "../../../sampleApp/cpp/MainComponent.cpp"
    "../../../sampleApp/cpp/GestureTestBox.h"
    "../../../sampleApp/cpp/GestureTestBox.cpp"
    "../../../JuceLibraryCode/include_juce_core.cpp"
    "../../../JuceLibraryCode/include_juce_core_CompilationTime.cpp"
    "../../../JuceLibraryCode/include_juce_data_structures.cpp"
    "../../../JuceLibraryCode/include_juce_events.cpp"
    "../../../JuceLibraryCode/include_juce_graphics.cpp"
    "../../../JuceLibraryCode/include_juce_graphics_Harfbuzz.cpp"
    "../../../JuceLibraryCode/include_juce_graphics_Sheenbidi.c"
    "../../../JuceLibraryCode/include_juce_gui_basics.cpp"
    "../../../JuceLibraryCode/include_juce_gui_extra.cpp"
    "../../../JuceLibraryCode/include_juce_opengl.cpp"
    "../../../JuceLibraryCode/JuceHeader.h"
)

set_source_files_properties(
    "../../../sampleApp/cpp/MainComponent.h"
    "../../../JuceLibraryCode/JuceHeader.h"
    PROPERTIES HEADER_FILE_ONLY TRUE)

if( JUCE_BUILD_CONFIGURATION MATCHES "DEBUG" )
    target_compile_options( ${BINARY_NAME} PRIVATE "-fsigned-char" )
endif()

if( JUCE_BUILD_CONFIGURATION MATCHES "RELEASE" )
    target_compile_options( ${BINARY_NAME} PRIVATE "-fsigned-char" )
endif()

find_library(log "log")
find_library(android "android")
find_library(glesv3 "GLESv3")
find_library(egl "EGL")

target_link_libraries( ${BINARY_NAME}

    ${log}
    ${android}
    ${glesv3}
    ${egl}
    "cpufeatures"
    SourceLib
)
