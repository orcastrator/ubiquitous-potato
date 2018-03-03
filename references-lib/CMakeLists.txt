#
# Build
#
# to find library_dir
project(references-lib CXX)

include("${PROJECT_SOURCE_DIR}/manifest.cmake")
# to find include_dir
include_directories(SYSTEM "${BOOST_PATH}")
include_directories(SYSTEM "${PROJECT_SOURCE_DIR}")

add_library( ${PROJECT_NAME} ${MANIFEST} )
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

#
# Install
#

# if ( NOT DEFINED CMAKE_INSTALL_LIBDIR )
    # set( CMAKE_INSTALL_LIBDIR "../install/library" )
# endif ( )

# install( FILES "${INCLUDE_DIR}/${PROJECT_NAME}" DESTINATION "${CMAKE_INSTALL_PREFIX}/include" )
# install( FILES ${ARTIFACTS} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/corvusoft/${PROJECT_NAME}" )
# install( TARGETS ${PROJECT_NAME}
        # LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        # ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        # COMPONENT library )

install(TARGETS references-lib ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")

target_link_libraries(${PROJECT_NAME})

# Expose B's public includes (including Boost transitively) to other
# subprojects through cache variable.
# set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
                                 # ${Boost_INCLUDE_DIRS}
    # CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
