INCLUDES = -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core $(all_includes)

noinst_LTLIBRARIES = libqueryoptiondlg.la

libqueryoptiondlg_a_DLG_SOURCES = \
  GroupOptPage.dlg \
  QueryOptPage.dlg \
  ThreshOptPage.dlg \
  BlockOptPage.dlg

libqueryoptiondlg_a_QOBJ_SOURCES =   \
  $(libqueryoptiondlg_a_DLG_SOURCES:.dlg=.cpp) \
  $(libqueryoptiondlg_a_DLG_SOURCES:.dlg=Data.cpp) \
 QueryOptionsDlg.cpp

libqueryoptiondlg_a_SOURCES = $(libqueryoptiondlg_a_QOBJ_SOURCES) 
                        
EXTRA_DIST = \
  $(libqueryoptiondlg_a_QOBJ_SOURCES:.cpp=.h) \
  $(libqueryoptiondlg_a_DLG_SOURCES)

CLEANFILES += *.rpo  *~

%Data.cpp %Data.h: %.dlg
	-qtarch -g $<

METASOURCES = AUTO

