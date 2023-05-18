find_package(Catch2 CONFIG REQUIRED)

add_executable(jacoco_tests JacocoParserTests.cpp)
target_link_libraries(jacoco_tests jacoco project_options project_warnings Catch2::Catch2WithMain)

include(Catch)

# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
# to whatever you want, or use different for different binaries
catch_discover_tests(
    jacoco_tests
    TEST_PREFIX
    "jacocotests."
    REPORTER
    XML
    OUTPUT_DIR
    .
    OUTPUT_PREFIX
    "jacocotests."
    OUTPUT_SUFFIX
    .xml)