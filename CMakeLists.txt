cmake_minimum_required(VERSION 3.0.0)
project(montclair-golf-club VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-g -O2")

set(INC_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LIB_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(STB_INCLUDES ${LIB_DIR}/stb)

find_package(gtest REQUIRED)

add_library(montclair-golf-club main.cpp
                                src/course/terrain_data.cpp
                                src/course/sampler/MetaballSampler.cpp
                                src/image/rgba_color.cpp
                                src/image/TerrainToColorMap.cpp
                                src/image/TerrainImageConverter.cpp
                                src/image/ImageWriter.cpp
                                src/support/prepare_stb.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${STB_INCLUDES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INC_DIR})

set(TEST_DIR ${CMAKE_SOURCE_DIR}/test)

set(TEST_PATHS ${TEST_DIR}/TerrainDataTest.cpp 
               ${TEST_DIR}/RGBAColorTest.cpp
               ${TEST_DIR}/GenericImageTest.cpp
               ${TEST_DIR}/MetaballSamplerTest.cpp)

set(TEST_NAMES TerrainDataTest RGBAColorTest GenericImageTest MetaballSamplerTest)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  foreach(test_path test_name IN ZIP_LISTS TEST_PATHS TEST_NAMES)
    add_executable(${test_name} ${test_path})
    target_link_libraries(${test_name} PRIVATE GTest::gtest_main ${PROJECT_NAME})
    target_include_directories(${test_name} PUBLIC ${INC_DIR}
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
  endforeach()
endif()

# demos
add_executable(terrain-convert-demo demo/terrain_convert_demo.cpp)
target_link_libraries(terrain-convert-demo PUBLIC ${PROJECT_NAME})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
