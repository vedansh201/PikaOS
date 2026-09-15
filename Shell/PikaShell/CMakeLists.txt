cmake_minimum_required(VERSION 3.16)

project(PikaShell LANGUAGES CXX)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(PikaShell
    src/main.cpp

    src/Core/PikaWindow.cpp
    src/Core/Desktop.cpp
    src/Core/PikaController.cpp
    src/Core/PikaCommandParser.cpp

    src/Panel/Panel.cpp
    src/FileManager/PikaFileManager.cpp

    src/Terminal/PikaTerminal.cpp
    src/Core/PikaApplicationManager.cpp

    src/Widgets/PikachuWidget.cpp

    include/Core/PikaController.hpp
    include/Core/PikaCommandParser.hpp
    include/Terminal/PikaTerminal.hpp
    include/Widgets/PikachuWidget.hpp
    include/FileManager/PikaFileManager.hpp
    include/Core/PikaApplicationManager.hpp
)

qt_add_resources(PikaShell Resources
    FILES
        assets/pikachu.png
)

target_include_directories(PikaShell PRIVATE include)

find_package(qtermwidget6 REQUIRED)

target_link_libraries(PikaShell PRIVATE
    Qt6::Widgets
    qtermwidget6
)
