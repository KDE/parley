INCLUDES = $(all_includes)  -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core

noinst_LIBRARIES = libqueryoptiondlg.a

libqueryoptiondlg_a_DLG_SOURCES = \
  GroupOptPage.dlg \
  QueryOptPage.dlg \
  ThreshOptPage.dlg \
  BlockOptPage.dlg

libqueryoptiondlg_a_QOBJ_SOURCES =   \
  $(libqueryoptiondlg_a_DLG_SOURCES:.dlg=.cpp) \
  $(libqueryoptiondlg_a_DLG_SOURCES:.dlg=Data.cpp) \
 QueryOptionsDlg.cpp

libqueryoptiondlg_a_LIBADD = $(libqueryoptiondlg_a_SOURCES:.cpp=.o)
libqueryoptiondlg_a_SOURCES = $(libqueryoptiondlg_a_QOBJ_SOURCES) \
                              $(libqueryoptiondlg_a_QOBJ_SOURCES:.cpp=.moc.cpp)
                        
EXTRA_DIST = \
  $(libqueryoptiondlg_a_QOBJ_SOURCES:.cpp=.h) \
  $(libqueryoptiondlg_a_DLG_SOURCES)

CLEANFILES += *.rpo  *~

%Data.cpp %Data.h: %.dlg
	-qtarch -g $<

METASOURCES = AUTO

