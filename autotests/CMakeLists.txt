include(ECMMarkAsTest)
include(ECMAddTests)
include(CTest)

find_package(Qt5 5.2.0 CONFIG REQUIRED Test)

# To add a new unittest
# 1. Add its name to parley_gui_unit_tests or parley_non_gui_unit_tests
# 2. Add any non-parley library helper cpp files to parley_unittest_helpers

# set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

set( unittest_INCLUDE_DIRS
  ../src
  ../src/practice
  ../src/collection
)

set(unittest_LINK_LIBS
  Qt5::Test
  ${parley_LINK_LIBS}
  parley_LIB
)

set(parley_unittest_helpers
  parleyunittestutilities.cpp
)

macro(PARLEY_GUI_UNITTESTS)
   foreach(_testname ${ARGN})
     get_filename_component( _name ${_testname} NAME_WE )
     ecm_add_test( ${_testname} ${parley_unittest_helpers}
       LINK_LIBRARIES ${unittest_LINK_LIBS}
       TEST_NAME ${_name}
       GUI)
   endforeach()
endmacro()

macro(PARLEY_NON_GUI_UNITTESTS)
   foreach(_testname ${ARGN})
     get_filename_component( _name ${_testname} NAME_WE )
     ecm_add_test( ${_testname} ${parley_unittest_helpers}
       LINK_LIBRARIES ${unittest_LINK_LIBS}
       TEST_NAME ${_name}
       )
   endforeach()
endmacro()


include_directories( ${unittest_INCLUDE_DIRS})


parley_gui_unittests(
  #todo restore dummy unit test when file locking is fixed
  # allpass1test
   allpass2test.cpp
)
parley_non_gui_unittests(
   testentrytest.cpp
   sessionmanagerfixedtest.cpp
)
