INCLUDES = $(all_includes)  -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core

noinst_LIBRARIES = libqueryoptiondlg.a

libqueryoptiondlg_a_DLG_SOURCES = \
  GroupOptPage.dlg \
  QueryOptPage.dlg \
  ThreshOptPage.dlg \
  BlockOptPage.dlg

libqueryoptiondlg_a_QOBJ_SOURCES =   \
  $(libqueryoptiondlg_a_DLG_SOURCES:.dlg=.cpp) \
  $(libqueryoptiondlg_a_DLG_SOURCES:.dlg=Data.cpp) \
 QueryOptionsDlg.cpp

libqueryoptiondlg_a_LIBADD = $(libqueryoptiondlg_a_SOURCES:.cpp=.o)
libqueryoptiondlg_a_SOURCES = $(libqueryoptiondlg_a_QOBJ_SOURCES) \
                              $(libqueryoptiondlg_a_QOBJ_SOURCES:.cpp=.moc.cpp)
                        
EXTRA_DIST = \
  $(libqueryoptiondlg_a_QOBJ_SOURCES:.cpp=.h) \
  $(libqueryoptiondlg_a_DLG_SOURCES)

CLEANFILES += *.rpo  *~

%Data.cpp %Data.h: %.dlg
	-qtarch -g $<


doc:


messages:


GroupOptPage.moc.cpp: $(srcdir)/GroupOptPage.h
	$(MOC) $< -o GroupOptPage.moc.cpp

GroupOptPageData.moc.cpp: $(srcdir)/GroupOptPageData.h
	$(MOC) $< -o GroupOptPageData.moc.cpp

QueryOptPage.moc.cpp: $(srcdir)/QueryOptPage.h
	$(MOC) $< -o QueryOptPage.moc.cpp

QueryOptPageData.moc.cpp: $(srcdir)/QueryOptPageData.h
	$(MOC) $< -o QueryOptPageData.moc.cpp

ThreshOptPage.moc.cpp: $(srcdir)/ThreshOptPage.h
	$(MOC) $< -o ThreshOptPage.moc.cpp

ThreshOptPageData.moc.cpp: $(srcdir)/ThreshOptPageData.h
	$(MOC) $< -o ThreshOptPageData.moc.cpp

BlockOptPage.moc.cpp: $(srcdir)/BlockOptPage.h
	$(MOC) $< -o BlockOptPage.moc.cpp

BlockOptPageData.moc.cpp: $(srcdir)/BlockOptPageData.h
	$(MOC) $< -o BlockOptPageData.moc.cpp

QueryOptionsDlg.moc.cpp: $(srcdir)/QueryOptionsDlg.h
	$(MOC) $< -o QueryOptionsDlg.moc.cpp

clean-metasources:
	-rm -f $(libqueryoptiondlg_a_METASOURCES) *.moc.*

release: 
