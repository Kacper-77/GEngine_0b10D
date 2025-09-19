cmake_minimum_required(VERSION 3.10)
project(GameEngine2D VERSION 0.1.0)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Files
include_directories(include)
file(GLOB_RECURSE SOURCES "src/*/*.cpp" "src/main.cpp")

# SDL2 via find_package
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

# SDL2 via sdl2-config
execute_process(COMMAND sdl2-config --cflags OUTPUT_VARIABLE SDL2_CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND sdl2-config --libs OUTPUT_VARIABLE SDL2_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)

# Executable
add_executable(GameEngine2D ${SOURCES})

# Link SDL2
separate_arguments(SDL2_CFLAGS_LIST UNIX_COMMAND "${SDL2_CFLAGS}")
target_compile_options(GameEngine2D PRIVATE ${SDL2_CFLAGS_LIST})
target_link_libraries(GameEngine2D ${SDL2_LIBS} -L/opt/homebrew/lib -lSDL2_image)

# Flags (optional)
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(GameEngine2D PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Info (optional)
message(STATUS "SDL2 include dirs: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2 libraries: ${SDL2_LIBRARIES}")
message(STATUS "SDL2 cflags: ${SDL2_CFLAGS}")
message(STATUS "SDL2 libs: ${SDL2_LIBS}")