INCLUDES = $(all_includes)  -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core

noinst_LTLIBRARIES = libquerydlg.la


libquerydlg_a_DLG_sources = \
  VerbQueryDlg.dlg \
  AdjQueryDlg.dlg \
  ArtQueryDlg.dlg \
  QueryDlg.dlg \
  MCQueryDlg.dlg \
  SimpleQueryDlg.dlg


libquerydlg_a_QOBJ_sources =   \
  $(libquerydlg_a_DLG_sources:.dlg=.cpp) \
  $(libquerydlg_a_DLG_sources:.dlg=Data.cpp) \
  QueryDlgBase.cpp

libquerydlg_la_SOURCES = $(libquerydlg_a_QOBJ_sources) \
                        MyProgress.cpp
                        
EXTRA_DIST = \
  $(libquerydlg_a_QOBJ_sources:.cpp=.h) \
  $(libquerydlg_a_DLG_sources) \
  MyProgress.h


# CLEANFILES += *~ # This breaks badly with automake 1.5


#%Data.cpp %Data.h: %.dlg
#	-qtarch -g $<

METASOURCES = AUTO

