file(GLOB source *.cpp)

add_library(core ${source})
target_link_libraries(core PRIVATE project_options project_warnings fmt::fmt spdlog::spdlog_header_only)
target_include_directories(core INTERFACE ..)