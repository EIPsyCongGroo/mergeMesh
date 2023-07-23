cmake_minimum_required(VERSION 3.24)
project(merge)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
set(VCPKG_TARGET_TRIPLET x64-windows)

find_package(openmesh  REQUIRED)

add_definitions(-D_USE_MATH_DEFINES)
# Add your project files
file(GLOB SRC_FILES *.cpp)
file(GLOB HEADER_FILES *.h *.hpp)
add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENMESH_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENMESH_LIBRARIES})