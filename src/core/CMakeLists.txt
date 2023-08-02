add_library(core CoverageInfo.cpp CoverageInfo.h)
target_link_libraries(core project_options project_warnings fmt::fmt)
target_include_directories(core INTERFACE ..)