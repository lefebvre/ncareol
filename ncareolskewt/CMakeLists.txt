TRIBITS_SUBPACKAGE(skewt)


SET(SOURCE
Adiabats.cpp
IsoMR.cpp
IsoPleth.cpp
IsoPress.cpp
IsoTherms.cpp
PAdiabats.cpp
PresRect.cpp
Rect.cpp
SkewT.cpp
SkewTAdapter.cpp
SkewTAdapterSVG.cpp
SkewTMetFormulas.cpp
SkewTRect.cpp
SkewTtrace.cpp
WindBarb.cpp
WindBarbs.cpp
)

SET(HEADERS
Adiabats.h
IsoMR.h
IsoPleth.h
IsoPress.h
IsoTherms.h
PAdiabats.h
PresRect.h
Rect.h
SkewT.h
SkewTAdapter.h
SkewTAdapterSVG.h
SkewTMetFormulas.h
SkewTRect.h
SkewTdefs.h
SkewTdoxygen.h
SkewTtrace.h
WindBarb.h
WindBarbs.h
)

#
# Add library
TRIBITS_ADD_LIBRARY(skewt
  SOURCES ${SOURCE}
  NOINSTALLHEADERS ${HEADERS}
)
#
# Add testing directory
#TRIBITS_ADD_TEST_DIRECTORIES(tests)

INSTALL(FILES ${HEADERS} DESTINATION "include/skewt/")
TRIBITS_SUBPACKAGE_POSTPROCESS()
