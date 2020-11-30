cmake_minimum_required(VERSION 3.11)
set (PROJECT_NAME "main")
project (${PROJECT_NAME})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
include(glfw)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c11 -g -O0")
set (CMAKE_BUILD_TYPE "Debug")
set (source_path ${PROJECT_SOURCE_DIR})

set (buffer_object_dir ${source_path}/buffer_object)
set (create_window_dir ${source_path}/create_window)
set (model_dir ${source_path}/model)
set (shader_obj_dir ${source_path}/shader_obj)
set (include_path ${source_path}/include)

include_directories(
    ${source_path}/include,
    ${buffer_object_dir},
    ${create_window_dir},
    ${model_dir},
    ${shader_obj_dir},
)

add_library (glad "${source_path}/glad/glad.c" "${source_path}/glad/glad.h")
add_library (buffer_object "${buffer_object_dir}/buffer_object.c" "${buffer_object_dir}/buffer_object.h")
add_library (window "${create_window_dir}/window.c" "${create_window_dir}/window.h")
add_library (model_obj "${model_dir}/model_obj.c" "${model_dir}/model_obj.h")
add_library (read_model_file "${model_dir}/read_model_file.c" "${model_dir}/read_model_file.h")
add_library (shader_obj "${shader_obj_dir}/shader.c" "${shader_obj_dir}/shader.h")

FILE(GLOB SC_FILES "*.c" "*.h")
add_executable (${PROJECT_NAME} ${SC_FILES})

target_link_libraries (${PROJECT_NAME} model_obj read_model_file glfw glad buffer_object window shader_obj)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${source_path}/shader_code
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/shader_code
)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${source_path}/model_file
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/model_file
)

##############################################################

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${source_path}/shader_code
    ${CMAKE_SOURCE_DIR}/build/shader_code
)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${source_path}/model_file
    ${CMAKE_SOURCE_DIR}/build/model_file
)