cmake_minimum_required(VERSION 3.22)

# JUCE directory detection
# Source/cpp/CMakeLists.txt is at Source/cpp/, so JUCEture root is at ../..
get_filename_component(JUCETURE_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)

# Debug: Output path information
message(STATUS "=== JUCEture CMake Debug Info ===")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_LIST_DIR: ${CMAKE_CURRENT_LIST_DIR}")
message(STATUS "JUCETURE_ROOT_DIR: ${JUCETURE_ROOT_DIR}")

# Priority: Explicitly specified paths (CMake variable / cache / environment variable) > Relative paths
set(JUCE_DIR_FOUND "")

# Step 1: Check CMake variable or cache first
if(DEFINED JUCE_MODULES_DIR AND EXISTS "${JUCE_MODULES_DIR}")
    message(STATUS "Using JUCE_MODULES_DIR from CMake variable: ${JUCE_MODULES_DIR}")
    get_filename_component(JUCE_DIR_FOUND "${JUCE_MODULES_DIR}/.." ABSOLUTE)
elseif(DEFINED JUCE_DIR AND EXISTS "${JUCE_DIR}/modules")
    message(STATUS "Using JUCE_DIR from CMake variable: ${JUCE_DIR}")
    get_filename_component(JUCE_DIR_FOUND "${JUCE_DIR}" ABSOLUTE)
    get_filename_component(JUCE_MODULES_DIR "${JUCE_DIR_FOUND}/modules" ABSOLUTE)
    set(JUCE_MODULES_DIR "${JUCE_MODULES_DIR}" CACHE PATH "Path to JUCE modules directory" FORCE)
elseif(DEFINED ENV{JUCE_DIR} AND EXISTS "$ENV{JUCE_DIR}/modules")
    message(STATUS "Using JUCE_DIR from environment variable: $ENV{JUCE_DIR}")
    get_filename_component(JUCE_DIR_FOUND "$ENV{JUCE_DIR}" ABSOLUTE)
    get_filename_component(JUCE_MODULES_DIR "${JUCE_DIR_FOUND}/modules" ABSOLUTE)
    set(JUCE_MODULES_DIR "${JUCE_MODULES_DIR}" CACHE PATH "Path to JUCE modules directory" FORCE)
endif()

# Step 2: If not specified, search relative paths from JUCEture root
if(NOT JUCE_MODULES_DIR)
    set(JUCE_RELATIVE_PATHS_RAW
        "${JUCETURE_ROOT_DIR}/../JUCE"             # Parent directory's JUCE (e.g. FlashMashApp/JUCE)
        "${JUCETURE_ROOT_DIR}/external/JUCE"       # JUCEture root/external/JUCE
        "${JUCETURE_ROOT_DIR}/JUCE"                # JUCEture root/JUCE
        "${JUCETURE_ROOT_DIR}/../../JUCE"
        "${JUCETURE_ROOT_DIR}/../../../JUCE"
    )

    message(STATUS "Searching for JUCE directory in relative paths:")
    foreach(PATH_RAW ${JUCE_RELATIVE_PATHS_RAW})
        get_filename_component(PATH_ABS "${PATH_RAW}" ABSOLUTE)
        if(NOT JUCE_DIR_FOUND)
            message(STATUS "  - ${PATH_ABS}")
            if(EXISTS "${PATH_ABS}/modules")
                message(STATUS "    -> Found JUCE directory")
                set(JUCE_DIR_FOUND "${PATH_ABS}")
                get_filename_component(JUCE_MODULES_DIR "${JUCE_DIR_FOUND}/modules" ABSOLUTE)
                set(JUCE_MODULES_DIR "${JUCE_MODULES_DIR}" CACHE PATH "Path to JUCE modules directory" FORCE)
            endif()
        endif()
    endforeach()
endif()

if(NOT JUCE_MODULES_DIR)
    message(FATAL_ERROR
        "JUCE directory not found.\n"
        "Please set JUCE_DIR to the path containing JUCE (the directory that contains 'modules' subdirectory).\n"
        "Example: cmake -DJUCE_DIR=/path/to/juce ..."
    )
