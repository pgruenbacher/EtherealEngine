

######################################################################
# add external project, google/benchmark

include(ExternalProject)
ExternalProject_Add(
  Gbm
  URL "https://github.com/google/benchmark/archive/v1.2.0.tar.gz"
  URL_MD5 "48d0b090cd7a84af2c4a28c8dc963c74"
  CMAKE_ARGS -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX:STRING=<INSTALL_DIR>
)

ExternalProject_Get_Property(Gbm source_dir)

find_package(Threads REQUIRED)


############################################3
file(GLOB_RECURSE SRCS *.cpp)

ADD_EXECUTABLE(Benchmark ${SRCS})

target_compile_features(Benchmark
        PRIVATE
        cxx_std_17
)

target_include_directories(
        Benchmark SYSTEM
        PRIVATE ${source_dir}/include
)

add_custom_target(bench Benchmark
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
######################################################################
# hitch gbm to test)

# add_dependencies(Benchmark Gbm)
ExternalProject_Get_Property(Gbm binary_dir)

target_link_libraries(Benchmark runtime editor_core)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  target_link_libraries(
    Benchmark
    debug ${binary_dir}/src/Debug/${CMAKE_FIND_LIBRARY_PREFIXES}benchmark${CMAKE_FIND_LIBRARY_SUFFIXES}
    optimized ${binary_dir}/src/Release/${CMAKE_FIND_LIBRARY_PREFIXES}benchmark${CMAKE_FIND_LIBRARY_SUFFIXES}
  )
else ()
  target_link_libraries(
    Benchmark
    general ${binary_dir}/src/${CMAKE_FIND_LIBRARY_PREFIXES}benchmark.a
    general pthread
  )
endif ()
