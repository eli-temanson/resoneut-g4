
cmake_minimum_required(VERSION 3.0)
set(PROJECT_NAME resoneut-g4)
project(${PROJECT_NAME})
set(OUTPUT_NAME single_det)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})
include_directories(${CMAKE_CURRENT_SOUREC_DIR}/src)
link_directories(${Geant4_LIBRARY_DIR})

file(GLOB sources src/*.cc)
file(GLOB headers src/*.hh)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor)
add_executable(${OUTPUT_NAME} ${sources} ${headers})

# target_include_directories(RNAnalysis
#     PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
#     SYSTEM PRIVATE ${ROOT_INCLUDE_DIRS}
# )
# target_sources(RNAnalysis PRIVATE
#     src/main.cpp
#     src/detector.cpp
#     src/physics.cpp
#     src/analysis.cpp
#     src/mass_lookup.cpp
#     src/reco_cluster.cpp
# )


target_link_libraries(${OUTPUT_NAME} PRIVATE 
  nlohmann_json::nlohmann_json 
  ${Geant4_LIBRARIES}
)

# add_custom_target(RESONEUT-G4 DEPENDS ${OUTPUT_NAME})

install(TARGETS ${OUTPUT_NAME} DESTINATION build/ )
