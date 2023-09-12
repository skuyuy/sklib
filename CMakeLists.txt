cmake_minimum_required(VERSION 3.20.0)

project(sklib VERSION 1.0.0 LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(_SKLIB_INCLDIR ${CMAKE_SOURCE_DIR}/include)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(_SK_PLATFORM_WIN ON)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(_SK_PLATFORM_DARWIN ON)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(_SK_PLATFORM_LINUX ON)
endif()


add_subdirectory(deps)
add_subdirectory(generated)
add_subdirectory(src)
