cmake_minimum_required(VERSION 3.22)

# JUCE directory detection (priority: Relative paths > Environment variable or CMake variable)
# This detection is based on relative paths from JUCEture root (where Source/ directory exists)
# Step 1: Check relative paths first
# JUCEture root is the directory containing Source/ directory
# Source/cpp/CMakeLists.txt is at Source/cpp/, so JUCEture root is at ../
get_filename_component(JUCETURE_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)

# Relative paths from JUCEture root
set(JUCE_RELATIVE_PATHS
    "${JUCETURE_ROOT_DIR}/../../../JUCE"  # JUCEture root/../../../JUCE
    "${JUCETURE_ROOT_DIR}/../../JUCE"     # JUCEture root/../../JUCE
    "${JUCETURE_ROOT_DIR}/../JUCE"        # JUCEture root/../JUCE
    "${JUCETURE_ROOT_DIR}/JUCE"           # JUCEture root/./JUCE
    "${JUCETURE_ROOT_DIR}/external/JUCE"  # JUCEture root/./external/JUCE
    "${JUCETURE_ROOT_DIR}/../external/JUCE"    # JUCEture root/../external/JUCE
    "${JUCETURE_ROOT_DIR}/../../external/JUCE" # JUCEture root/../../external/JUCE
)

# Find JUCE modules directory in relative paths first
find_path(JUCE_MODULES_DIR
    NAMES juce_core/juce_core.h
    PATHS ${JUCE_RELATIVE_PATHS}
    PATH_SUFFIXES modules
    NO_DEFAULT_PATH
    DOC "Path to JUCE modules directory"
)

message(STATUS "JUCE_MODULES_DIR: ${JUCE_MODULES_DIR}")

# Step 2: If not found in relative paths, check environment variable or CMake variable
if(NOT JUCE_MODULES_DIR)
    if(DEFINED JUCE_DIR)
        # CMake variable JUCE_DIR is set
        set(JUCE_SEARCH_PATHS "${JUCE_DIR}")
    elseif(DEFINED ENV{JUCE_DIR})
        # Environment variable JUCE_DIR is set
        set(JUCE_SEARCH_PATHS "$ENV{JUCE_DIR}")
    else()
        # Use relative paths for error message
        set(JUCE_SEARCH_PATHS ${JUCE_RELATIVE_PATHS})
    endif()
    
    # Search in the specified path
    if(JUCE_SEARCH_PATHS)
        find_path(JUCE_MODULES_DIR
            NAMES juce_core/juce_core.h
            PATHS ${JUCE_SEARCH_PATHS}
            PATH_SUFFIXES modules
            NO_DEFAULT_PATH
            DOC "Path to JUCE modules directory"
        )
    endif()
endif()

# If not found, show error with helpful message
if(NOT JUCE_MODULES_DIR)
    message(FATAL_ERROR
        "JUCE modules directory not found.\n"
        "Please set JUCE_DIR to the path containing JUCE modules, or place JUCE in one of:\n"
        "${JUCE_RELATIVE_PATHS}\n"
        "Example: cmake -DJUCE_DIR=/path/to/juce ..."
    )
endif()

# Export JUCE_MODULES_DIR for parent CMakeLists.txt if needed
set(JUCE_MODULES_DIR ${JUCE_MODULES_DIR} CACHE PATH "Path to JUCE modules directory")

add_library(SourceLib STATIC
    # Common files
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
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/SingleTap
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
    JUCE_PROJUCER_VERSION=0x8000c
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

