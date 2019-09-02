cmake_minimum_required(VERSION 3.10)
project(3d-viewer VERSION 0.1.0 LANGUAGES CXX)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

add_executable(viewer
  main.cpp
  viewer.hpp
  viewer.cpp
)

target_compile_features(viewer
  PRIVATE
    cxx_std_17
)

target_include_directories(viewer
  PRIVATE
    .
)

target_link_libraries(viewer
  PRIVATE
    Qt5::Widgets
    OpenGL::GL
    glm
)

set_target_properties(viewer
  PROPERTIES
    AUTOMOC TRUE
)