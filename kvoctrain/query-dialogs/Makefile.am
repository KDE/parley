INCLUDES = $(all_includes)  -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core

noinst_LTLIBRARIES = libquerydlg.la


libquerydlg_a_DLG_SOURCES = \
  VerbQueryDlg.dlg \
  AdjQueryDlg.dlg \
  ArtQueryDlg.dlg \
  QueryDlg.dlg \
  MCQueryDlg.dlg \
  SimpleQueryDlg.dlg


libquerydlg_a_QOBJ_SOURCES =   \
  $(libquerydlg_a_DLG_SOURCES:.dlg=.cpp) \
  $(libquerydlg_a_DLG_SOURCES:.dlg=Data.cpp) \
  QueryDlgBase.cpp

libquerydlg_la_SOURCES = $(libquerydlg_a_QOBJ_SOURCES) \
                        MyProgress.cpp
                        
EXTRA_DIST = \
  $(libquerydlg_a_QOBJ_SOURCES:.cpp=.h) \
  $(libquerydlg_a_DLG_SOURCES) \
  MyProgress.h


CLEANFILES += *~


#%Data.cpp %Data.h: %.dlg
#	-qtarch -g $<

METASOURCES = AUTO

