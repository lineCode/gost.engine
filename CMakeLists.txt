cmake_minimum_required (VERSION 2.6)
project (gost)

set(SOURCE)

configure_file (
  "${PROJECT_SOURCE_DIR}/TutorialConfig.h.inl"
  "${PROJECT_BINARY_DIR}/TutorialConfig.h"
  )
  
  
add_executable(gost gost.cpp)
