add_library(jacoco jacoco_parser.cpp jacoco_coverage.cpp)
target_link_libraries(jacoco core project_options project_warnings simple-cpp-xml::lib spdlog)
target_include_directories(jacoco INTERFACE ..)