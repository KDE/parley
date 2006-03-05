kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#INCLUDES = -I$(srcdir)/../../../libkdeedu/kdeeducore -I$(srcdir)/.. \
#	-I$(srcdir)/../kvt-core -I$(top_builddir)/kvoctrain/kvoctrain/common-dialogs $(all_includes)
#
#noinst_LTLIBRARIES = libdocpropdlg.la
#
#libdocpropdlg_la_SOURCES = \
#  TitlePage.cpp    TypeOptPage.cpp      TenseOptPage.cpp     UsageOptPage.cpp \
#  TitlePageForm.ui TypeOptPageForm.ui   TenseOptPageForm.ui  UsageOptPageForm.ui \
#  LessOptPage.cpp    DocOptionsPage.cpp    LangPropPage.cpp \
#  LessOptPageForm.ui DocOptionsPageForm.ui LangPropPageForm.ui \
#  DocPropDlg.cpp    DocPropLangDlg.cpp
#
#METASOURCES = AUTO
