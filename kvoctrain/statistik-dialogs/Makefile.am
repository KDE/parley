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


doc:


messages:


StatistikPage.moc.cpp: $(srcdir)/StatistikPage.h
	$(MOC) $< -o StatistikPage.moc.cpp

StatistikPageData.moc.cpp: $(srcdir)/StatistikPageData.h
	$(MOC) $< -o StatistikPageData.moc.cpp

StatistikDlg.moc.cpp: $(srcdir)/StatistikDlg.h
	$(MOC) $< -o StatistikDlg.moc.cpp

GenStatPage.moc.cpp: $(srcdir)/GenStatPage.h
	$(MOC) $< -o GenStatPage.moc.cpp

GenStatPageData.moc.cpp: $(srcdir)/GenStatPageData.h
	$(MOC) $< -o GenStatPageData.moc.cpp

clean-metasources:
	-rm -f $(libstatdlg_a_METASOURCES) *.moc.*

release: 
