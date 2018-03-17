project(niv-server CXX)
include("${PROJECT_SOURCE_DIR}/manifest.cmake")

LIST(APPEND CMAKE_PROGRAM_PATH  "${DOCOPT_PATH}/lib")

# to find include_dir
include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/../restbed/build/include")
include_directories(SYSTEM "${BOOST_PATH}")
include_directories(SYSTEM "${DOCOPT_PATH}/include")

add_executable( ${PROJECT_NAME} ${MANIFEST} )
add_dependencies( ${PROJECT_NAME} "references-lib" )

target_link_libraries( ${PROJECT_NAME}
                    "${PROJECT_SOURCE_DIR}/../restbed/build/library/restbed.lib"
                    "${PROJECT_SOURCE_DIR}/../install/lib/references-lib.lib"
                    "${DOCOPT_PATH}/lib/docopt.lib"
                    )
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

link_directories("${PROJECT_SOURCE_DIR}/../restbed/build/library")
link_directories("${DOCOPT_PATH}/lib")
