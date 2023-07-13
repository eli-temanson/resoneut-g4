
cmake_minimum_required(VERSION 3.0)
project(single_det)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(OUTPUT_NAME single_det)
set(CMAKE_BUILD_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})
include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB sources ${CMAKE_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${CMAKE_SOURCE_DIR}/src/*.hh)

# file(GLOB MACRO_FILES "macros/*.mac")
# file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})
# file(GLOB DATA_FILES "data/*.dat")
# file(COPY ${DATA_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_executable(${OUTPUT_NAME} ${sources} ${headers})
target_link_libraries(${OUTPUT_NAME} ${Geant4_LIBRARIES})
add_custom_target(RESONEUT-G4 DEPENDS ${OUTPUT_NAME})

# install(TARGETS ${OUTPUT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/../ )