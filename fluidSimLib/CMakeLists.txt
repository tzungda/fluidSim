cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0074 NEW) # recognize <Package>_Root variables

project(fluidSimLib)

file( GLOB SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*cpp )
file( GLOB INCS ${CMAKE_CURRENT_SOURCE_DIR}/*h )

# add files
add_library( ${PROJECT_NAME} STATIC ${SRCS} ${INCS} )

set( fluidSimLib_VERSION 1 )
set( fluidSimLib_VERSION_MAJOR 0 )

set_target_properties(fluidSimLib
    PROPERTIES
        CMAKE_POSITION_INDEPENDENT_CODE ON
        VERSION ${fluidSimLib_VERSION}
        SOVERSION ${fluidSimLib_VERSION_MAJOR}
)

install( TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib )