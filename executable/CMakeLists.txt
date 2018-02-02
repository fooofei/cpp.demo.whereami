cmake_minimum_required(VERSION 2.6)
project(call)

if (APPLE)
    set(CMAKE_MACOSX_RPATH 0)
endif ()

add_executable(${PROJECT_NAME} executable.cpp)

if (NOT WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE -fvisibility=hidden)
endif ()


if (NOT TARGET encoding)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../cpp_cross_platform_encoding build_encoding)
endif ()
target_link_libraries(${PROJECT_NAME} encoding)

if (NOT TARGET print_chs)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../print_chs build_print_chs)
endif ()
target_link_libraries(${PROJECT_NAME} print_chs)

if (NOT TARGET whereami)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../whereami build_whereami)
endif ()
target_link_libraries(${PROJECT_NAME} whereami)

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
