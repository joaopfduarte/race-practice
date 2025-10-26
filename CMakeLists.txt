cmake_minimum_required(VERSION 3.16)
project(race_practice)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

# Ajuste estes caminhos conforme seu ambiente
include_directories("C:/msys64/mingw64/include")
link_directories("C:/msys64/mingw64/lib")

file(GLOB_RECURSE SRC_FILES
        "src/*.cpp"
)

add_executable(race_practice main.cpp)

target_include_directories(race_practice PRIVATE src)

target_link_libraries(race_practice PRIVATE
        OpenGL::GL
        freeglut
        opengl32
        glu32
        winmm
        gdi32
        user32
)