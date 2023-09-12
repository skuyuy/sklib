include(CMakePackageConfigHelpers)

install(TARGETS sklib-os sklib-string sklib-vfs
        EXPORT sklibTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# headers will be installed globally, see install/CMakeLists.txt

install(EXPORT sklibTargets
        FILE sklibTargets.cmake
        NAMESPACE sklib::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sklib)

# install the whole include directory to cover all libraries
install(DIRECTORY ${_SKLIB_INCLDIR}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.hpp")

configure_package_config_file(Config.cmake.in
                              ${CMAKE_CURRENT_BINARY_DIR}/sklibConfig.cmake
                              INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sklib)
write_basic_package_version_file(sklibConfigVersion.cmake
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY AnyNewerVersion ARCH_INDEPENDENT)

install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/sklibConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/sklibConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sklib)
