file(GLOB_RECURSE libsrc *.h *.cpp *.hpp *.c *.cc)


add_library(editor_lib ${libsrc})
target_link_libraries(editor_lib PUBLIC stdc++fs)
target_link_libraries(editor_lib PUBLIC editor_core)
target_link_libraries(editor_lib PUBLIC runtime)

add_executable (editor ${libsrc})
target_link_libraries(editor PUBLIC stdc++fs)
target_link_libraries(editor PUBLIC editor_core)
target_link_libraries(editor PUBLIC runtime)
target_link_libraries(editor PUBLIC editor_lib)
target_include_directories (editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

if(MINGW)
  set_target_properties(editor PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++ -static")
endif()
