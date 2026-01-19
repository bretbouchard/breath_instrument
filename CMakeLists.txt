cmake_minimum_required(VERSION 3.26)
project(BreathLead VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find JUCE
set(JUCE_PATH "/Applications/JUCE" CACHE PATH "Path to JUCE")

if(EXISTS "${JUCE_PATH}")
    add_subdirectory("${JUCE_PATH}" "${CMAKE_BINARY_DIR}/juce")
    set(JUCE_FOUND TRUE)
    message(STATUS "Found JUCE at: ${JUCE_PATH}")
else()
    message(FATAL_ERROR "JUCE not found at ${JUCE_PATH}. Please set JUCE_PATH.")
endif()

# BreathLead DSP Library (engine layer - pure DSP, no wrapper)
add_library(BreathLeadDSP INTERFACE)
target_include_directories(BreathLeadDSP INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# JUCE Plugin with ALL 7 REQUIRED FORMATS
if(JUCE_FOUND)
    juce_add_plugin("BreathLead"
        COMPANY_NAME "SchillingerEcosystem"
        BUNDLE_ID "com.schillingerEcosystem.breathlead"
        PLUGIN_IS_A_SYNTH 1
        NEEDS_MIDI_INPUT 1
        # ALL 7 REQUIRED FORMATS per contract
        FORMATS VST3 AU CLAP LV2 Standalone
        LV2URI "http://schillingerEcosystem.com/plugins/breathlead"
    )

    # Fix VST3 parameter automation conflict
    # We never released a VST2 version, so we can safely ignore this warning
    target_compile_definitions(BreathLead PRIVATE
        JUCE_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING=1
    )

    target_sources(BreathLead
        PRIVATE
            # Processor
            include/plugin/BreathLeadProcessor.h
            src/plugin/BreathLeadProcessor.cpp
            # Plugin Factory
            src/plugin/BreathLeadPlugin.cpp
            # Editor
            include/plugin/BreathLeadEditor.h
            src/plugin/BreathLeadEditor.cpp
            # DSP Voice (Pure DSP)
            include/dsp/BreathLeadVoice.h
            src/dsp/BreathLeadVoice.cpp
    )

    # Add include directory for plugin headers
    target_include_directories(BreathLead PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    # Link against BreathLeadDSP engine
    target_link_libraries(BreathLead PRIVATE BreathLeadDSP)

    # Include JUCE modules
    target_link_libraries(BreathLead PRIVATE
        juce::juce_audio_plugin_client
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    )

    # Platform-specific build configurations
    if(WIN32)
        # Windows builds
        target_compile_definitions(BreathLead PRIVATE
            _CRT_SECURE_NO_WARNINGS
        )
    endif()
endif()

# Note: AUv3 format requires iOS-specific build configuration
# See iOS documentation for AUv3 build instructions
