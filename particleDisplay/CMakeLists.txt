cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0074 NEW) # recognize <Package>_Root variables

project(particleDisplay)

# add fluidSimLib lib
set ( fluidSimLib_Dir "../fluidSimLib" )
add_subdirectory( ${fluidSimLib_Dir} fluidSimLib )

file( GLOB SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*cpp ${CMAKE_CURRENT_SOURCE_DIR}/*c )
file( GLOB INCS ${CMAKE_CURRENT_SOURCE_DIR}/*h ${CMAKE_CURRENT_SOURCE_DIR}/stb ${CMAKE_CURRENT_SOURCE_DIR}/KHR )

# add files
add_executable( ${PROJECT_NAME} ${SRCS} ${INCS} )

set( particleDisplay_VERSION 1 )
set( particleDisplay_VERSION_MAJOR 0 )

# include dirs
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${fluidSimLib_Dir} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/external/glm-master )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw-3.3.8.bin.WIN64/include )

# lib paths
target_link_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw-3.3.8.bin.WIN64/lib-vc2017
    )
# libs
target_link_libraries(${PROJECT_NAME}
fluidSimLib
    glfw3.lib
)

set_target_properties(particleDisplay
    PROPERTIES
        CMAKE_POSITION_INDEPENDENT_CODE ON
        VERSION ${particleDisplay_VERSION}
        SOVERSION ${particleDisplay_VERSION_MAJOR}
)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

install( TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib ARCHIVE DESTINATION exe )
