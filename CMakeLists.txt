cmake_minimum_required(VERSION 3.16)

project(radar VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5
    REQUIRED COMPONENTS Quick Core Widgets
)

qt_add_executable(appradar
    main.cpp
    DataModel.hpp
    DataModel.cpp
)

qt_add_qml_module(appradar
    URI radar
    VERSION 1.0
    QML_FILES Main.qml Radar.qml
)

set_target_properties(appradar PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appradar
    PRIVATE Qt6::Quick Qt6::Core Qt6::Widgets
)

install(TARGETS appradar
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
