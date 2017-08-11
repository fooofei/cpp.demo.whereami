cmake_minimum_required(VERSION 2.6)
project(whereami)

if (APPLE)
    set(CMAKE_MACOSX_RPATH 0)
endif ()

set(SOURCE_FILES
        library.cpp
        ../whereami.cpp
        )


add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

if (NOT WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE -fvisibility=hidden)
endif()

# 第二个参数设置上级目录 ${CMAKE_HOME_DIRECTORY}/build_encoding 总是不好
# error : make[1]: *** No rule to make target '../build_encoding/CMakeFiles/encoding.dir/all'
add_subdirectory(${CMAKE_HOME_DIRECTORY}/../../encoding  build_encoding)
target_link_libraries(${PROJECT_NAME} encoding)


add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ../
        COMMAND ${CMAKE_COMMAND} -E remove $<TARGET_FILE:${PROJECT_NAME}>
        )
