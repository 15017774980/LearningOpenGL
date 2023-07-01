cmake_minimum_required(VERSION 3.8)
project(Programs)

list(APPEND LibrariesAllSubDir "MyOpenGL")
list(APPEND LibrariesAllSubDir "Light")
list(APPEND LibrariesAllSubDir "Model")
list(APPEND LibrariesAllSubDir "DepthTest")
list(APPEND LibrariesAllSubDir "StencilTest")
list(APPEND LibrariesAllSubDir "Blending")
list(APPEND LibrariesAllSubDir "FaceCulling")

foreach(dirname ${LibrariesAllSubDir})
    add_subdirectory(${dirname})
endforeach()