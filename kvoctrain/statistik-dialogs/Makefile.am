INCLUDES = $(all_includes)  -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core

noinst_LIBRARIES = libstatdlg.a

libstatdlg_a_DLG_SOURCES = \
  StatistikPage.dlg \
  GenStatPage.dlg
  
libstatdlg_a_QOBJ_SOURCES =   \
  $(libstatdlg_a_DLG_SOURCES:.dlg=.cpp) \
  $(libstatdlg_a_DLG_SOURCES:.dlg=Data.cpp) \
  StatistikDlg.cpp 


libstatdlg_a_LIBADD = $(libstatdlg_a_SOURCES:.cpp=.o)
libstatdlg_a_SOURCES = $(libstatdlg_a_QOBJ_SOURCES) $(libstatdlg_a_QOBJ_SOURCES:.cpp=.moc.cpp)

                        
EXTRA_DIST = \
  $(libstatdlg_a_QOBJ_SOURCES:.cpp=.h) \
  $(libstatdlg_a_DLG_SOURCES)

CLEANFILES += *.rpo  *~

%Data.cpp %Data.h: %.dlg
	-qtarch -g $<

METASOURCES = AUTO

