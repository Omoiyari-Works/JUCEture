cmake_minimum_required(VERSION 3.22)

# JUCE directory detection (priority: Relative paths > Environment variable or CMake variable)
# This detection is based on relative paths from JUCEture root (where Source/ directory exists)
# Step 1: Check relative paths first
# JUCEture root is the directory containing Source/ directory
# Source/cpp/CMakeLists.txt is at Source/cpp/, so JUCEture root is at ../
get_filename_component(JUCETURE_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)

# Debug: Output path information
message(STATUS "=== JUCEture CMake Debug Info ===")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_LIST_DIR: ${CMAKE_CURRENT_LIST_DIR}")
message(STATUS "JUCETURE_ROOT_DIR: ${JUCETURE_ROOT_DIR}")

# Relative paths from JUCEture root
set(JUCE_RELATIVE_PATHS_RAW
    "${JUCETURE_ROOT_DIR}/../../../JUCE"  # JUCEture root/../../../JUCE
    "${JUCETURE_ROOT_DIR}/../../JUCE"     # JUCEture root/../../JUCE
    "${JUCETURE_ROOT_DIR}/../JUCE"        # JUCEture root/../JUCE
    "${JUCETURE_ROOT_DIR}/JUCE"           # JUCEture root/./JUCE
    "${JUCETURE_ROOT_DIR}/external/JUCE"  # JUCEture root/./external/JUCE
    "${JUCETURE_ROOT_DIR}/../external/JUCE"    # JUCEture root/../external/JUCE
    "${JUCETURE_ROOT_DIR}/../../external/JUCE" # JUCEture root/../../external/JUCE
)

# Convert all paths to absolute paths and search for JUCE directory
set(JUCE_RELATIVE_PATHS "")
set(JUCE_DIR_FOUND "")
message(STATUS "Searching for JUCE directory in the following paths:")
foreach(PATH_RAW ${JUCE_RELATIVE_PATHS_RAW})
    get_filename_component(PATH_ABS "${PATH_RAW}" ABSOLUTE)
    list(APPEND JUCE_RELATIVE_PATHS "${PATH_ABS}")
    
    # Check if this path contains the modules directory
    if(NOT JUCE_DIR_FOUND)
        message(STATUS "  - ${PATH_ABS}")
        if(EXISTS "${PATH_ABS}/modules")
            message(STATUS "    -> Found JUCE directory")
            set(JUCE_DIR_FOUND "${PATH_ABS}")
        else()
            if(EXISTS "${PATH_ABS}")
                message(STATUS "    -> EXISTS but modules directory not found")
            else()
                message(STATUS "    -> NOT EXISTS")
            endif()
        endif()
    endif()
endforeach()

# Set JUCE_MODULES_DIR if found
if(JUCE_DIR_FOUND)
    message(STATUS "Found JUCE directory: ${JUCE_DIR_FOUND}")
    get_filename_component(JUCE_MODULES_DIR "${JUCE_DIR_FOUND}/modules" ABSOLUTE)
    message(STATUS "Setting JUCE_MODULES_DIR to: ${JUCE_MODULES_DIR}")
    set(JUCE_MODULES_DIR "${JUCE_MODULES_DIR}" CACHE PATH "Path to JUCE modules directory" FORCE)
else()
    message(STATUS "JUCE directory not found in relative paths")
endif()

message(STATUS "JUCE_MODULES_DIR: ${JUCE_MODULES_DIR}")
message(STATUS "=== End JUCEture CMake Debug Info ===")

