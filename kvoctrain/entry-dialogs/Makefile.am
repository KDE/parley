INCLUDES = $(all_includes) -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core

noinst_LTLIBRARIES = libentrydlg.la

libentrydlg_a_DLG_sources = \
  FromToEntryPage.dlg \
  CommonEntryPage.dlg \
  AuxInfoEntryPage.dlg \
  TenseEntryPage.dlg \
  AdjEntryPage.dlg

libentrydlg_a_QOBJ_sources =   \
  $(libentrydlg_a_DLG_sources:.dlg=.cpp) \
  $(libentrydlg_a_DLG_sources:.dlg=Data.cpp) \
  EntryDlg.cpp MySpinBox.cpp


libentrydlg_la_SOURCES = $(libentrydlg_a_QOBJ_sources) blockall.cpp

EXTRA_DIST = \
  $(libentrydlg_a_QOBJ_sources:.cpp=.h) \
  $(libentrydlg_a_DLG_sources) \
  blockall.h


# CLEANFILES += *~ # This breaks badly with automake 1.5

#%Data.cpp %Data.h: %.dlg
#	-qtarch -g $<

METASOURCES = AUTO

