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


doc:


messages:

  
TitlePage.moc.cpp: $(srcdir)/TitlePage.h
	$(MOC) $< -o TitlePage.moc.cpp

TitlePageData.moc.cpp: $(srcdir)/TitlePageData.h
	$(MOC) $< -o TitlePageData.moc.cpp

LessOptPage.moc.cpp: $(srcdir)/LessOptPage.h
	$(MOC) $< -o LessOptPage.moc.cpp

LessOptPageData.moc.cpp: $(srcdir)/LessOptPageData.h
	$(MOC) $< -o LessOptPageData.moc.cpp

TypeOptPage.moc.cpp: $(srcdir)/TypeOptPage.h
	$(MOC) $< -o TypeOptPage.moc.cpp

TypeOptPageData.moc.cpp: $(srcdir)/TypeOptPageData.h
	$(MOC) $< -o TypeOptPageData.moc.cpp

TenseOptPage.moc.cpp: $(srcdir)/TenseOptPage.h
	$(MOC) $< -o TenseOptPage.moc.cpp

TenseOptPageData.moc.cpp: $(srcdir)/TenseOptPageData.h
	$(MOC) $< -o TenseOptPageData.moc.cpp

UsageOptPage.moc.cpp: $(srcdir)/UsageOptPage.h
	$(MOC) $< -o UsageOptPage.moc.cpp

UsageOptPageData.moc.cpp: $(srcdir)/UsageOptPageData.h
	$(MOC) $< -o UsageOptPageData.moc.cpp

DocOptionsPage.moc.cpp: $(srcdir)/DocOptionsPage.h
	$(MOC) $< -o DocOptionsPage.moc.cpp

DocOptionsPageData.moc.cpp: $(srcdir)/DocOptionsPageData.h
	$(MOC) $< -o DocOptionsPageData.moc.cpp

LangPropPage.moc.cpp: $(srcdir)/LangPropPage.h
	$(MOC) $< -o LangPropPage.moc.cpp

LangPropPageData.moc.cpp: $(srcdir)/LangPropPageData.h
	$(MOC) $< -o LangPropPageData.moc.cpp

DocPropDlg.moc.cpp: $(srcdir)/DocPropDlg.h
	$(MOC) $< -o DocPropDlg.moc.cpp

DocPropLangDlg.moc.cpp: $(srcdir)/DocPropLangDlg.h
	$(MOC) $< -o DocPropLangDlg.moc.cpp

clean-metasources:
	-rm -f $(libdocpropdlg_a_METASOURCES)  *.moc.*


release:
