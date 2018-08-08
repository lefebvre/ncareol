################################################################################
#                                                                              #
#             ncareol  TriBITS Project and Package CMakeLists.txt File           #
#                                                                              #
################################################################################

IF (NOT TRIBITS_PROCESSING_PACKAGE)
  # This CMakeLists.txt file is being processed as the ncareol TriBITS projects's base
  # CMakeLists.txt file!  (See comments at bottom of this file.)
  CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12 FATAL_ERROR)
  INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/ProjectName.cmake")
  PROJECT(${PROJECT_NAME} NONE)
  # enable C++11 by default
  SET(ncareol_ENABLE_CXX11 ON CACHE BOOL "Compile using the C++11 standard" FORCE)

  # disable generating HTML dependencies webpage and xml files
  SET(${PROJECT_NAME}_DEPS_XML_OUTPUT_FILE OFF CACHE BOOL "")
  # disable TriBITS export system to save time configuring
  SET(${PROJECT_NAME}_ENABLE_INSTALL_CMAKE_CONFIG_FILES OFF CACHE BOOL "")
  SET(${PROJECT_NAME}_ENABLE_EXPORT_MAKEFILES OFF CACHE BOOL "")
  SET(${PROJECT_NAME}_TRIBITS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/TriBITS/tribits" CACHE PATH "")
  INCLUDE("${${PROJECT_NAME}_TRIBITS_DIR}/TriBITS.cmake")
  #
  # For windows with BUILD_SHARED_LIBS we must use CMAKE_RUNTIME_OUTPUT_DIRECTORY
  # to place all *dll and *exe in the same directory so unit tests will work
  IF("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows" AND BUILD_SHARED_LIBS)
    IF(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
      SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/cmake_runtime_output" CACHE STRING "")
    ENDIF()
  ENDIF()

  TRIBITS_PROJECT_ENABLE_ALL()
ELSE()
  # This CMakeLists.txt file is being processed as the TriBITS package file.
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
  # include binary directory for configured includes
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
  TRIBITS_PACKAGE_DECL(ncareol)
  #
  # Process subpackages
  #
  TRIBITS_PROCESS_SUBPACKAGES()
  TRIBITS_PACKAGE_POSTPROCESS()
ENDIF()

# NOTE: In order to allow the `ncareol/` directory to be both a TriBITS
# package (for inclusion in other TriBITS projects) and to be a TriBITS
# project itself, you only have to put in a simple if statement in this
# top-level CMakeLists.txt file.  That is all!
