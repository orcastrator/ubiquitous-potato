project(autobackup-service CXX)
include("${PROJECT_SOURCE_DIR}/manifest.cmake")
# to find include_dir
include_directories(SYSTEM "${BOOST_PATH}")

if(MSVC)
  # use multithreaded debugging
  set(CMAKE_CXX_FLAGS_DEBUG "/MDd")
  set(CMAKE_CXX_FLAGS_RELEASE "/MD")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /clr /EHa")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()


add_executable( ${PROJECT_NAME} ${MANIFEST} )

target_link_libraries( ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

#
# Install
#
# include( "${PROJECT_SOURCE_DIR}/cmake/artifacts.cmake" )

# install( FILES "${INCLUDE_DIR}/${PROJECT_NAME}" DESTINATION "${CMAKE_INSTALL_PREFIX}/include" )
# install( FILES ${ARTIFACTS} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/corvusoft/${PROJECT_NAME}" )
# install( TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT library )
