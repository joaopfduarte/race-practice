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

add_executable(race_practice main.cpp
        app/GameApp.cpp
        app/GameApp.h
        app/InputMapper.cpp
        app/InputMapper.h
        app/Time.cpp
        app/Time.h
        domain/types/MathTypes.cpp
        domain/types/MathTypes.h
        domain/car/Physics.h
        domain/car/Car.h
        domain/car/LapeTimer.h
        domain/track/Track.cpp
        domain/track/Track.h
        infrastructure/GIWindow.cpp
        infrastructure/GIWindow.h
        infrastructure/GIRenderer2D.cpp
        infrastructure/GIRenderer2D.h
        infrastructure/Texture.cpp
        infrastructure/Texture.h
        ui/Scene2D.cpp
        ui/Scene2D.h
        ui/Hud.cpp
        ui/Hud.h
)

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