endif()

message(STATUS "JUCE_MODULES_DIR: ${JUCE_MODULES_DIR}")
message(STATUS "=== End JUCEture CMake Debug Info ===")

# Export JUCE_MODULES_DIR for parent CMakeLists.txt if needed
set(JUCE_MODULES_DIR ${JUCE_MODULES_DIR} CACHE PATH "Path to JUCE modules directory")

# JuceLibraryCode directory detection
set(JUCELIBRARYCODE_DIR_FOUND "")

# Step 1: Check CMake variable or cache first
if(DEFINED JUCELIBRARYCODE_DIR AND EXISTS "${JUCELIBRARYCODE_DIR}/JuceHeader.h")
    message(STATUS "Using JUCELIBRARYCODE_DIR from CMake variable: ${JUCELIBRARYCODE_DIR}")
    set(JUCELIBRARYCODE_DIR_FOUND "${JUCELIBRARYCODE_DIR}")
elseif(DEFINED ENV{JUCELIBRARYCODE_DIR} AND EXISTS "$ENV{JUCELIBRARYCODE_DIR}/JuceHeader.h")
    message(STATUS "Using JUCELIBRARYCODE_DIR from environment variable: $ENV{JUCELIBRARYCODE_DIR}")
    get_filename_component(JUCELIBRARYCODE_DIR_FOUND "$ENV{JUCELIBRARYCODE_DIR}" ABSOLUTE)
    set(JUCELIBRARYCODE_DIR "${JUCELIBRARYCODE_DIR_FOUND}" CACHE PATH "Path to JuceLibraryCode directory" FORCE)
endif()

# Step 2: If not specified, search relative paths from JUCEture root
if(NOT JUCELIBRARYCODE_DIR_FOUND)
    set(JUCELIBRARYCODE_RELATIVE_PATHS_RAW
        "${JUCETURE_ROOT_DIR}/../JuceLibraryCode"        # Parent directory's JuceLibraryCode
        "${JUCETURE_ROOT_DIR}/JuceLibraryCode"           # JUCEture root/JuceLibraryCode
        "${JUCETURE_ROOT_DIR}/external/JuceLibraryCode"  # JUCEture root/external/JuceLibraryCode
        "${JUCETURE_ROOT_DIR}/../../JuceLibraryCode"
        "${JUCETURE_ROOT_DIR}/../../../JuceLibraryCode"
    )

    message(STATUS "Searching for JuceLibraryCode directory in relative paths:")
    foreach(PATH_RAW ${JUCELIBRARYCODE_RELATIVE_PATHS_RAW})
        get_filename_component(PATH_ABS "${PATH_RAW}" ABSOLUTE)
        if(NOT JUCELIBRARYCODE_DIR_FOUND)
            message(STATUS "  - ${PATH_ABS}")
            if(EXISTS "${PATH_ABS}/JuceHeader.h")
                message(STATUS "    -> Found JuceLibraryCode directory")
                set(JUCELIBRARYCODE_DIR_FOUND "${PATH_ABS}")
                set(JUCELIBRARYCODE_DIR "${JUCELIBRARYCODE_DIR_FOUND}" CACHE PATH "Path to JuceLibraryCode directory" FORCE)
            endif()
        endif()
    endforeach()
endif()

if(NOT JUCELIBRARYCODE_DIR_FOUND)
    message(FATAL_ERROR
        "JuceLibraryCode directory not found.\n"
        "Please set JUCELIBRARYCODE_DIR to the path containing JuceLibraryCode (the directory that contains 'JuceHeader.h' file).\n"
        "Example: cmake -DJUCELIBRARYCODE_DIR=/path/to/JuceLibraryCode ..."
    )
endif()

# Export JUCELIBRARYCODE_DIR for parent CMakeLists.txt if needed
set(JUCELIBRARYCODE_DIR ${JUCELIBRARYCODE_DIR} CACHE PATH "Path to JuceLibraryCode directory")

