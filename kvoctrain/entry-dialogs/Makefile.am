INCLUDES = -I$(top_srcdir)/kvoctrain -I$(top_srcdir)/kvoctrain/kvt-core \
	   -I$(top_srcdir)/kvoctrain/kvoctrain/docprop-dialogs \
	   -I$(top_builddir)/kvoctrain/kvoctrain/docprop-dialogs \
 -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core \
 $(all_includes)

noinst_LTLIBRARIES = libentrydlg.la

libentrydlg_la_SOURCES =  \
  PhoneticEntryPage.cpp  \
  PhoneticEntryPageForm.ui  \
  FromToEntryPage.cpp    CommonEntryPage.cpp     AuxInfoEntryPage.cpp \
  FromToEntryPageForm.ui CommonEntryPageForm.ui  AuxInfoEntryPageForm.ui \
  TenseEntryPage.cpp     AdjEntryPage.cpp        MCEntryPage.cpp \
  TenseEntryPageForm.ui  AdjEntryPageForm.ui     MCEntryPageForm.ui \
  EntryDlg.cpp MySpinBox.cpp blockall.cpp

METASOURCES = AUTO

