cmake_minimum_required(VERSION 3.0.0)

# KDE Application Version, managed by release script
set(KDE_APPLICATIONS_VERSION_MAJOR "18")
set(KDE_APPLICATIONS_VERSION_MINOR "08")
set(KDE_APPLICATIONS_VERSION_MICRO "0")
set(KDE_APPLICATIONS_VERSION "${KDE_APPLICATIONS_VERSION_MAJOR}.${KDE_APPLICATIONS_VERSION_MINOR}.${KDE_APPLICATIONS_VERSION_MICRO}")

project(parley VERSION ${KDE_APPLICATIONS_VERSION})

find_package(ECM 5.15.0 REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(FeatureSummary)

#Allows QString concatenation to use a single memory allocation per source line.
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)

set(REQUIRED_QT_VERSION "5.2")
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED
  Core
  Svg
  Concurrent
  Multimedia
  WebEngineWidgets
)

find_package(KF5 5.22 MODULE REQUIRED COMPONENTS
  DocTools  #to produce the docbook
  CoreAddons
  Config
  Crash
  I18n
  KIO
  NewStuff
  Kross
  #KHtml
  Sonnet
  KCMUtils
  XmlGui
  Notifications
  #Plasma
)

find_package(LibKEduVocDocument)
set_package_properties(LibKEduVocDocument PROPERTIES
                       TYPE REQUIRED
                       PURPOSE "Required to manipulate the KDE word files."
                      )

find_package(LibXslt)
set_package_properties(LibXslt PROPERTIES
                       URL "http://xmlsoft.org/XSLT"
                       TYPE OPTIONAL
                       PURPOSE "Required to build HTML export for Parley."
                      )

find_package(LibXml2)
set_package_properties(LibXml2 PROPERTIES
                       URL "http://xmlsoft.org"
                       TYPE OPTIONAL
                       PURPOSE "Required to build HTML export for Parley."
                      )


#@todo frameworks restore Xattica
# find_package(LibAttica)

# macro_log_feature(LIBATTICA_FOUND "libattica" "A library to access Open Collaboration Service providers" "http://kde.org" FALSE "" "Required to access OCS providers in get hot new stuff.")


# at the end, output the configuration
configure_file(
   ${CMAKE_CURRENT_SOURCE_DIR}/config-parley.h.cmake
   ${CMAKE_CURRENT_BINARY_DIR}/config-parley.h
)

# let our config-parley.h be found first in any case
include_directories (BEFORE ${CMAKE_CURRENT_BINARY_DIR})



# Define link libs for parley executable and unittests
set( parley_LINK_LIBS
  LibKEduVocDocumentImport__KEduVocDocument
  #KF5::KHtml
  KF5::KrossCore
  # KF5::ConfigCore
  Qt5::Core
  # Qt5::Qml
  Qt5::Svg
  KF5::I18n
  KF5::NewStuff
  KF5::SonnetCore
  KF5::SonnetUi
  KF5::KCMUtils
  KF5::XmlGui
  KF5::Completion
  KF5::ConfigWidgets
  KF5::ConfigGui
  KF5::WidgetsAddons
  KF5::TextWidgets
  KF5::KIOWidgets
  Qt5::Widgets
  Qt5::Gui
  Qt5::Concurrent
  Qt5::Multimedia
  KF5::Notifications
  KF5::Crash
  KF5::CoreAddons
  Qt5::WebEngineWidgets
)

message(STATUS "cmake cflags ${CMAKE_C_FLAGS}")
message(STATUS "cmake link flags ${CMAKE_EXE_LINKER_FLAGS}")

if(${LIBXSLT_FOUND} AND ${LIBXML2_FOUND})
  set(HAVE_LIBXSLT 1)
  message(STATUS "XLST found")
  set( parley_LINK_LIBS ${parley_LINK_LIBS} ${LIBXML2_LIBRARIES} ${LIBXSLT_LIBRARIES})
  include_directories(${LIBXML2_INCLUDE_DIR} ${LIBXSLT_INCLUDE_DIR})
else(${LIBXSLT_FOUND} AND ${LIBXML2_FOUND})
  set(HAVE_LIBXSLT 0)
  message(STATUS "XSLT not found, but not required")
endif(${LIBXSLT_FOUND} AND ${LIBXML2_FOUND})

if(X11_Xrender_FOUND)
  message(STATUS "X11 Xrender found")
  set( parley_LINK_LIBS ${parley_LINK_LIBS} ${X11_LIBRARIES} ${X11_Xrender_LIB})
else(X11_Xrender_FOUND)
  message(STATUS "X11 Xrender not found, but not required")
endif(X11_Xrender_FOUND)


#define the include libraries for both the parley executable and the autotests
# set( parley_INCLUDE_DIRS
#   # ${LIBATTICA_INCLUDE_DIR}
#   # libkdeedu/keduvocdocument contains the library for reading and writing vocabulary files
#   ${LIBKDEEDU_INCLUDE_DIR}
# )


if(BUILD_TESTING)
  add_subdirectory( autotests )
  add_subdirectory( tests )
endif()

add_subdirectory( docs )
add_subdirectory( src )
add_subdirectory( icons )
add_subdirectory( plugins )
add_subdirectory( themes )
add_subdirectory( tipofday )

if(HAVE_LIBXSLT)
    add_subdirectory( xslt )
endif(HAVE_LIBXSLT)


#@todo unused This seems unused delete December 2014
#add_subdirectory( scripts_test )

#@todo frameworks restore/rewrite the plasmoid
#add_subdirectory(plasmoid)


install(FILES org.kde.parley.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
