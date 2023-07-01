cmake_minimum_required(VERSION 3.8)
project(EngineClient)

set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJECT_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Build)
set(PROJECT_CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Core)
set(PROJECT_LIBRARIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Libraries)
set(PROJECT_LIS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Libs)

list(APPEND allSubDir "Libraries")
list(APPEND allSubDir "Core")
list(APPEND allSubDir "Programs")

foreach(dirname ${allSubDir})
    add_subdirectory(${dirname})
endforeach()

set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Other")