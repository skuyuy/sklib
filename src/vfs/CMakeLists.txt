set(MODULE_NAME vfs)
set(MODULE_SOURCE node.cpp module.cpp)
set(MODULE_TEST ON)

# add a custom config for vfs which is placed in <root>/res/vfs
if(${MODULE_TEST})
    set(MODULE_DEFINITIONS SK_VFS_CONFIG_PATH="${CMAKE_SOURCE_DIR}/res/vfs")
endif()

include(${CMAKE_SOURCE_DIR}/cmake/module.cmake)
