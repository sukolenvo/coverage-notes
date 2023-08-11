file(GLOB source *.cpp)

add_library(core ${source})
target_link_libraries(core PRIVATE project_options project_warnings spdlog::spdlog)
target_include_directories(core INTERFACE ..)