cmake_minimum_required(VERSION 3.15)

project(GetPdbId)

add_executable(${PROJECT_NAME})

set(DIA_SDK_DIR "$ENV{VSINSTALLDIR}DIA SDK")

target_include_directories(${PROJECT_NAME} PRIVATE "${DIA_SDK_DIR}/include")

target_link_directories(${PROJECT_NAME} PRIVATE "${DIA_SDK_DIR}/lib/amd64")

target_link_libraries(${PROJECT_NAME} Ole32.lib)
target_link_libraries(${PROJECT_NAME} diaguids.lib)

target_sources(${PROJECT_NAME} PRIVATE RegistryTools.h RegistryTools.cpp GetPdbId.cpp)