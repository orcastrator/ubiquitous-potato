project(server CXX)
include("${PROJECT_SOURCE_DIR}/manifest.cmake")
# to find include_dir
include_directories(SYSTEM "${REPO_DIRECTORY}/restbed/install/include")
include_directories(SYSTEM "${REPO_DIRECTORY}/boost/boost_1_55_0")

add_executable( ${PROJECT_NAME} ${MANIFEST} )

target_link_libraries( ${PROJECT_NAME} "${REPO_DIRECTORY}/restbed/install/library/restbed.lib" "${REPO_DIRECTORY}/ubiquitous-potato/build/references-lib/Debug/references-lib.lib")
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

#
# Install
#
# include( "${PROJECT_SOURCE_DIR}/cmake/artifacts.cmake" )

# install( FILES "${INCLUDE_DIR}/${PROJECT_NAME}" DESTINATION "${CMAKE_INSTALL_PREFIX}/include" )
# install( FILES ${ARTIFACTS} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/corvusoft/${PROJECT_NAME}" )
# install( TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT library )
