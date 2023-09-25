cmake_minimum_required(VERSION 3.14)
project(timeR)

set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXECUTABLE_ENABLE_EXPORTS ON)

add_subdirectory("./3rdparty/allegro/")

file(GLOB src "src/*.cpp")

set(allegro5_includes
    "3rdparty/allegro/include"
    "3rdparty/allegro/addons/audio"
    "3rdparty/allegro/addons/acodec"
    "build/3rdparty/allegro/include")   
set(allegro5_libs
    allegro allegro_main 
    allegro_audio
    allegro_acodec)

add_executable(${PROJECT_NAME} ${src})
target_include_directories(${PROJECT_NAME} PUBLIC
    "src"
    "3rdparty"
    ${allegro5_includes})
target_link_libraries(${PROJECT_NAME} PUBLIC
    ${allegro5_libs})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/build/compile_commands.json 
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/../compile_commands.json)
