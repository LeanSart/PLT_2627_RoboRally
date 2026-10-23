set(prj_target prj)
 
generate_dia_header("${PROJECT_SOURCE_DIR}/Dia/UMLDiagram.dia")
 
# Tous les .cpp du dossier prj (main.cpp inclus)
file(GLOB_RECURSE prj_sources *.cpp)
 
# Exécutable prj : les headers doivent être générés avant la compilation
add_executable(${prj_target} ${prj_sources})
add_dependencies(${prj_target} generate-headers)
 
target_include_directories(${prj_target} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${SFML_INCLUDE_DIR}
  ${jsoncpp_include_dir}
  )
 
target_link_libraries(${prj_target} PUBLIC
  ${SFML_LIBRARIES}
  )
target_link_libraries(${prj_target} PRIVATE
  Threads::Threads
  )
 
target_compile_options(${prj_target} PUBLIC
  -Wall -Wextra
  )
 
if(BUILD_CODE_COVERAGE)
  add_dependencies(code-coverage ${prj_target})
endif()
 
# vim: set sw=2 sts=2 et:
