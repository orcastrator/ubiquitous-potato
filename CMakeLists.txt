cmake_minimum_required( VERSION 3.10.1 )

project( server CXX )
set( server_VERSION_MAJOR 0 )
set( server_VERSION_MINOR 1.0 )
set( server_VERSION ${server_VERSION_MAJOR}.${server_VERSION_MINOR} )
set( PACKAGE_VERSION ${server_VERSION} )
set( VERSION "${server_VERSION}" )

include(GNUInstallDirs)

# CMAKE_SOURCE_DIR the directory which contains the top-level CMakeLists.txt, i.e. the top level source directory
include_directories(SYSTEM ${CMAKE_SOURCE_DIR})

if(NOT EXISTS ${BOOST_PATH})
    message( FATAL_ERROR "Unable to find boost libraries at: '${BOOST_PATH}'")
endif()

# PROJECT_SOURCE_DIR contains the full path to the root of your project source directory, i.e. to the nearest directory where CMakeLists.txt contains the PROJECT() command

#
# Build Options
#
# option( BUILD_SHARED   "Build shared library."              OFF )
# option( BUILD_EXAMPLES "Build examples applications."       OFF )
# option( BUILD_TESTS    "Build all available test suites."   OFF )


message("CMAKE BUILD TYPE: ${CMAKE_BUILD_TYPE}")
if(MSVC)
  # use multithreaded debugging
  set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

#
# Build
#
# to find library_dir

foreach(module
  references-lib
  niv-server
  autobackup-service
)
add_subdirectory(${module})
endforeach()

if ( BUILD_SHARED )
    set_target_properties( ${PROJECT_NAME} PROPERTIES SOVERSION ${server_VERSION_MAJOR} VERSION ${server_VERSION} )
endif ( )

#
# Install
#
# include( "${PROJECT_SOURCE_DIR}/cmake/artifacts.cmake" )

if ( NOT DEFINED CMAKE_INSTALL_LIBDIR )
    set( CMAKE_INSTALL_LIBDIR library )
endif ( )
