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


doc:


messages:

  
GenOptPage.moc.cpp: $(srcdir)/GenOptPage.h
	$(MOC) $< -o GenOptPage.moc.cpp

GenOptPageData.moc.cpp: $(srcdir)/GenOptPageData.h
	$(MOC) $< -o GenOptPageData.moc.cpp

LangOptPage.moc.cpp: $(srcdir)/LangOptPage.h
	$(MOC) $< -o LangOptPage.moc.cpp

LangOptPageData.moc.cpp: $(srcdir)/LangOptPageData.h
	$(MOC) $< -o LangOptPageData.moc.cpp

ViewOptPage.moc.cpp: $(srcdir)/ViewOptPage.h
	$(MOC) $< -o ViewOptPage.moc.cpp

ViewOptPageData.moc.cpp: $(srcdir)/ViewOptPageData.h
	$(MOC) $< -o ViewOptPageData.moc.cpp

PasteOptPage.moc.cpp: $(srcdir)/PasteOptPage.h
	$(MOC) $< -o PasteOptPage.moc.cpp

PasteOptPageData.moc.cpp: $(srcdir)/PasteOptPageData.h
	$(MOC) $< -o PasteOptPageData.moc.cpp

GeneralOptionsDlg.moc.cpp: $(srcdir)/GeneralOptionsDlg.h
	$(MOC) $< -o GeneralOptionsDlg.moc.cpp

clean-metasources:
	-rm -f $(liboptiondlg_a_METASOURCES) *.moc.*

release: 
