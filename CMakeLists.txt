cmake_minimum_required(VERSION 3.14)
project(capacity_lib VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(capacity STATIC src/capacity.cpp)
target_include_directories(capacity PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

add_library(capacity_shared SHARED src/capacity.cpp)
target_include_directories(capacity_shared PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
set_target_properties(capacity_shared PROPERTIES OUTPUT_NAME capacity)

include(CTest)
if(BUILD_TESTING)
  add_executable(test_capacity tests/test_capacity.cpp)
  target_link_libraries(test_capacity PRIVATE capacity)
  add_test(NAME test_capacity COMMAND test_capacity)

  add_executable(test_c_api tests/test_c_api.c)
  target_link_libraries(test_c_api PRIVATE capacity)
  set_target_properties(test_c_api PROPERTIES LINKER_LANGUAGE CXX)
  add_test(NAME test_c_api COMMAND test_c_api)
endif()

install(TARGETS capacity capacity_shared ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
