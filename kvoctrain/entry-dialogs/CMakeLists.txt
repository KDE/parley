kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#INCLUDES = -I$(srcdir)/../../../libkdeedu/kdeeducore -I$(srcdir)/.. \
#	-I$(srcdir)/../kvt-core -I../docprop-dialogs -I$(srcdir)/../docprop-dialogs \
#	-I$(top_builddir)/kvoctrain/kvoctrain/common-dialogs $(all_includes)
#
#noinst_LTLIBRARIES = libentrydlg.la
#
#libentrydlg_la_SOURCES = PhoneticEntryPage.cpp FromToEntryPage.cpp \
#	CommonEntryPage.cpp AuxInfoEntryPage.cpp FromToEntryPageForm.ui CommonEntryPageForm.ui \
#	AuxInfoEntryPageForm.ui TenseEntryPage.cpp AdjEntryPage.cpp MCEntryPage.cpp \
#	TenseEntryPageForm.ui AdjEntryPageForm.ui MCEntryPageForm.ui EntryDlg.cpp MySpinBox.cpp \
#	blockall.cpp
#
#METASOURCES = AUTO
#
