cmake_minimum_required(VERSION 3.10)
project(GameEngine2D VERSION 0.1.0)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDL2 via find_package
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# SDL2 via sdl2-config
execute_process(COMMAND sdl2-config --cflags OUTPUT_VARIABLE SDL2_CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND sdl2-config --libs OUTPUT_VARIABLE SDL2_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)
separate_arguments(SDL2_CFLAGS_LIST UNIX_COMMAND "${SDL2_CFLAGS}")

# Include and link directories
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

# Engine as a lib
file(GLOB_RECURSE ENGINE_SOURCES "src/*/*.cpp")
add_library(GameEngineLib STATIC ${ENGINE_SOURCES})
target_include_directories(GameEngineLib PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_options(GameEngineLib PRIVATE ${SDL2_CFLAGS_LIST})
target_link_libraries(GameEngineLib ${SDL2_LIBS} -lSDL2_image)

# Executable
add_executable(GameEngine2D src/main.cpp)
target_include_directories(GameEngine2D PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/include/core)
target_compile_options(GameEngine2D PRIVATE ${SDL2_CFLAGS_LIST})
target_link_libraries(GameEngine2D GameEngineLib ${SDL2_LIBS} -lSDL2_image)

# Compiler flags
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(GameEngine2D PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Unit tests
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)
enable_testing()

# MOVEMENT
add_executable(MovementTest tests/test_MovementSystem.cpp)
target_include_directories(MovementTest PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/include/core)
target_link_libraries(MovementTest GameEngineLib gtest_main)
add_test(NAME MovementTest COMMAND MovementTest)

# COMPONENT STORAGE
add_executable(ComponentStorageTest tests/test_ComponentStorage.cpp)
target_include_directories(ComponentStorageTest PRIVATE ${CMAKE_SOURCE_DIR}/include/core)
target_link_libraries(ComponentStorageTest GameEngineLib gtest_main)
add_test(NAME ComponentStorageTest COMMAND ComponentStorageTest)

#SYSTEM MANAGER
add_executable(SystemManagerTest tests/test_SystemManager.cpp)
target_include_directories(SystemManagerTest PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(SystemManagerTest GameEngineLib gtest_main)
add_test(NAME SystemManagerTest COMMAND SystemManagerTest)

# Info
message(STATUS "SDL2 include dirs: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2 libraries: ${SDL2_LIBRARIES}")
message(STATUS "SDL2 cflags: ${SDL2_CFLAGS}")
message(STATUS "SDL2 libs: ${SDL2_LIBS}")
