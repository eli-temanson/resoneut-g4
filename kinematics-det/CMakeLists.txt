
cmake_minimum_required(VERSION 3.0)
set(PROJECT_NAME resoneut-g4)
project(${PROJECT_NAME})
set(OUTPUT_NAME kinematics-det)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})
include_directories(${CMAKE_CURRENT_SOUREC_DIR}/src)
link_directories(${Geant4_LIBRARY_DIR})

find_package(ROOT REQUIRED)
set(CMAKE_CXX_FLAGS "${ROOT_CXX_FLAGS}")
include(${ROOT_USE_FILE})
include_directories(${ROOT_INCLUDE_DIRS})
link_directories(${ROOT_LIBRARY_DIR})

file(GLOB sources src/*.cc)
file(GLOB headers src/*.hh)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor)
add_executable(${OUTPUT_NAME} ${sources} ${headers})

target_link_libraries(${OUTPUT_NAME} PRIVATE 
  nlohmann_json::nlohmann_json 
  ${Geant4_LIBRARIES}
  ${ROOT_LIBRARIES}
)

install(TARGETS ${OUTPUT_NAME} DESTINATION build/ )


