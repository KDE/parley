INCLUDES = $(all_includes) -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core

noinst_LIBRARIES = libentrydlg.a

libentrydlg_a_DLG_SOURCES = \
  FromToEntryPage.dlg \
  CommonEntryPage.dlg \
  AuxInfoEntryPage.dlg \
  TenseEntryPage.dlg \
  AdjEntryPage.dlg

libentrydlg_a_QOBJ_SOURCES =   \
  $(libentrydlg_a_DLG_SOURCES:.dlg=.cpp) \
  $(libentrydlg_a_DLG_SOURCES:.dlg=Data.cpp) \
  EntryDlg.cpp MySpinBox.cpp


libentrydlg_a_LIBADD= $(libentrydlg_a_SOURCES:.cpp=.o)
libentrydlg_a_SOURCES = $(libentrydlg_a_QOBJ_SOURCES) $(libentrydlg_a_QOBJ_SOURCES:.cpp=.moc.cpp) \
                        blockall.cpp

EXTRA_DIST = \
  $(libentrydlg_a_QOBJ_SOURCES:.cpp=.h) \
  $(libentrydlg_a_DLG_SOURCES) \
  blockall.h


CLEANFILES += *.rpo  *~

%Data.cpp %Data.h: %.dlg
	-qtarch -g $<

METASOURCES = AUTO

