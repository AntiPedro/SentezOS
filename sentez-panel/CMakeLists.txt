cmake_minimum_required(VERSION 3.16)
project(SentezOS-Panel VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Bağımlılıklar: Qt6 ve LayerShellQt
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
find_package(LayerShellQt REQUIRED)

add_executable(sentez-panel
    src/main.cpp
    src/Taskbar.cpp
    src/Taskbar.h
    src/WindowButton.cpp
    src/WindowButton.h
)

target_link_libraries(sentez-panel PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    LayerShellQt::LayerShellQt
)
