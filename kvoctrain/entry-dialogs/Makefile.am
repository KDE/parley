INCLUDES = $(all_includes) -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core

noinst_LTLIBRARIES = libentrydlg.la

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


libentrydlg_la_SOURCES = $(libentrydlg_a_QOBJ_SOURCES) blockall.cpp

EXTRA_DIST = \
  $(libentrydlg_a_QOBJ_SOURCES:.cpp=.h) \
  $(libentrydlg_a_DLG_SOURCES) \
  blockall.h


CLEANFILES += *~

#%Data.cpp %Data.h: %.dlg
#	-qtarch -g $<

METASOURCES = AUTO

