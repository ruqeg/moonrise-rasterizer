cmake_minimum_required(VERSION 3.0)

project(moonrise-engine VERSION 0.0.1 LANGUAGES C)

option(BUILD_SHARED_LIBS "Build shared libraries (.dll/.so) instead of static ones (.lib/.a)" ON)
option(MRE_BUILD_EXAMPLE OFF)

find_package(SDL2 REQUIRED)

include_directories(include)

set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

add_library(moonrise_engine
  ${SRCROOT}/MRE_line.c
  ${SRCROOT}/MRE_system.c
)

install(
  TARGETS moonrise_engine
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

if(MRE_BUILD_EXAMPLE)
  add_subdirectory(example)
endif()

unset(MRE_BUILD_EXAMPLE CACHE)
