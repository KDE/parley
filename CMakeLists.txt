project(parley)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

set(CMAKE_MODULE_PATH ${parley_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package(LibKdeEdu REQUIRED)

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

macro_optional_find_package(LibXslt)
macro_optional_find_package(LibXml2)
find_package(LibAttica)

macro_bool_to_01(LIBXSLT_FOUND AND LIBXML2_FOUND HAVE_LIBXSLT)

macro_log_feature(LIBXSLT_FOUND "LibXSLT" "A library to transform XMLfiles into other XML files" "http://xmlsoft.org/XSLT" FALSE "" "Required to build HTML export for Parley.")
macro_log_feature(LIBXML2_FOUND "LibXML2" "Libraries used to develop XML applications" "http://xmlsoft.org" FALSE "" "Required to build HTML export for Parley.")
macro_log_feature(LIBATTICA_FOUND "libattica" "A library to access Open Collaboration Service providers" "http://kde.org" FALSE "" "Required to access OCS providers in get hot new stuff.")

macro_bool_to_01(1 HAVE_XRENDER)

# at the end, output the configuration
configure_file(
   ${CMAKE_CURRENT_SOURCE_DIR}/config-parley.h.cmake
   ${CMAKE_CURRENT_BINARY_DIR}/config-parley.h
)

add_subdirectory( doc )
add_subdirectory( scripts_test )
add_subdirectory( src )
add_subdirectory( icons )
add_subdirectory( plugins )
add_subdirectory( themes )
add_subdirectory( tipofday )

if(HAVE_LIBXSLT)
    add_subdirectory( xslt )
endif(HAVE_LIBXSLT)

add_subdirectory(plasmoid)

install(FILES parley.appdata.xml DESTINATION ${SHARE_INSTALL_PREFIX}/appdata/)
