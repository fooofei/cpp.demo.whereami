cmake_minimum_required(VERSION 2.6)
project(whereami)

if (APPLE)
    set(CMAKE_MACOSX_RPATH 0)
endif ()


add_library(${PROJECT_NAME} SHARED library.cpp)
target_sources(${PROJECT_NAME} PRIVATE ../whereami.cpp)

if (NOT WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE -fvisibility=hidden)
endif()

# 第二个参数设置上级目录 ${CMAKE_HOME_DIRECTORY}/build_encoding 总是不好
# error : make[1]: *** No rule to make target '../build_encoding/CMakeFiles/encoding.dir/all'
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../cpp_cross_platform_encoding  build_encoding)
target_link_libraries(${PROJECT_NAME} encoding)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../print_chs build_print_chs)
target_link_libraries(${PROJECT_NAME} print_chs)

add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ../../bin
        COMMAND ${CMAKE_COMMAND} -E remove $<TARGET_FILE:${PROJECT_NAME}>
        )
