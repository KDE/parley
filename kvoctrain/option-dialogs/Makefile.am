INCLUDES = $(all_includes)  -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core

noinst_LIBRARIES = liboptiondlg.a

liboptiondlg_a_DLG_SOURCES = \
  GenOptPage.dlg \
  LangOptPage.dlg \
  ViewOptPage.dlg \
  PasteOptPage.dlg
  
liboptiondlg_a_QOBJ_SOURCES =   \
  $(liboptiondlg_a_DLG_SOURCES:.dlg=.cpp) \
  $(liboptiondlg_a_DLG_SOURCES:.dlg=Data.cpp) \
  GeneralOptionsDlg.cpp 


liboptiondlg_a_LIBADD=$(liboptiondlg_a_SOURCES:.cpp=.o)
liboptiondlg_a_SOURCES = $(liboptiondlg_a_QOBJ_SOURCES) $(liboptiondlg_a_QOBJ_SOURCES:.cpp=.moc.cpp)
                        
EXTRA_DIST = \
  $(liboptiondlg_a_QOBJ_SOURCES:.cpp=.h) \
  $(liboptiondlg_a_DLG_SOURCES)

CLEANFILES += *.rpo  *~

%Data.cpp %Data.h: %.dlg
	-qtarch -g $<

METASOURCES = AUTO

