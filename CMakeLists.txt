cmake_minimum_required(VERSION 3.14)
project(timeR)

set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXECUTABLE_ENABLE_EXPORTS ON)

exec_program(${CMAKE_SOURCE_DIR}/blobber ARGS ${CMAKE_SOURCE_DIR}/res)
exec_program(mv ARGS ${CMAKE_SOURCE_DIR}/res.blob ${CMAKE_BINARY_DIR}/)
add_custom_command(OUTPUT res.o COMMAND ld -r -b binary -o res.o res.blob)

file(GLOB src "src/*.cpp")
add_executable(${PROJECT_NAME} ${src} res.o)
target_include_directories(${PROJECT_NAME} PUBLIC "src" "raylib/include")
target_link_libraries(${PROJECT_NAME}
                      PUBLIC ${CMAKE_SOURCE_DIR}/raylib/linux/libraylib.so)
target_compile_options(${PROJECT_NAME} PUBLIC -O0 -fPIC)

add_custom_command(
  TARGET ${PROJECT_NAME}
  PRE_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/build/compile_commands.json
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/../compile_commands.json)
