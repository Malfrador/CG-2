cmake_minimum_required(VERSION 3.5)

set(projectname "Uebung02Vorlage")

#Ab hier normalerweise nichts zu ändern!
project(${projectname})

if(NOT DEFINED ${common_included})
    find_path(where_is_common NAMES "common.cmake" 
    PATHS "${CMAKE_SOURCE_DIR}" 
    "${CMAKE_SOURCE_DIR}/.." 
    "${CMAKE_SOURCE_DIR}/../.."
    "${CMAKE_SOURCE_DIR}/../../../")
    include("${where_is_common}/common.cmake")
endif()
setup_project(${projectname})