include_directories(${PROJECT_SOURCE_DIR}/include)

if(MRE_BUILD_EXAMPLE_ROTATING_CUBE)
add_executable(example_exe example_rotating_cube.c)
else()
add_executable(example_exe example_textured_cube.c)
endif()

target_link_libraries(example_exe moonrise-rasterizer
  -lSDL2 -lSDL2main -lm -ldl
)
