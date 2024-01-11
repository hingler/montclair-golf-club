cmake_minimum_required(VERSION 3.0.0)
project(montclair-golf-club VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_SYSTEM_PROCESSOR arm64)
# i've got libs on x86 and on arm, how to delineate??
set(CMAKE_OSX_ARCHITECTURES arm64)

set(CMAKE_CXX_FLAGS "-g -O3")
set(CMAKE_VERBOSE_MAKEFILE TRUE)

set(INC_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LIB_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(STB_INCLUDES ${LIB_DIR}/stb)
set(GLM_INCLUDES ${LIB_DIR}/glm)

# x86 bindings
set(HOMEBREW_PREFIX "/usr/local")
set(HOMEBREW_INCLUDE_PREFIX ${HOMEBREW_PREFIX}/include)
set(HOMEBREW_LIB_PREFIX ${HOMEBREW_PREFIX}/lib)

add_subdirectory(${LIB_DIR}/googletest)

find_library(TBB_LIB
  NAMES tbb libtbb
  PATHS ${HOMEBREW_LIB_PREFIX}
)

add_library(montclair-golf-club main.cpp
                                src/course/terrain_data.cpp
                                src/course/sampler/MetaballSampler.cpp
                                src/course/sampler/SimplexNoiseSampler.cpp
                                src/course/sampler/MaxFloatSampler.cpp

                                src/course/sampler/grass/SimpleGrassSampler.cpp
                                src/course/sampler/grass/TeeSampler.cpp
                                src/course/sampler/GaussianMetaballSampler.cpp

                                src/course/sampler/gaussian/GaussianFairwaySampler.cpp
                                src/course/sampler/gaussian/GaussianSandSampler.cpp

                                src/course/sampler/terrain/SimpleCourseTerrainSampler.cpp

                                src/course/CourseWriter.cpp
                                src/course/generator/BruteForceCourseGenerator.cpp
                                src/course/generator/impl/seed_tracker_impl.cpp
                                src/course/generator/impl/PositionTracker.cpp
                                src/course/generator/impl/GetDistanceToCircle.cpp
                                src/course/path/fairway_point.cpp
                                src/course/path/BezierCurve.cpp
                                src/course/path/CompoundCurve.cpp
                                src/course/path/CoursePathToCurve.cpp
                                src/course/path/CoursePath.cpp

                                src/image/rgba_color.cpp
                                src/image/TerrainToColorMap.cpp
                                src/image/TerrainImageConverter.cpp
                                src/image/ImageWriter.cpp
                                src/image/filter/GaussianBlur.cpp

                                src/noise/TurbulentDisplace.cpp

                                src/terrain/HillGenerator.cpp

                                src/util/Gaussian.cpp

                                src/mgc/CourseGen.cpp

                                src/support/prepare_stb.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${STB_INCLUDES} ${GLM_INCLUDES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INC_DIR} ${HOMEBREW_INCLUDE_PREFIX})
target_link_libraries(${PROJECT_NAME} PUBLIC ${TBB_LIB})

set(TEST_DIR ${CMAKE_SOURCE_DIR}/test)

set(TEST_PATHS ${TEST_DIR}/TerrainDataTest.cpp 
               ${TEST_DIR}/RGBAColorTest.cpp
               ${TEST_DIR}/GenericImageTest.cpp
               ${TEST_DIR}/MetaballSamplerTest.cpp
               ${TEST_DIR}/InterpolatingImageSamplerTest.cpp
               ${TEST_DIR}/SimplexNoiseSamplerTest.cpp
               ${TEST_DIR}/GetDistanceToCircleTest.cpp
               ${TEST_DIR}/BezierCurveTest.cpp
               ${TEST_DIR}/CompoundCurveTest.cpp)

set(TEST_NAMES TerrainDataTest RGBAColorTest GenericImageTest MetaballSamplerTest
               InterpolatingImageSamplerTest
               SimplexNoiseSamplerTest
               GetDistanceToCircleTest
               BezierCurveTest
               CompoundCurveTest)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  foreach(test_path test_name IN ZIP_LISTS TEST_PATHS TEST_NAMES)
    add_executable(${test_name} ${test_path})
    target_link_libraries(${test_name} PRIVATE GTest::gtest_main ${PROJECT_NAME})
    target_include_directories(${test_name} PUBLIC ${INC_DIR} ${HOMEBREW_INCLUDE_PREFIX}
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
  endforeach()
endif()

# demos
add_executable(terrain-convert-demo demo/terrain_convert_demo.cpp)
target_link_libraries(terrain-convert-demo PUBLIC ${PROJECT_NAME})

add_executable(sampler-write-demo demo/sampler_write_demo.cpp)
target_link_libraries(sampler-write-demo PUBLIC ${PROJECT_NAME})

add_executable(fairway-test demo/fairway_test.cpp)
target_link_libraries(fairway-test PUBLIC ${PROJECT_NAME})

add_executable(gaussian-test demo/gaussian_test.cpp)
target_link_libraries(gaussian-test PUBLIC ${PROJECT_NAME})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
