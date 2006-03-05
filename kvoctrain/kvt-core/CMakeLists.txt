kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############

set(kvoctraincore_LIB_SRCS 
   langset.cpp 
   LineList.cpp 
   UsageManager.cpp 
   QueryManager.cpp )

kde4_automoc(${kvoctraincore_LIB_SRCS})

kde4_add_library(kvoctraincore SHARED ${kvoctraincore_LIB_SRCS})

target_link_libraries(kvoctraincore  ${QT_AND_KDECORE_LIBS} )

set_target_properties(kvoctraincore PROPERTIES VERSION 1.0.0 SOVERSION 1 )
install_targets(${LIB_INSTALL_DIR} kvoctraincore )


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#INCLUDES = -I$(srcdir)/../../../libkdeedu/kdeeducore \
#	-I$(top_builddir)/libkdeedu/kdeeducore -I$(srcdir)/.. -I../queryoption-dialogs \
#	-I$(srcdir)/../queryoption-dialogs -I$(top_builddir)/kvoctrain/kvoctrain/common-dialogs $(all_includes)
#
#lib_LTLIBRARIES = libkvoctraincore.la
#
#METASOURCES = AUTO
#
#libkvoctraincore_la_SOURCES = langset.cpp LineList.cpp UsageManager.cpp QueryManager.cpp
#
#
#
#
