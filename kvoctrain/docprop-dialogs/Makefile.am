INCLUDES = $(all_includes)  -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core

noinst_LTLIBRARIES = libdocpropdlg.la

libdocpropdlg_a_DLG_sources = \
  TitlePage.dlg \
  LessOptPage.dlg \
  TypeOptPage.dlg \
  TenseOptPage.dlg \
  UsageOptPage.dlg \
  DocOptionsPage.dlg \
  LangPropPage.dlg

libdocpropdlg_a_QOBJ_sources =   \
  $(libdocpropdlg_a_DLG_sources:.dlg=.cpp) \
  $(libdocpropdlg_a_DLG_sources:.dlg=Data.cpp) \
  DocPropDlg.cpp    DocPropLangDlg.cpp 


libdocpropdlg_la_SOURCES = $(libdocpropdlg_a_QOBJ_sources)

EXTRA_DIST = \
  $(libdocpropdlg_a_QOBJ_sources:.cpp=.h) \
  $(libdocpropdlg_a_DLG_sources)

# CLEANFILES += *~ # This breaks badly with automake 1.5


#%Data.cpp %Data.h: %.dlg
#	-qtarch -g $<

METASOURCES = AUTO
