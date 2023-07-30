find_package(spdlog CONFIG REQUIRED)

add_library(jacoco JacocoParser.cpp JacocoParser.h)
target_link_libraries(jacoco core project_options project_warnings simple-cpp-xml::lib spdlog::spdlog)
target_include_directories(jacoco INTERFACE ..)