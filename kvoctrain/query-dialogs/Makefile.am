INCLUDES = $(all_includes)  -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core

noinst_LIBRARIES = libquerydlg.a


libquerydlg_a_DLG_SOURCES = \
  VerbQueryDlg.dlg \
  AdjQueryDlg.dlg \
  ArtQueryDlg.dlg \
  QueryDlg.dlg \
  MCQueryDlg.dlg \
  SimpleQueryDlg.dlg


libquerydlg_a_LIBADD  = $(libquerydlg_a_SOURCES:.cpp=.o)
libquerydlg_a_QOBJ_SOURCES =   \
  $(libquerydlg_a_DLG_SOURCES:.dlg=.cpp) \
  $(libquerydlg_a_DLG_SOURCES:.dlg=Data.cpp) \
  QueryDlgBase.cpp

libquerydlg_a_SOURCES = $(libquerydlg_a_QOBJ_SOURCES) \
                        $(libquerydlg_a_QOBJ_SOURCES:.cpp=.moc.cpp) \
                        MyProgress.cpp
                        
EXTRA_DIST = \
  $(libquerydlg_a_QOBJ_SOURCES:.cpp=.h) \
  $(libquerydlg_a_DLG_SOURCES) \
  MyProgress.h


CLEANFILES += *.rpo  *~


%Data.cpp %Data.h: %.dlg
	-qtarch -g $<

METASOURCES = AUTO

