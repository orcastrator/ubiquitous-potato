project(niv-server CXX)
include("${PROJECT_SOURCE_DIR}/manifest.cmake")
# to find include_dir
include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/../restbed/install/include")
include_directories(SYSTEM "${BOOST_PATH}")
# include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/../install/include")

add_executable( ${PROJECT_NAME} ${MANIFEST} )

# target_link_libraries( ${PROJECT_NAME} references-lib )
target_link_libraries( ${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/../restbed/install/library/restbed.lib" "${PROJECT_SOURCE_DIR}/../build/references-lib/Debug/references-lib.lib")
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

link_directories("${PROJECT_SOURCE_DIR}/../restbed/install/library")
#
# Install
#
# include( "${PROJECT_SOURCE_DIR}/cmake/artifacts.cmake" )

# install( FILES "${INCLUDE_DIR}/${PROJECT_NAME}" DESTINATION "${CMAKE_INSTALL_PREFIX}/include" )
# install( FILES ${ARTIFACTS} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/corvusoft/${PROJECT_NAME}" )
# install( TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT library )
