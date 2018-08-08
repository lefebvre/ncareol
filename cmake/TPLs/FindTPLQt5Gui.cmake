INCLUDE(QtPrlAdditions)

IF(Qt5Gui_LIBRARY_DIRS)
  SET(Qt5Gui_LIBRARY_DIR ${Qt5Gui_LIBRARY_DIRS})
ENDIF()

IF(TPL_Qt5Gui_LIBRARY_DIRS)
  SET(Qt5Gui_LIBRARY_DIR ${TPL_Qt5Gui_LIBRARY_DIRS})
ENDIF()

IF(Qt5Gui_INCLUDE_DIRS)
  SET(Qt5Gui_INCLUDE_DIR ${Qt5Gui_INCLUDE_DIRS})
ENDIF()

IF(TPL_Qt5Gui_INCLUDE_DIRS)
  SET(Qt5Gui_INCLUDE_DIR ${TPL_Qt5Gui_INCLUDE_DIRS})
ENDIF()

IF(TPL_Qt5Gui_LIBRARIES)
  SET(Qt5Gui_LIBRARIES ${TPL_Qt5Gui_LIBRARIES})
ENDIF()
IF(NOT Qt5Gui_REQUIRED_VERSION)
  SET(Qt5Gui_REQUIRED_VERSION 5.1.0)
ENDIF()
MESSAGE("REQUIRED Qt5Gui version: ${Qt5Gui_REQUIRED_VERSION}")
FIND_PACKAGE(Qt5Gui "${Qt5Gui_REQUIRED_VERSION}")

if(NOT Qt5Gui_FOUND)
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
  MESSAGE(FATAL_ERROR "Couldn't find Qt5Gui. This causes explosions.")
endif()
#
# Bug fix for Qt5 cmake configuration. They do not cache their variables for project use
#
SET(Qt5Core_QMAKE_EXECUTABLE ${Qt5Core_QMAKE_EXECUTABLE} CACHE STRING "")
SET(Qt5Core_MOC_EXECUTABLE ${Qt5Core_MOC_EXECUTABLE} CACHE STRING "")
SET(Qt5Core_RCC_EXECUTABLE ${Qt5Core_RCC_EXECUTABLE} CACHE STRING "")
#
# Extra work to get the location correct
GET_TARGET_PROPERTY(Qt5Gui_location Qt5::Gui LOCATION)
GET_FILENAME_COMPONENT(Qt5Gui_LIBRARY_DIRS "${Qt5Gui_location}" DIRECTORY CACHE)
IF(NOT(Qt5Gui_INCLUDE_DIRS))
  SET(Qt5Gui_INCLUDE_DIRS ${Qt5Gui_INCLUDE_DIRS})
ENDIF()

IF(NOT(Qt5Gui_LIBRARY_DIRS))
  SET(Qt5Gui_LIBRARY_DIRS ${Qt5Gui_LIBRARY_DIRS})
ENDIF()

IF(NOT(TPL_Qt5Gui_INCLUDE_DIRS))
  SET(TPL_Qt5Gui_INCLUDE_DIRS ${Qt5Gui_INCLUDE_DIRS})
ENDIF()

IF(NOT(TPL_Qt5Gui_LIBRARY_DIRS))
  SET(TPL_Qt5Gui_LIBRARY_DIRS ${Qt5Gui_LIBRARY_DIRS})
ENDIF()

IF(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
   SET(LIB_NAME QtGui)
ELSE()
   SET(LIB_NAME Qt5Gui)
ENDIF()

SET(component ${LIB_NAME})
SET(component_lib_dir "${Qt5Gui_LIBRARY_DIRS}")

QT_PRL_ADDITIONS(${component} ${component_lib_dir} ADDITIONAL_LIBS)
#remove duplicate entries list
IF(ADDITIONAL_LIBS)
   LIST(REMOVE_DUPLICATES ADDITIONAL_LIBS)
   MESSAGE(STATUS "Found additional libraries: ${ADDITIONAL_LIBS}")
ENDIF()

SET(TPL_Qt5Gui_LIBRARIES Qt5::Gui ${ADDITIONAL_LIBS} CACHE STRING "")
SET(TPL_Qt5Gui_INCLUDE_DIRS ${TPL_Qt5Gui_INCLUDE_DIRS} CACHE STRING "")
