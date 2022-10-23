cmake_minimum_required(VERSION 3.0)

project(moonrise-engine VERSION 0.0.1 LANGUAGES C)

option(BUILD_SHARED_LIBS "Build shared libraries (.dll/.so) instead of static ones (.lib/.a)" ON)
option(MRE_BUILD_EXAMPLE OFF)

find_package(SDL2 REQUIRED)

include_directories(include)
include_directories(extlibs)

set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

add_library(moonrise-rasterizer
  ${SRCROOT}/MRE_math.c
  ${SRCROOT}/MRE_alg.c
  ${SRCROOT}/MRE_texture.c
  ${SRCROOT}/MRE_bounding_sphere.c
  ${SRCROOT}/MRE_geometry.c
  ${SRCROOT}/MRE_graphics.c
  ${SRCROOT}/MRE.c
)

install(
  TARGETS moonrise-rasterizer
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

if(MRE_BUILD_EXAMPLE)
  add_subdirectory(example)
endif()

unset(MRE_BUILD_EXAMPLE CACHE)
