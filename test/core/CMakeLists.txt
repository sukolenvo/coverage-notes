find_package(Catch2 CONFIG REQUIRED)

add_executable(core_tests core_tests.cpp CoverageInfoTests.cpp)
target_link_libraries(core_tests core project_options project_warnings Catch2::Catch2)