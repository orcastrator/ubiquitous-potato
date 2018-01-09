cmake_minimum_required( VERSION 3.10.1 )

project( my-rest-server CXX )
set( my-rest-server_VERSION_MAJOR 0 )
set( my-rest-server_VERSION_MINOR 1.0 )
set( my-rest-server_VERSION ${my-rest-server_VERSION_MAJOR}.${my-rest-server_VERSION_MINOR} )
set( PACKAGE_VERSION ${my-rest-server_VERSION} )
set( VERSION "${my-rest-server_VERSION}" )

# CMAKE_SOURCE_DIR the directory which contains the top-level CMakeLists.txt, i.e. the top level source directory
include_directories(SYSTEM ${CMAKE_SOURCE_DIR})

if(NOT EXISTS ${REPO_DIRECTORY})
    message( FATAL_ERROR "repo directory at '${REPO_DIRECTORY}' does not exist")
endif()

# PROJECT_SOURCE_DIR contains the full path to the root of your project source directory, i.e. to the nearest directory where CMakeLists.txt contains the PROJECT() command

#
# Build Options
#
# option( BUILD_SHARED   "Build shared library."              OFF )
# option( BUILD_EXAMPLES "Build examples applications."       OFF )
# option( BUILD_TESTS    "Build all available test suites."   OFF )

# use multithreaded debugging
set(CMAKE_CXX_FLAGS_DEBUG "/MTd")

#
# Build
#
# to find library_dir

foreach(module
  references-lib
  server
)
add_subdirectory(${module})
endforeach()

if ( BUILD_SHARED )
    set_target_properties( ${PROJECT_NAME} PROPERTIES SOVERSION ${my-rest-server_VERSION_MAJOR} VERSION ${my-rest-server_VERSION} )
endif ( )

#
# Install
#
# include( "${PROJECT_SOURCE_DIR}/cmake/artifacts.cmake" )

if ( NOT DEFINED CMAKE_INSTALL_LIBDIR )
    set( CMAKE_INSTALL_LIBDIR library )
endif ( )

# install( FILES "${INCLUDE_DIR}/${PROJECT_NAME}" DESTINATION "${CMAKE_INSTALL_PREFIX}/include" )
# install( FILES ${ARTIFACTS} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/corvusoft/${PROJECT_NAME}" )
# install( TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT library )
