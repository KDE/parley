# To add a new unittest
# 1. Add its name to parley_unittests

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

set( unittest_INCLUDE_DIRS
  ../src
  ../src/practice
  ../src/collection
)

set(unittest_LINK_LIBS
  ${QT_QTTEST_LIBRARY}
)

set(parley_unittests
  allpass1test
  allpass2test
)

set(parley_unittests_helpers
  parleyunittestutilities.cpp
)

macro(ADD_PARLEY_UNITTESTS)
  foreach(_unittestname ${ARGN} )

    kde4_add_unit_test(${_unittestname} ${_unittestname}.cpp ${parley_unittests_helpers} )

    target_link_libraries( ${_unittestname} ${unittest_LINK_LIBS} parley_LIB ${parley_LINK_LIBS} )

  endforeach(_unittestname ${argn} )
endmacro(ADD_PARLEY_UNITTESTS)



include_directories( ${unittest_INCLUDE_DIRS} ${parley_INCLUDE_DIRS} )

ADD_PARLEY_UNITTESTS( ${parley_unittests} )
