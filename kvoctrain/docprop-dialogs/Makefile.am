INCLUDES = $(all_includes)  -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core

noinst_LIBRARIES = libdocpropdlg.a

libdocpropdlg_a_DLG_SOURCES = \
  TitlePage.dlg \
  LessOptPage.dlg \
  TypeOptPage.dlg \
  TenseOptPage.dlg \
  UsageOptPage.dlg \
  DocOptionsPage.dlg \
  LangPropPage.dlg

libdocpropdlg_a_QOBJ_SOURCES =   \
  $(libdocpropdlg_a_DLG_SOURCES:.dlg=.cpp) \
  $(libdocpropdlg_a_DLG_SOURCES:.dlg=Data.cpp) \
  DocPropDlg.cpp    DocPropLangDlg.cpp 


libdocpropdlg_a_SOURCES = $(libdocpropdlg_a_QOBJ_SOURCES) $(libdocpropdlg_a_QOBJ_SOURCES:.cpp=.moc.cpp)
libdocpropdlg_a_LIBADD = $(libdocpropdlg_a_SOURCES:.cpp=.o)

EXTRA_DIST = \
  $(libdocpropdlg_a_QOBJ_SOURCES:.cpp=.h) \
  $(libdocpropdlg_a_DLG_SOURCES)

CLEANFILES += *.rpo  *~


%Data.cpp %Data.h: %.dlg
	-qtarch -g $<

METASOURCES = AUTO
