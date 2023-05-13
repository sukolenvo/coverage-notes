find_package(Catch2 CONFIG REQUIRED)

add_executable(jacoco_tests jacoco_tests.cpp JacocoParserTests.cpp)
target_link_libraries(jacoco_tests jacoco project_options project_warnings Catch2::Catch2)