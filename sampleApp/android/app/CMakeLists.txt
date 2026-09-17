# Automatically generated CMakeLists, modified for JUCE module architecture
cmake_minimum_required(VERSION 3.22)

project(juce_jni_project)

set(BINARY_NAME "juce_jni")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library("cpufeatures" STATIC "${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c")
set_source_files_properties("${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c" PROPERTIES COMPILE_FLAGS "-Wno-sign-conversion -Wno-gnu-statement-expression")

add_definitions([[-DJUCE_ANDROID=1]] [[-DJUCE_ANDROID_API_VERSION=24]] [[-DJUCE_PUSH_NOTIFICATIONS=1]] [[-DJUCE_PUSH_NOTIFICATIONS_ACTIVITY="com/rmsl/juce/JuceActivity"]] [[-DJUCE_ANDROID_GL_ES_VERSION_3_0=1]] [[-DJUCER_ANDROIDSTUDIO_7F0E4A25=1]] [[-DJUCE_APP_VERSION=1.0.0]] [[-DJUCE_APP_VERSION_HEX=0x10000]])

include_directories( AFTER
    "${ANDROID_NDK}/sources/android/cpufeatures"
    "../../../sampleApp/cpp"
)

# JUCE CMake & juceture module setup
set(JUCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../../JUCE" CACHE PATH "Path to JUCE directory" FORCE)
set(JUCE_MODULES_ONLY ON CACHE BOOL "Only configure JUCE modules" FORCE)
add_subdirectory(${JUCE_DIR} JUCE EXCLUDE_FROM_ALL)
juce_add_module(../../../juceture)

enable_language(ASM)

if(JUCE_BUILD_CONFIGURATION MATCHES "DEBUG")
    add_definitions([[-DJUCE_MODULE_AVAILABLE_juce_core=1]] [[-DJUCE_MODULE_AVAILABLE_juce_data_structures=1]] [[-DJUCE_MODULE_AVAILABLE_juce_events=1]] [[-DJUCE_MODULE_AVAILABLE_juce_graphics=1]] [[-DJUCE_MODULE_AVAILABLE_juce_gui_basics=1]] [[-DJUCE_MODULE_AVAILABLE_juce_gui_extra=1]] [[-DJUCE_MODULE_AVAILABLE_juce_opengl=1]] [[-DJUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1]] [[-DJUCE_STRICT_REFCOUNTEDPOINTER=1]] [[-DJUCE_STANDALONE_APPLICATION=1]] [[-DJUCER_ANDROIDSTUDIO_7F0E4A25=1]] [[-DJUCE_APP_VERSION=1.0.0]] [[-DJUCE_APP_VERSION_HEX=0x10000]] [[-DDEBUG=1]] [[-D_DEBUG=1]])
elseif(JUCE_BUILD_CONFIGURATION MATCHES "RELEASE")
    add_definitions([[-DJUCE_MODULE_AVAILABLE_juce_core=1]] [[-DJUCE_MODULE_AVAILABLE_juce_data_structures=1]] [[-DJUCE_MODULE_AVAILABLE_juce_events=1]] [[-DJUCE_MODULE_AVAILABLE_juce_graphics=1]] [[-DJUCE_MODULE_AVAILABLE_juce_gui_basics=1]] [[-DJUCE_MODULE_AVAILABLE_juce_gui_extra=1]] [[-DJUCE_MODULE_AVAILABLE_juce_opengl=1]] [[-DJUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1]] [[-DJUCE_STRICT_REFCOUNTEDPOINTER=1]] [[-DJUCE_STANDALONE_APPLICATION=1]] [[-DJUCER_ANDROIDSTUDIO_7F0E4A25=1]] [[-DJUCE_APP_VERSION=1.0.0]] [[-DJUCE_APP_VERSION_HEX=0x10000]] [[-DNDEBUG=1]])
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
)

set_source_files_properties(
    "../../../sampleApp/cpp/MainComponent.h"
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
    juceture
    juce::juce_gui_extra
    juce::juce_opengl
)
