#
# Build
#
# to find library_dir
project(references-lib CXX)

include("${PROJECT_SOURCE_DIR}/manifest.cmake")
# to find include_dir
include_directories(SYSTEM "${REPO_DIRECTORY}/boost/boost_1_55_0")
include_directories(SYSTEM "${REPO_DIRECTORY}/ubiquitous-potato/references-lib")

add_library( ${PROJECT_NAME} ${MANIFEST} )
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

#
# Install
#
# include( "${PROJECT_SOURCE_DIR}/cmake/artifacts.cmake" )

# if ( NOT DEFINED CMAKE_INSTALL_LIBDIR )
    # set( CMAKE_INSTALL_LIBDIR library )
# endif ( )

# install( FILES "${INCLUDE_DIR}/${PROJECT_NAME}" DESTINATION "${CMAKE_INSTALL_PREFIX}/include" )
# install( FILES ${ARTIFACTS} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/corvusoft/${PROJECT_NAME}" )
# install( TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT library )
