INCLUDE(QtPrlAdditions)

IF(Qt5Core_LIBRARY_DIRS)
  SET(Qt5Core_LIBRARY_DIR ${Qt5Core_LIBRARY_DIRS})
ENDIF()

IF(TPL_Qt5Core_LIBRARY_DIRS)
  SET(Qt5Core_LIBRARY_DIR ${TPL_Qt5Core_LIBRARY_DIRS})
ENDIF()

IF(Qt5Core_INCLUDE_DIRS)
  SET(Qt5Core_INCLUDE_DIR ${Qt5Core_INCLUDE_DIRS})
ENDIF()

IF(TPL_Qt5Core_INCLUDE_DIRS)
  SET(Qt5Core_INCLUDE_DIR ${TPL_Qt5Core_INCLUDE_DIRS})
ENDIF()

IF(TPL_Qt5Core_LIBRARIES)
  SET(Qt5Core_LIBRARIES ${TPL_Qt5Core_LIBRARIES})
ENDIF()
IF(NOT Qt5Core_REQUIRED_VERSION)
  SET(Qt5Core_REQUIRED_VERSION 5.1.0)
ENDIF()
MESSAGE("REQUIRED Qt5Core version: ${Qt5Core_REQUIRED_VERSION}")
FIND_PACKAGE(Qt5Core "${Qt5Core_REQUIRED_VERSION}")

if(NOT Qt5Core_FOUND)
  message("                             ____")
  message("                     __,-~~/~    `---.")
  message("                   _/_,---(      ,    )")
  message("               __ /        <    /   )  \\___")
  message("- ------===;;;'====------------------===;;;===----- -  -")
  message("                 \\/  ~\"~\"~\"~\"~\"~\\~\"~)~\"/")
  message("                 (_ (   \\  (     >    \\)")
  message("                  \\_( _ <         >_>'")
  message("                      ~ `-i' ::>|--\"")
  message("                          I;|.|.|")
  message("                         <|i::|i|`.")
  message("                         (` ^'`-' )")
  MESSAGE(FATAL_ERROR "Couldn't find Qt5Core. This causes explosions.")
endif()
#
# Bug fix for Qt5 cmake configuration. They do not cache their variables for project use
#
SET(Qt5Core_QMAKE_EXECUTABLE ${Qt5Core_QMAKE_EXECUTABLE} CACHE STRING "")
SET(Qt5Core_MOC_EXECUTABLE ${Qt5Core_MOC_EXECUTABLE} CACHE STRING "")
SET(Qt5Core_RCC_EXECUTABLE ${Qt5Core_RCC_EXECUTABLE} CACHE STRING "")
MESSAGE(STATUS "QMAKE_EXECUTABLE: '${Qt5Core_QMAKE_EXECUTABLE}'")
MESSAGE(STATUS "MOC_EXECUTABLE: '${Qt5Core_MOC_EXECUTABLE}'")
MESSAGE(STATUS "RCC_EXECUTABLE: '${Qt5Core_RCC_EXECUTABLE}'")

#
# Extra work to get the locations correct
GET_TARGET_PROPERTY(Qt5Core_location Qt5::Core LOCATION)
GET_FILENAME_COMPONENT(Qt5Core_LIBRARY_DIRS "${Qt5Core_location}" DIRECTORY CACHE)
MESSAGE(STATUS "Qt5Core_LIBRARY_DIRS=${Qt5Core_LIBRARY_DIRS}")
#
# Get the root of this Qt installation for reference
IF(Qt5Core_LIBRARY_DIRS MATCHES ".*framework/?$")
  GET_FILENAME_COMPONENT(Qt5_INSTALL_PREFIX "${Qt5Core_LIBRARY_DIRS}/../../" ABSOLUTE CACHE)
ELSE()
  GET_FILENAME_COMPONENT(Qt5_INSTALL_PREFIX "${Qt5Core_LIBRARY_DIRS}/../" ABSOLUTE CACHE)
ENDIF()
MESSAGE(STATUS "Qt5_INSTALL_PREFIX: ${Qt5_INSTALL_PREFIX}")

SET(Qt5_PLUGINS_DIR ${Qt5_INSTALL_PREFIX}/plugins CACHE STRING "Location of Qt5 plugins")
MESSAGE(STATUS "Qt5_PLUGINS_DIR: ${Qt5_PLUGINS_DIR}")


IF(NOT(Qt5Core_INCLUDE_DIRS))
  SET(Qt5Core_INCLUDE_DIRS ${Qt5Core_INCLUDE_DIRS})
ENDIF()

IF(NOT(Qt5Core_LIBRARY_DIRS))
  SET(Qt5Core_LIBRARY_DIRS ${Qt5Core_LIBRARY_DIRS})
ENDIF()

IF(NOT TPL_Qt5Core_INCLUDE_DIRS )
  SET(TPL_Qt5Core_INCLUDE_DIRS ${Qt5Core_INCLUDE_DIRS})
ENDIF()

IF(NOT TPL_Qt5Core_LIBRARY_DIRS )
  SET(TPL_Qt5Core_LIBRARY_DIRS ${Qt5Core_LIBRARY_DIRS})
ENDIF()
IF(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
   SET(LIB_NAME QtCore)
ELSE()
   SET(LIB_NAME Qt5Core)
ENDIF()

SET(component ${LIB_NAME})
SET(component_lib_dir "${Qt5Core_LIBRARY_DIRS}")

QT_PRL_ADDITIONS(${component} ${component_lib_dir} ADDITIONAL_LIBS)
#remove duplicate entries list
IF(ADDITIONAL_LIBS)
   LIST(REMOVE_DUPLICATES ADDITIONAL_LIBS)
   MESSAGE(STATUS "Found additional libraries: ${ADDITIONAL_LIBS}")
ENDIF()
SET(TPL_Qt5Core_LIBRARIES Qt5::Core ${ADDITIONAL_LIBS} CACHE STRING "")
SET(TPL_Qt5Core_INCLUDE_DIRS ${TPL_Qt5Core_INCLUDE_DIRS} CACHE STRING "")