# Export JUCE path to Gradle's build.gradle
# This allows build.gradle to dynamically set Java source directories based on the detected JUCE path
if(JUCE_DIR_FOUND)
    file(WRITE "${JUCETURE_ROOT_DIR}/sampleApp/android/app/juce.properties" "JUCE_MODULES_PATH=${JUCE_DIR_FOUND}/modules")
    message(STATUS "Exported JUCE path to sampleApp/android/app/juce.properties")
endif()

add_library(SourceLib STATIC
    # Common files
    CoordinateAccessor.cpp
    CoordinateAccessor.h
    CoordinateConverter.cpp
    CoordinateConverter.h
    GestureEvent.cpp
    GestureEvent.h
    GestureMediator.cpp
    GestureMediator.h
    JUCEtureAPI.cpp
    JUCEtureAPI.h
    NotifierGestureFromOSJNI.cpp
    NotifierGestureFromOSJNI.h
    # SingleTap
    SingleTap/SingleTapDetector.cpp
    SingleTap/SingleTapDetector.h
    SingleTap/SingleTapEvent.cpp
    SingleTap/SingleTapEvent.h
    SingleTap/ISingleTapHandler.h
    SingleTap/ISingleTapMediator.h
    # LongTap
    LongTap/LongTapDetector.cpp
    LongTap/LongTapDetector.h
    LongTap/LongTapEvent.cpp
    LongTap/LongTapEvent.h
    LongTap/ILongTapHandler.h
    LongTap/ILongTapMediator.h
    # Drag
    Drag/DragDetector.cpp
    Drag/DragDetector.h
    Drag/DragStartEvent.cpp
    Drag/DragStartEvent.h
    Drag/DragMoveEvent.cpp
    Drag/DragMoveEvent.h
    Drag/DragEndEvent.cpp
    Drag/DragEndEvent.h
    Drag/IDragHandler.h
    Drag/IDragMediator.h
    # Pinch
    Pinch/PinchDetector.cpp
    Pinch/PinchDetector.h
    Pinch/PinchStartEvent.cpp
    Pinch/PinchStartEvent.h
    Pinch/PinchScaleEvent.cpp
    Pinch/PinchScaleEvent.h
    Pinch/PinchEndEvent.cpp
    Pinch/PinchEndEvent.h
    Pinch/IPinchHandler.h
    Pinch/IPinchMediator.h
)

target_include_directories(SourceLib PUBLIC
    ${JUCELIBRARYCODE_DIR}
    ${JUCE_MODULES_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/SingleTap
    ${CMAKE_CURRENT_SOURCE_DIR}/LongTap
    ${CMAKE_CURRENT_SOURCE_DIR}/Drag
    ${CMAKE_CURRENT_SOURCE_DIR}/Pinch
)

# Set based on values configured in Projucer
target_compile_definitions(SourceLib PRIVATE
    JUCE_ANDROID=1
    JUCE_ANDROID_API_VERSION=24
    JUCE_PUSH_NOTIFICATIONS=1
    JUCE_PUSH_NOTIFICATIONS_ACTIVITY="com/rmsl/juce/JuceActivity"
    JUCE_ANDROID_GL_ES_VERSION_3_0=1
    JUCER_ANDROIDSTUDIO_7F0E4A25=1
    JUCE_APP_VERSION=1.0.0
    JUCE_APP_VERSION_HEX=0x10000
    JUCE_PROJUCER_VERSION=0x90001
    JUCE_MODULE_AVAILABLE_juce_core=1
    JUCE_MODULE_AVAILABLE_juce_data_structures=1
    JUCE_MODULE_AVAILABLE_juce_events=1
    JUCE_MODULE_AVAILABLE_juce_graphics=1
    JUCE_MODULE_AVAILABLE_juce_gui_basics=1
    JUCE_MODULE_AVAILABLE_juce_gui_extra=1
    JUCE_MODULE_AVAILABLE_juce_opengl=1
    JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
    JUCE_STRICT_REFCOUNTEDPOINTER=1
    JUCE_STANDALONE_APPLICATION=1
    $<$<CONFIG:Debug>:DEBUG=1>
    $<$<CONFIG:Debug>:_DEBUG=1>
    $<$<CONFIG:Release>:NDEBUG=1>
)

