# $Id: CMakeLists.txt 86065 2014-11-07 08:51:15Z gcosmo $

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(MTASSimulation)
# The version number.
set (MTASSimulation_VERSION_MAJOR 2)
set (MTASSimulation_VERSION_MINOR 0)
set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

#configure a header file to pass some of the CMake settings to the source code
configure_file ("${PROJECT_SOURCE_DIR}/MTASSimulationConfig.h.in"
		"${PROJECT_BINARY_DIR}/MTASSimulation.h"
		)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 10.3.1 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 10.3.1  REQUIRED)
endif()



# Find ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
set(ROOT_LIBRARIES -L${ROOT_LIBRARY_DIR} -lCore  -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include
		    ${Geant4_INCLUDE_DIR}
		    ${ROOT_INCLUDE_DIR}
		    ${PROJECT_BINARY_DIR}
		    )


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
#echo ${ROOT_LIBRARIES}
add_executable(MTASSimulation_XML MTASSimulation_XML.cc ${sources} ${headers})
target_link_libraries(MTASSimulation_XML 
	${Geant4_LIBRARIES}
	${ROOT_LIBRARIES}
	)
#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(MTASSimulation_SCRIPTS
  input.mac
  vis.mac
  )

foreach(_script ${MTASSimulation_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()


#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS MTASSimulation_XML DESTINATION bin)


