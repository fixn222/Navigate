cmake_minimum_required(VERSION 3.21)
project(navigate LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 Test)

file(GLOB navigate_sources CONFIGURE_DEPENDS src/*.cpp src/*.h)
list(REMOVE_ITEM navigate_sources ${CMAKE_SOURCE_DIR}/src/main.cpp)
add_library(navigate_core STATIC ${navigate_sources})
target_include_directories(navigate_core PUBLIC src)
target_link_libraries(navigate_core PUBLIC Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick Qt6::QuickControls2)

add_executable(navigate src/main.cpp src/resources.qrc)
target_link_libraries(navigate PRIVATE navigate_core)

add_executable(navigate_tests tests/navigate_tests.cpp)
target_link_libraries(navigate_tests PRIVATE navigate_core Qt6::Test)

install(TARGETS navigate RUNTIME DESTINATION bin)
install(FILES pkgbuild/navigate.desktop DESTINATION share/applications)
install(FILES pkgbuild/navigate.svg DESTINATION share/icons/hicolor/scalable/apps)
