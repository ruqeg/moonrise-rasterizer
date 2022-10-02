include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(example_exe
  main.c
)

target_link_libraries(example_exe moonrise_engine
  -lSDL2 -lSDL2main -lSDL2_image -lm -ldl
)
