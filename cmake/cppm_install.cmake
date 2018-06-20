
set(default_project_dir share/${PROJECT_NAME}/default_project)
install(DIRECTORY DESTINATION ${default_project_dir})
install(DIRECTORY DESTINATION ${default_project_dir}/include)
install(DIRECTORY DESTINATION ${default_project_dir}/src)
install(DIRECTORY DESTINATION ${default_project_dir}/thirdparty)
install(DIRECTORY DESTINATION ${default_project_dir}/test)
install(DIRECTORY DESTINATION ${default_project_dir}/bin)
install(DIRECTORY DESTINATION ${default_project_dir}/cmake/Modules)
install(FILES ${CMAKE_MODULE_PATH}/utils.cmake DESTINATION ${default_project_dir}/cmake)
install(FILES ${CMAKE_MODULE_PATH}/cmake_option.cmake DESTINATION ${default_project_dir}/cmake)
install(FILES ${CMAKE_MODULE_PATH}/project_maker.cmake DESTINATION ${default_project_dir}/cmake)

install(DIRECTORY FindThirdparty DESTINATION share/${PROJECT_NAME})