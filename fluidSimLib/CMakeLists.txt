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

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

install( TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib )
