add_subdirectory(core)
add_subdirectory(jacoco)
add_subdirectory(cobertura)

add_executable(coverage-notes main.cpp)
target_link_libraries(
    coverage-notes
    PRIVATE core jacoco cobertura project_options project_warnings CLI11::CLI11 spdlog::spdlog simple-cpp-github-rest::lib)
set_target_properties(coverage-notes PROPERTIES OUTPUT_NAME coverage-notes-${CMAKE_PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR})