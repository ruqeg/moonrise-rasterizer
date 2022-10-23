include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(example_exe
  main.c
)

target_link_libraries(example_exe moonrise-rasterizer
  -lSDL2 -lSDL2main -lm -ldl
)