# Step 2: If not found in relative paths, check environment variable or CMake variable
if(NOT JUCE_MODULES_DIR)
    if(DEFINED JUCE_DIR)
        # CMake variable JUCE_DIR is set
        message(STATUS "Using JUCE_DIR from CMake variable: ${JUCE_DIR}")
        get_filename_component(JUCE_DIR_ABS "${JUCE_DIR}" ABSOLUTE)
        if(EXISTS "${JUCE_DIR_ABS}/modules")
            message(STATUS "Found JUCE at: ${JUCE_DIR_ABS}")
            get_filename_component(JUCE_MODULES_DIR "${JUCE_DIR_ABS}/modules" ABSOLUTE)
            set(JUCE_MODULES_DIR "${JUCE_MODULES_DIR}" CACHE PATH "Path to JUCE modules directory" FORCE)
        else()
            message(STATUS "JUCE_DIR points to invalid location: ${JUCE_DIR_ABS}")
        endif()
    elseif(DEFINED ENV{JUCE_DIR})
        # Environment variable JUCE_DIR is set
        message(STATUS "Using JUCE_DIR from environment variable: $ENV{JUCE_DIR}")
        get_filename_component(JUCE_DIR_ABS "$ENV{JUCE_DIR}" ABSOLUTE)
        if(EXISTS "${JUCE_DIR_ABS}/modules")
            message(STATUS "Found JUCE at: ${JUCE_DIR_ABS}")
            get_filename_component(JUCE_MODULES_DIR "${JUCE_DIR_ABS}/modules" ABSOLUTE)
            set(JUCE_MODULES_DIR "${JUCE_MODULES_DIR}" CACHE PATH "Path to JUCE modules directory" FORCE)
        else()
            message(STATUS "ENV{JUCE_DIR} points to invalid location: ${JUCE_DIR_ABS}")
        endif()
    else()
        # Try to infer from parent CMakeLists.txt location
        # If called from Builds/Android/app/CMakeLists.txt, JUCE should be at ../../../JUCE
        get_filename_component(PARENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../.." ABSOLUTE)
        set(INFERRED_JUCE_DIR "${PARENT_DIR}/JUCE")
        message(STATUS "Trying inferred JUCE_DIR: ${INFERRED_JUCE_DIR}")
        if(EXISTS "${INFERRED_JUCE_DIR}/modules")
            message(STATUS "Found JUCE at inferred path!")
            get_filename_component(JUCE_MODULES_DIR "${INFERRED_JUCE_DIR}/modules" ABSOLUTE)
            set(JUCE_MODULES_DIR "${JUCE_MODULES_DIR}" CACHE PATH "Path to JUCE modules directory" FORCE)
        else()
            message(STATUS "Inferred path not found")
        endif()
    endif()
    
    message(STATUS "Step 2 result - JUCE_MODULES_DIR: ${JUCE_MODULES_DIR}")
endif()

# If not found, show error with helpful message
if(NOT JUCE_MODULES_DIR)
    message(FATAL_ERROR
        "JUCE directory not found.\n"
        "Please set JUCE_DIR to the path containing JUCE (the directory that contains 'modules' subdirectory), or place JUCE in one of:\n"
        "${JUCE_RELATIVE_PATHS}\n"
        "Example: cmake -DJUCE_DIR=/path/to/juce ..."
    )
endif()

# Export JUCE_MODULES_DIR for parent CMakeLists.txt if needed
set(JUCE_MODULES_DIR ${JUCE_MODULES_DIR} CACHE PATH "Path to JUCE modules directory")

# JuceLibraryCode directory detection (priority: Relative paths > Environment variable or CMake variable)
# This detection is based on relative paths from JUCEture root (where Source/ directory exists)
# Step 1: Check relative paths first
# Relative paths from JUCEture root
set(JUCELIBRARYCODE_RELATIVE_PATHS_RAW
    "${JUCETURE_ROOT_DIR}/../../../JuceLibraryCode"  # JUCEture root/../../../JuceLibraryCode
    "${JUCETURE_ROOT_DIR}/../../JuceLibraryCode"     # JUCEture root/../../JuceLibraryCode
    "${JUCETURE_ROOT_DIR}/../JuceLibraryCode"        # JUCEture root/../JuceLibraryCode
    "${JUCETURE_ROOT_DIR}/JuceLibraryCode"           # JUCEture root/./JuceLibraryCode
    "${JUCETURE_ROOT_DIR}/external/JuceLibraryCode"  # JUCEture root/./external/JuceLibraryCode
    "${JUCETURE_ROOT_DIR}/../external/JuceLibraryCode"    # JUCEture root/../external/JuceLibraryCode
    "${JUCETURE_ROOT_DIR}/../../external/JuceLibraryCode" # JUCEture root/../../external/JuceLibraryCode
)

# Convert all paths to absolute paths and search for JuceLibraryCode directory
set(JUCELIBRARYCODE_RELATIVE_PATHS "")
set(JUCELIBRARYCODE_DIR_FOUND "")
message(STATUS "Searching for JuceLibraryCode directory in the following paths:")
foreach(PATH_RAW ${JUCELIBRARYCODE_RELATIVE_PATHS_RAW})
    get_filename_component(PATH_ABS "${PATH_RAW}" ABSOLUTE)
    list(APPEND JUCELIBRARYCODE_RELATIVE_PATHS "${PATH_ABS}")
    
    # Check if this path contains the JuceHeader.h file
    if(NOT JUCELIBRARYCODE_DIR_FOUND)
        message(STATUS "  - ${PATH_ABS}")
        if(EXISTS "${PATH_ABS}/JuceHeader.h")
            message(STATUS "    -> Found JuceLibraryCode directory")
            set(JUCELIBRARYCODE_DIR_FOUND "${PATH_ABS}")
        else()
            if(EXISTS "${PATH_ABS}")
                message(STATUS "    -> EXISTS but JuceHeader.h not found")
            else()
                message(STATUS "    -> NOT EXISTS")
            endif()
        endif()
    endif()
endforeach()

# Set JUCELIBRARYCODE_DIR if found
if(JUCELIBRARYCODE_DIR_FOUND)
    message(STATUS "Found JuceLibraryCode directory: ${JUCELIBRARYCODE_DIR_FOUND}")
    set(JUCELIBRARYCODE_DIR "${JUCELIBRARYCODE_DIR_FOUND}" CACHE PATH "Path to JuceLibraryCode directory" FORCE)
else()
    message(STATUS "JuceLibraryCode directory not found in relative paths")
endif()

message(STATUS "JUCELIBRARYCODE_DIR: ${JUCELIBRARYCODE_DIR}")

# Step 2: If not found in relative paths, check environment variable or CMake variable
if(NOT JUCELIBRARYCODE_DIR)
    if(DEFINED CACHE{JUCELIBRARYCODE_DIR})
        # CMake variable JUCELIBRARYCODE_DIR is set
        message(STATUS "Using JUCELIBRARYCODE_DIR from CMake variable: ${JUCELIBRARYCODE_DIR}")
        get_filename_component(JUCELIBRARYCODE_DIR_ABS "${JUCELIBRARYCODE_DIR}" ABSOLUTE)
        if(EXISTS "${JUCELIBRARYCODE_DIR_ABS}/JuceHeader.h")
            message(STATUS "Found JuceLibraryCode at: ${JUCELIBRARYCODE_DIR_ABS}")
            set(JUCELIBRARYCODE_DIR "${JUCELIBRARYCODE_DIR_ABS}" CACHE PATH "Path to JuceLibraryCode directory" FORCE)
        else()
            message(STATUS "JUCELIBRARYCODE_DIR points to invalid location: ${JUCELIBRARYCODE_DIR_ABS}")
        endif()
    elseif(DEFINED ENV{JUCELIBRARYCODE_DIR})
        # Environment variable JUCELIBRARYCODE_DIR is set
        message(STATUS "Using JUCELIBRARYCODE_DIR from environment variable: $ENV{JUCELIBRARYCODE_DIR}")
        get_filename_component(JUCELIBRARYCODE_DIR_ABS "$ENV{JUCELIBRARYCODE_DIR}" ABSOLUTE)
        if(EXISTS "${JUCELIBRARYCODE_DIR_ABS}/JuceHeader.h")
            message(STATUS "Found JuceLibraryCode at: ${JUCELIBRARYCODE_DIR_ABS}")
            set(JUCELIBRARYCODE_DIR "${JUCELIBRARYCODE_DIR_ABS}" CACHE PATH "Path to JuceLibraryCode directory" FORCE)
        else()
            message(STATUS "ENV{JUCELIBRARYCODE_DIR} points to invalid location: ${JUCELIBRARYCODE_DIR_ABS}")
        endif()
    else()
        # Try to infer from parent CMakeLists.txt location
        # If called from Builds/Android/app/CMakeLists.txt, JuceLibraryCode should be at ../../../JuceLibraryCode
        get_filename_component(PARENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../.." ABSOLUTE)
        set(INFERRED_JUCELIBRARYCODE_DIR "${PARENT_DIR}/JuceLibraryCode")
        message(STATUS "Trying inferred JUCELIBRARYCODE_DIR: ${INFERRED_JUCELIBRARYCODE_DIR}")
        if(EXISTS "${INFERRED_JUCELIBRARYCODE_DIR}/JuceHeader.h")
            message(STATUS "Found JuceLibraryCode at inferred path!")
            set(JUCELIBRARYCODE_DIR "${INFERRED_JUCELIBRARYCODE_DIR}" CACHE PATH "Path to JuceLibraryCode directory" FORCE)
        else()
            message(STATUS "Inferred path not found")
        endif()
    endif()
    
    message(STATUS "Step 2 result - JUCELIBRARYCODE_DIR: ${JUCELIBRARYCODE_DIR}")
endif()

# If not found, show error with helpful message
if(NOT JUCELIBRARYCODE_DIR)
    message(FATAL_ERROR
        "JuceLibraryCode directory not found.\n"
        "Please set JUCELIBRARYCODE_DIR to the path containing JuceLibraryCode (the directory that contains 'JuceHeader.h' file), or place JuceLibraryCode in one of:\n"
        "${JUCELIBRARYCODE_RELATIVE_PATHS}\n"
        "Example: cmake -DJUCELIBRARYCODE_DIR=/path/to/JuceLibraryCode ..."
    )
endif()

# Export JUCELIBRARYCODE_DIR for parent CMakeLists.txt if needed
set(JUCELIBRARYCODE_DIR ${JUCELIBRARYCODE_DIR} CACHE PATH "Path to JuceLibraryCode directory")

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
    ${JUCELIBRARYCODE_DIR}
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

