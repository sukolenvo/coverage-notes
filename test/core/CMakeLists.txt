find_package(Catch2 CONFIG REQUIRED)

add_executable(core_tests core_tests.cpp CoverageInfoTests.cpp)
target_link_libraries(core_tests core project_options project_warnings Catch2::Catch2)

include(Catch)

# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
# to whatever you want, or use different for different binaries
catch_discover_tests(
    core_tests
    TEST_PREFIX
    "coretests."
    REPORTER
    xml
    OUTPUT_DIR
    .
    OUTPUT_PREFIX
    "coretests."
    OUTPUT_SUFFIX
    .xml)