INCLUDES = $(all_includes)  -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core

noinst_LTLIBRARIES = libqueryoptiondlg.la

libqueryoptiondlg_a_DLG_sources = \
  GroupOptPage.dlg \
  QueryOptPage.dlg \
  ThreshOptPage.dlg \
  BlockOptPage.dlg

libqueryoptiondlg_a_QOBJ_sources =   \
  $(libqueryoptiondlg_a_DLG_sources:.dlg=.cpp) \
  $(libqueryoptiondlg_a_DLG_sources:.dlg=Data.cpp) \
 QueryOptionsDlg.cpp

libqueryoptiondlg_la_SOURCES = $(libqueryoptiondlg_a_QOBJ_sources)
                        
EXTRA_DIST = \
  $(libqueryoptiondlg_a_QOBJ_sources:.cpp=.h) \
  $(libqueryoptiondlg_a_DLG_sources)

# CLEANFILES += *~ # Breaks badly with automake 1.5

#%Data.cpp %Data.h: %.dlg
#	-qtarch -g $<

METASOURCES = AUTO

