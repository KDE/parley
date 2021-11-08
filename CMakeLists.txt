cmake_minimum_required(VERSION 3.16)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "22")
set(RELEASE_SERVICE_VERSION_MINOR "03")
set(RELEASE_SERVICE_VERSION_MICRO "70")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(parley VERSION ${RELEASE_SERVICE_VERSION})
set(QT_MIN_VERSION "5.15.0")
set(KF5_MIN_VERSION "5.73.0")

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(ECMEnableSanitizers)
include(FeatureSummary)
include(KDEClangFormat)
include(KDEGitCommitHooks)

#Allows QString concatenation to use a single memory allocation per source line.
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)
add_definitions(-DQT_NO_FOREACH)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED
  Core
  Svg
  Concurrent
  Multimedia
  WebEngineWidgets
)

find_package(KF5 ${KF5_MIN_VERSION} MODULE REQUIRED COMPONENTS
  DocTools  #to produce the docbook
  CoreAddons
  Config
  Crash
  I18n
  KIO
  NewStuff
  Sonnet
  TextWidgets
  KCMUtils
  XmlGui
  Notifications
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

find_package(TranslateShell)
set_package_properties(TranslateShell PROPERTIES
                       URL "https://github.com/soimort/translate-shell"
                       TYPE RUNTIME
                       PURPOSE "Runtime dependency for online access to translations"
                      )

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
  Qt5::Core
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

if(BUILD_TESTING)
  add_subdirectory( autotests )
  add_subdirectory( tests )
endif()

add_subdirectory( docs )
add_subdirectory( src )
add_subdirectory( icons )
add_subdirectory( themes )

if(HAVE_LIBXSLT)
    add_subdirectory( xslt )
endif(HAVE_LIBXSLT)

ki18n_install(po)
if (KF5DocTools_FOUND)
    kdoctools_install(po)
endif()
install(FILES org.kde.parley.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

# add clang-format target for all our real source files
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
