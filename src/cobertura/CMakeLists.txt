add_library(cobertura cobertura_parser.cpp cobertura_coverage.cpp)
target_link_libraries(cobertura core project_options project_warnings simple-cpp-xml::lib spdlog)
target_include_directories(cobertura INTERFACE ..)