cmake_minimum_required(VERSION 3.5)
set (CMAKE_CXX_STANDARD 11)

# name of project, will be name of plugin
project(LedServerUtilities)

#message(STATUS "hi")
include_directories(${CMAKE_JS_INC})
# needed to resolve nan.h library
#include_directories(${CMAKE_JS_INC} ../nan)
include_directories(${CMAKE_JS_INC} ./node_modules/node-addon-api)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
file(GLOB SOURCE_FILES "*.cpp" "*.h")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})
