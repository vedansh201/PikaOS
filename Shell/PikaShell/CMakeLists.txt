cmake_minimum_required(VERSION 3.16)

project(PikaShell)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(PikaShell
    src/main.cpp
    src/Core/PikaWindow.cpp
    src/Core/Desktop.cpp
    src/Panel/Panel.cpp
    src/Widgets/PikachuWidget.cpp
)

qt_add_resources(PikaShell "Resources"
    PREFIX "/"
    FILES
        assets/pikachu.png
)

target_include_directories(PikaShell PRIVATE include)

target_link_libraries(PikaShell PRIVATE Qt6::Widgets)