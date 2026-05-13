cmake_minimum_required(VERSION 3.16)

project(borealis LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Bluetooth DBus)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Bluetooth DBus)

include_directories(include)
file(GLOB HEADERS include/**/*.h include/*.h)

add_executable(borealis
  ${HEADERS}

  connection.cpp
  protocol/battery.cpp
  protocol/device_information.cpp
  aacp/protocol.cpp
  aacp/battery.cpp
  aacp/device_information.cpp
  main.cpp
)
target_link_libraries(borealis Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Bluetooth Qt${QT_VERSION_MAJOR}::DBus bluetooth)

include(GNUInstallDirs)
install(TARGETS borealis
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
