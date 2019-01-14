include(GoogleTest)


# Enable ExternalProject CMake module
include(ExternalProject)

# Download and install GoogleTest
ExternalProject_Add(
    gtest
    URL https://github.com/google/googletest/archive/master.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    # Disable install step
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(gtest source_dir binary_dir)

# Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest )

# Set libgtest properties
set_target_properties(libgtest PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/lib/libgtest.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)



file(GLOB_RECURSE SRCS *.cpp)

ADD_EXECUTABLE(unit_tests ${SRCS})

gtest_discover_tests(unit_tests)

# target_compile_features(unit_tests
#     PUBLIC
#     cxx_std_17
# )


# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# target_include_directories(unit_tests PUBLIC "${source_dir}/googletest/include")


# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
target_include_directories(unit_tests PUBLIC "${source_dir}/googletest/include"
                    "${source_dir}/googlemock/include")

add_custom_target(check ${CMAKE_COMMAND} -E env CTEST_OUTPUT_ON_FAILURE=1
                  ${CMAKE_CTEST_COMMAND} -C $<CONFIG> --verbose
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

target_link_libraries(unit_tests runtime editor_core editor_lib)
target_link_libraries(unit_tests libgtest)
