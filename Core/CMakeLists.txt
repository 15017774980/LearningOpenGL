cmake_minimum_required(VERSION 3.8)
project(Core)

file(GLOB_RECURSE HEADER_FILES *.h)
file(GLOB_RECURSE SOURCE_FILES *.cpp)
set(AllFile ${HEADER_FILES} ${SOURCE_FILES})

foreach(fileItem ${AllFile})
    get_filename_component(PARENT_DIR "${fileItem}" DIRECTORY)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")
    set(GROUP "${GROUP}")
    source_group("${GROUP}" FILES "${fileItem}")
endforeach()

add_library(${PROJECT_NAME} STATIC ${AllFile})
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BUILD_DIR}/Binary)
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BUILD_DIR}/Binary)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_LIBRARIES_DIR}/glad
    ${PROJECT_LIBRARIES_DIR}/glfw
    ${PROJECT_LIBRARIES_DIR}/stb_image
    ${PROJECT_LIBRARIES_DIR}/glm
    ${PROJECT_LIBRARIES_DIR}/assimp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    glad
    glfw
    stb_image
    glm
    assimp
)