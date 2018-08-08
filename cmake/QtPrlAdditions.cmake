IF(COMMAND QT_PRL_ADDITIONS)
RETURN()
ENDIF()
FUNCTION(QT_PRL_ADDITIONS LIB_NAME LIB_DIR ADDITIONS_OUT)
#platform dependent component configuration name
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
   SET(component_pkg ${component_lib_dir}/${component}.prl)
ELSEIF( EXISTS "${component_lib_dir}/${component}.prl")
   SET(component_pkg ${component_lib_dir}/${component}.prl)
ELSE()
   SET(component_pkg ${component_lib_dir}/lib${component}.prl)
ENDIF()
MESSAGE(STATUS "Searching for component: ${component_pkg}")
#Check if this componet configuration exists
IF( EXISTS ${component_pkg} )
   MESSAGE("-- Found component(${component}) configuration: ${component_pkg}")
   #read the component description file
   FILE(STRINGS ${component_pkg} file_lines)
   SET(FRAMEWORK FALSE)
   #loop over each line
   FOREACH(line ${file_lines})
      #only pay attention to library line
      IF(line MATCHES "QMAKE_PRL_LIBS")
         STRING(REGEX REPLACE "^.*=" "" line_pruned ${line})
         SPLIT( "${line_pruned}" " +" field_list)
         #
         # Process field list to remove qt libraries
         #
         SET(processed_field_list ${field_list})
         IF(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND NOT CMAKE_C_COMPILER_ID STREQUAL "GNU")
            LIST(APPEND ADDITIONAL_LIBS ${field_list})
         ELSE()
            FOREACH(field ${field_list})
               IF( field MATCHES "-l" )
                  STRING(REGEX REPLACE "-l" "" field ${field})
                  # Ignore internal qt module dependencies
                  IF(NOT field MATCHES "^Qt")
                    LIST(APPEND ADDITIONAL_LIBS ${field})
                  ENDIF()
               ELSEIF( field MATCHES "-F" )
                  # check for Darwin frameworks directory "-F/opt/local/..."
                  # ignore internal qt module directories
                  IF(NOT field MATCHES "QT")
                    LIST(APPEND ADDITIONAL_LIBS "${field}")
                  ENDIF()
               ELSEIF( field MATCHES "-framework" )
                  # check for Darwin frameworks "-framework ApplicationServices"
                  SET(FRAMEWORK TRUE)
               ELSEIF( FRAMEWORK ) # check for Darwin frameworks
                  # Ignore internal qt module dependencies
                  IF(NOT field MATCHES "^Qt")
                    LIST(APPEND ADDITIONAL_LIBS "-framework ${field}")
                  ENDIF()
                  SET(FRAMEWORK FALSE)
               ENDIF()
            ENDFOREACH()
           ENDIF()
           BREAK()
        ENDIF()
   ENDFOREACH()
ELSE()
  MESSAGE("-- Did not find component(${component})")
  MESSAGE("-- System libraries may not be included correctly!")
  MESSAGE("-- Resulting in undefined symbols at link time.")
ENDIF()
SET(ADDITIONAL_LIBS ${ADDITIONAL_LIBS} PARENT_SCOPE)
ENDFUNCTION()


