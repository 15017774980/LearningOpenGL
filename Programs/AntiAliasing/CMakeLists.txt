cmake_minimum_required(VERSION 3.8)
project(AntiAliasing)
option(USE_SOLUTION_FOLDERS ON)
option(GROUP_BY_EXPLORER ON)

file(GLOB_RECURSE ALL_SCRIPT *.h *.cpp)

foreach(fileItem ${ALL_SCRIPT})
    get_filename_component(PARENT_DIR "${fileItem}" DIRECTORY)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")
    set(GROUP "${GROUP}")
    source_group("${GROUP}" FILES "${fileItem}")
endforeach()

add_executable(${PROJECT_NAME} ${ALL_SCRIPT})
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_ROOT_DIR}")

# 包含头文件目录
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${PROJECT_CORE_DIR}/..
    ${PROJECT_LIBRARIES_DIR}
    ${PROJECT_LIBRARIES_DIR}/glad
    ${PROJECT_LIBRARIES_DIR}/glfw
    ${PROJECT_LIBRARIES_DIR}/stb_image
    ${PROJECT_LIBRARIES_DIR}/glm
    ${PROJECT_LIBRARIES_DIR}/assimp
)

# 链接
target_link_libraries(${PROJECT_NAME} PUBLIC
    Core
    glad
    glfw
    stb_image
    glm
    assimp
)