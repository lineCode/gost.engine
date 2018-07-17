cmake_minimum_required (VERSION 2.6)
project (gost)

set (GOST_VERSION_MAJOR 0)
set (GOST_VERSION_MINOR 2)

configure_file (
  "${PROJECT_SOURCE_DIR}/TutorialConfig.h.in"
  "${PROJECT_BINARY_DIR}/TutorialConfig.h"
  )
  
  
add_executable(gost gost.cpp)