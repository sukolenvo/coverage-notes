find_package(CLI11 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_subdirectory(core)
add_subdirectory(jacoco)

add_executable(coverage-notes main.cpp)
target_link_libraries(
    coverage-notes
    PUBLIC core jacoco project_options project_warnings CLI11::CLI11 spdlog::spdlog)

