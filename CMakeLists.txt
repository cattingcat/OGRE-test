project(simple_cpp_proj)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-std=c++11")

# setting up Boost
find_package(Boost 1.54 REQUIRED COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIR})

# setup directory with cmake-scripts for finding moduler OFRE/OIS
set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")

# setup directory with default config-file
set(OGRE_CONFIG_DIR "/usr/local/share/OGRE/")
file(COPY ${OGRE_CONFIG_DIR} DESTINATION "${CMAKE_BINARY_DIR}/resources")

# setting up OGRE / OIS
find_package(OGRE REQUIRED)
find_package(OIS REQUIRED)
include_directories(
	${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	${OGRE_Overlay_INCLUDE_DIRS}
)

# cunsiom log-message
MESSAGE(WARNING "My_Message!!!: " ${OGRE_INCLUDE_DIRS})

file(GLOB PROJ_FILES "./src/**/*.cpp" "./src/**/*.h" "./src/*.cpp" "./src/*.h")
set(SRC_LIST ${PROJ_FILES})
# aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

# link libs
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${OGRE_Overlay_LIBRARIES})
