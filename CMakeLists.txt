cmake_minimum_required(VERSION 3.22)
project(juceture)

# Detect JUCE_MODULES_DIR if not already provided
if(NOT DEFINED JUCE_MODULES_DIR OR NOT EXISTS "${JUCE_MODULES_DIR}")
    if(DEFINED JUCE_DIR AND EXISTS "${JUCE_DIR}/modules")
        set(JUCE_MODULES_DIR "${JUCE_DIR}/modules")
    elseif(EXISTS "${CMAKE_CURRENT_LIST_DIR}/../JUCE/modules")
        set(JUCE_MODULES_DIR "${CMAKE_CURRENT_LIST_DIR}/../JUCE/modules")
    endif()
endif()

add_library(juceture STATIC
    "${CMAKE_CURRENT_LIST_DIR}/juceture/juceture.cpp"
)

target_include_directories(juceture PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}"
    "${JUCE_MODULES_DIR}"
)

target_compile_definitions(juceture PRIVATE
    JUCE_ANDROID=1
    JUCE_ANDROID_API_VERSION=24
    JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
    JUCE_MODULE_AVAILABLE_juce_core=1
    JUCE_MODULE_AVAILABLE_juce_data_structures=1
    JUCE_MODULE_AVAILABLE_juce_events=1
    JUCE_MODULE_AVAILABLE_juce_graphics=1
    JUCE_MODULE_AVAILABLE_juce_gui_basics=1
    $<$<CONFIG:Debug>:DEBUG=1>
    $<$<CONFIG:Debug>:_DEBUG=1>
    $<$<CONFIG:Release>:NDEBUG=1>
)
