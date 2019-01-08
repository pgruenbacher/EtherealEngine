add_subdirectory_ex(editor_core)
add_subdirectory_ex(editor_runtime)


link_libraries(stdc++fs)
link_libraries(stdc++experimental)

set(CMAKE_C_COMPILER /usr/bin/gcc-8)
set(CMAKE_CXX_COMPILER /usr/bin/g++-8)