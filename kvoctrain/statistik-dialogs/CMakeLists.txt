kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#INCLUDES = -I$(srcdir)/../../../libkdeedu/kdeeducore -I$(srcdir)/.. \
#	-I$(srcdir)/../kvt-core -I$(top_builddir)/kvoctrain/kvoctrain/common-dialogs $(all_includes)
#
#noinst_LTLIBRARIES = libstatdlg.la
#
#libstatdlg_la_SOURCES = \
#  StatistikPage.cpp GenStatPage.cpp \
#  StatistikPageForm.ui GenStatPageForm.ui \
#  StatistikDlg.cpp
#
#METASOURCES = AUTO
#
