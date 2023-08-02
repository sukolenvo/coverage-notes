add_library(jacoco JacocoParser.cpp JacocoParser.h)
target_link_libraries(jacoco core project_options project_warnings simple-cpp-xml::lib spdlog)
target_include_directories(jacoco INTERFACE ..)