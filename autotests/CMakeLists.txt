include(ECMMarkAsTest)
include(ECMAddTests)
include(CTest)

find_package(Qt${QT_MAJOR_VERSION} 5.2.0 CONFIG REQUIRED Test)

# To add a new unittest
# 1. Add its name to parley_gui_unit_tests or parley_non_gui_unit_tests
# 2. Add any non-parley library helper cpp files to parley_unittest_helpers

# set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

include_directories(
    ../src
    ../src/practice
    ../src/editor
    ../src/collection
)

set(parley_unittest_helpers
    parleyunittestutilities.cpp
)

macro(PARLEY_GUI_UNITTESTS)
   foreach(_testname ${ARGN})
     get_filename_component( _name ${_testname} NAME_WE )
     ecm_add_test( ${_testname} ${parley_unittest_helpers}
       LINK_LIBRARIES
            parley_LIB
            Qt5::Test
       TEST_NAME ${_name}
       GUI)
   endforeach()
endmacro()

macro(PARLEY_NON_GUI_UNITTESTS)
   foreach(_testname ${ARGN})
     get_filename_component( _name ${_testname} NAME_WE )
     ecm_add_test( ${_testname} ${parley_unittest_helpers}
       LINK_LIBRARIES
            parley_LIB
            Qt5::Test
       TEST_NAME ${_name}
       )
   endforeach()
endmacro()

parley_non_gui_unittests(
    testentrytest.cpp
    sessionmanagerfixedtest.cpp
    translateshelltest.cpp
)
