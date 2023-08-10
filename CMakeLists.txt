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
    VERSION 0.1.2
    HOMEPAGE_URL "https://github.com/sukolenvo/coverage-notes"
    LANGUAGES CXX)

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

include(FetchContent)
FetchContent_Declare(
    XML
    GIT_REPOSITORY https://github.com/sukolenvo/simple-cpp-xml.git
    GIT_TAG v0.1
    GIT_SHALLOW TRUE
)
FetchContent_Declare(
    GithubRest
    GIT_REPOSITORY https://github.com/sukolenvo/simple-cpp-github-rest.git
    GIT_TAG v0.2
    GIT_SHALLOW TRUE
)
FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.3.2
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(XML GithubRest CLI11)

add_subdirectory(vendor/fmt)
add_subdirectory(vendor/spdlog)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
if (MSVC)
  target_compile_options(project_options INTERFACE /W4)
  if (COVERAGE_NOTES_WARNINGS_AS_ERRORS)
    target_compile_options(project_options INTERFACE /WX)
  endif ()
else ()
  target_compile_options(project_options INTERFACE -Wall -Wextra -Wpedantic)
  if (COVERAGE_NOTES_WARNINGS_AS_ERRORS)
    target_compile_options(project_options INTERFACE -Werror)
  endif ()
endif ()
add_library(project_warnings INTERFACE)

# Adding the src:
add_subdirectory(src)

enable_testing()
include(CTest)
add_subdirectory(vendor/Catch2)
add_subdirectory(test)
add_test(NAME coverage-notes-sample COMMAND coverage-notes --jacoco ${CMAKE_SOURCE_DIR}/sample.xml print)