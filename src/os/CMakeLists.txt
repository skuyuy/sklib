set(MODULE_NAME os)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(_PLATFORM_SOURCES win.cpp)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(_PLATFORM_SOURCES macos.cpp)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(_PLATFORM_SOURCES linux.cpp)
endif()

set(MODULE_SOURCE ${_PLATFORM_SOURCES})
set(MODULE_LINK sklib-string)
# todo MODULE_DEFINITIONS to platform depending on platform type
set(MODULE_TEST ON)

include(${CMAKE_SOURCE_DIR}/cmake/module.cmake)
