# CMakeList.txt : CMake project for veeam_test_task, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("veeam_test_task")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME ON) 
SET(BOOST_INCLUDE  "C:/Program Files/Boost")
SET(BOOST_LIB "C:/Program Files/Boost/stage_x64/lib")
ADD_DEFINITIONS("-DBOOST_ALL_NO_LIB")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${BOOST_INCLUDE})
file(GLOB SOURCES *.cpp)
add_executable(veeam_test_task ${SOURCES})
target_link_libraries(veeam_test_task ${BOOST_LIB})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
add_executable(veeam_test_task_boost_tests "veeam_test_task_boost_tests.cpp"  ${SOURCES})
target_link_libraries(veeam_test_task_boost_tests ${BOOST_LIB})



# Add source to this project's executable.




# TODO: Add tests and install targets if needed.
