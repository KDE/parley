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


doc:


messages:


CommonEntryPage.moc.cpp: $(srcdir)/CommonEntryPage.h
	$(MOC) $< -o CommonEntryPage.moc.cpp

CommonEntryPageData.moc.cpp: $(srcdir)/CommonEntryPageData.h
	$(MOC) $< -o CommonEntryPageData.moc.cpp

FromToEntryPage.moc.cpp: $(srcdir)/FromToEntryPage.h
	$(MOC) $< -o FromToEntryPage.moc.cpp

FromToEntryPageData.moc.cpp: $(srcdir)/FromToEntryPageData.h
	$(MOC) $< -o FromToEntryPageData.moc.cpp

AuxInfoEntryPage.moc.cpp: $(srcdir)/AuxInfoEntryPage.h
	$(MOC) $< -o AuxInfoEntryPage.moc.cpp

AuxInfoEntryPageData.moc.cpp: $(srcdir)/AuxInfoEntryPageData.h
	$(MOC) $< -o AuxInfoEntryPageData.moc.cpp

TenseEntryPage.moc.cpp: $(srcdir)/TenseEntryPage.h
	$(MOC) $< -o TenseEntryPage.moc.cpp

TenseEntryPageData.moc.cpp: $(srcdir)/TenseEntryPageData.h
	$(MOC) $< -o TenseEntryPageData.moc.cpp

AdjEntryPage.moc.cpp: $(srcdir)/AdjEntryPage.h
	$(MOC) $< -o AdjEntryPage.moc.cpp

AdjEntryPageData.moc.cpp: $(srcdir)/AdjEntryPageData.h
	$(MOC) $< -o AdjEntryPageData.moc.cpp

EntryDlg.moc.cpp: $(srcdir)/EntryDlg.h
	$(MOC) $< -o EntryDlg.moc.cpp

MySpinBox.moc.cpp: $(srcdir)/MySpinBox.h
	$(MOC) $< -o MySpinBox.moc.cpp

clean-metasources:
	-rm -f $(libentrydlg_a_METASOURCES) *.moc.*

release: 
