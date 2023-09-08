# module.cmake
# creates a module in form of a linkable library
# usage:
#
# set(MODULE_NAME FooModule)
# set(MODULE_SOURCE foo.cpp foo.h)
# set(MODULE_INCLUDE ${CMAKE_SOURCE_DIR}/bar)
# set(MODULE_DEFINITIONS MODULE="Foo")
# set(MODULE_LINK bar::bar)
#
# include(.../module.cmake)

message(STATUS "Configuring module: ${MODULE_NAME}")

set(_SKLIB_MODULE_NAME sklib-${MODULE_NAME})

add_library(${_SKLIB_MODULE_NAME})
target_sources(${_SKLIB_MODULE_NAME} PRIVATE ${MODULE_SOURCE})
target_include_directories(${_SKLIB_MODULE_NAME} PUBLIC
    ${_SKLIB_INCLDIR}
    ${CMAKE_BINARY_DIR}
    ${MODULE_INCLUDE})
target_compile_definitions(${_SKLIB_MODULE_NAME} PUBLIC
    MODULE_NAME="${_SKLIB_MODULE_NAME}"
    ${MODULE_DEFINITIONS})
target_link_libraries(${_SKLIB_MODULE_NAME} PUBLIC ${MODULE_LINK})

if(${MODULE_TEST})
    add_executable(${_SKLIB_MODULE_NAME}_test module_test.cpp)
    target_link_libraries(${_SKLIB_MODULE_NAME}_test PUBLIC ${_SKLIB_MODULE_NAME} ${MODULE_LINK})
    target_include_directories(${_SKLIB_MODULE_NAME}_test PUBLIC
        ${_SKLIB_INCLDIR}
        ${CMAKE_BINARY_DIR}
        ${MODULE_INCLUDE})
    target_compile_definitions(${_SKLIB_MODULE_NAME}_test PUBLIC
        ${MODULE_DEFINITIONS})
endif()
