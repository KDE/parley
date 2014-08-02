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


# Define link libs for parley executable and unittests
set( parley_LINK_LIBS
    ${LIBKDEEDU_KEDUVOCDOCUMENT_LIBRARIES}
    ${KDE4_KHTML_LIBS}
    ${KDE4_KNEWSTUFF3_LIBS}
    ${KDE4_PHONON_LIBS}
    ${KDE4_KCMUTILS_LIBRARY} #for the KPluginSelector
    ${KDE4_KROSSCORE_LIBS} ${KDE4_KROSSUI_LIBS} #for Kross
    ${QT_QTCORE_LIBRARY}
)

if(HAVE_LIBXSLT)
  set( parley_LINK_LIBS ${parley_LINK_LIBS} ${LIBXML2_LIBRARIES} ${LIBXSLT_LIBRARIES})
endif(HAVE_LIBXSLT)

if (X11_Xrender_FOUND)
  set( parley_LINK_LIBS ${parley_LINK_LIBS} ${X11_LIBRARIES} ${X11_Xrender_LIB})
endif(X11_Xrender_FOUND)


#define the include libraries for both the parley executable and the autotests
set( parley_INCLUDE_DIRS
  ${LIBATTICA_INCLUDE_DIR}
  # libkdeedu/keduvocdocument contains the library for reading and writing vocabulary files
  ${LIBKDEEDU_INCLUDE_DIR}
)

#### XSLT Support ####
# to transform the document xml to html or other
if(HAVE_LIBXSLT)
    set( parley_INCLUDE_DIRS ${parley_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIR} ${LIBXSLT_INCLUDE_DIR} )
endif(HAVE_LIBXSLT)
#### End XSLT Support ####



add_subdirectory( autotests )
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
