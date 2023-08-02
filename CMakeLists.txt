cmake_minimum_required(VERSION 3.20)

# Not ideal to use this global variable, but necessary to make sure
# that tooling and projects use the same version
set(CMAKE_CXX_STANDARD 20)

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif()

# Set the project name and language
project(
    coverage_notes
    VERSION 0.0.1
    DESCRIPTION ""
    HOMEPAGE_URL "https://github.com/sukolenvo/coverage-notes"
    LANGUAGES CXX C)

set(GIT_SHA
    "Unknown"
    CACHE STRING "SHA this build was generated from")
string(
    SUBSTRING "${GIT_SHA}"
    0
    8
    GIT_SHORT_SHA)

get_property(BUILDING_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if (BUILDING_MULTI_CONFIG)
  if (NOT CMAKE_BUILD_TYPE)
    # Make sure that all supported configuration types have their
    # associated conan packages available. You can reduce this
    # list to only the configuration types you use, but only if one
    # is not forced-set on the command line for VS
    message(TRACE "Setting up multi-config build types")
    set(CMAKE_CONFIGURATION_TYPES
        Debug
        Release
        RelWithDebInfo
        MinSizeRel
        CACHE STRING "Enabled build types" FORCE)
  else ()
    message(TRACE "User chose a specific build type, so we are using that")
    set(CMAKE_CONFIGURATION_TYPES
        ${CMAKE_BUILD_TYPE}
        CACHE STRING "Enabled build types" FORCE)
  endif ()
endif ()

add_subdirectory(vendor/simple-cpp-xml)
add_subdirectory(vendor/CLI11)
add_subdirectory(vendor/fmt)
add_subdirectory(vendor/spdlog)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
add_library(project_warnings INTERFACE)

# Adding the src:
add_subdirectory(src)

enable_testing()
include(CTest)
add_subdirectory(vendor/Catch2)
add_subdirectory(test)

# If MSVC is being used, and ASAN is enabled, we need to set the debugger environment
# so that it behaves well with MSVC's debugger, and we can run the target from visual studio
if (MSVC)
  get_all_installable_targets(all_targets)
  message("all_targets=${all_targets}")
  set_target_properties(${all_targets} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=$(VC_ExecutablePath_x64);%PATH%")
endif ()

# set the startup project for the "play" button in MSVC
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT aoc)
