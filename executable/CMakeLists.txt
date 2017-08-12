cmake_minimum_required(VERSION 2.6)
project(call)

if (APPLE)
    set(CMAKE_MACOSX_RPATH 0)
endif ()

add_executable(${PROJECT_NAME} executable.cpp)
target_sources(${PROJECT_NAME} PRIVATE ../whereami.cpp)

if (NOT WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE -fvisibility=hidden)
endif ()


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../cpp_cross_platform_encoding build_encoding)
target_link_libraries(${PROJECT_NAME} encoding)

if (NOT WIN32)
	# for dlopen
    target_link_libraries(${PROJECT_NAME} dl)
endif ()

add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        # Cann't use mv, it not work on Windows.
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ../../bin
        # COMMAND ${CMAKE_COMMAND} -E remove $<TARGET_FILE:${PROJECT_NAME}>
        )
