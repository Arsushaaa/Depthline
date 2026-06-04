cmake_minimum_required(VERSION 3.28)

project(Depthline
   VERSION 0.0.3
   LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

FetchContent_Declare(
   SFML
   GIT_REPOSITORY https://github.com/SFML/SFML.git
   GIT_TAG 3.1.0
   GIT_SHALLOW ON
   EXCLUDE_FROM_ALL
   SYSTEM
)

set(SFML_BUILD_NETWORK OFF)

FetchContent_MakeAvailable(SFML)

find_package(Threads REQUIRED)

add_executable(Depthline
   app/main.cpp
   src/Core/Game.cpp
   src/Entities/Enemy.cpp
   src/Entities/Player.cpp
   src/Entities/Weapon.cpp
   src/Input/PlayerCommand.cpp
   src/World/TileMap.cpp
   src/World/World.cpp
)

target_include_directories(Depthline PRIVATE
   ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Depthline PRIVATE
   SFML::Graphics
   SFML::Audio
   Threads::Threads
)

target_compile_options(Depthline PRIVATE
   -Wall
   -Wextra
   -Wpedantic
)

add_custom_command(TARGET Depthline POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory
         ${CMAKE_CURRENT_SOURCE_DIR}/assets
         $<TARGET_FILE_DIR:Depthline>/assets
)