INCLUDES = $(all_includes)  -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core

noinst_LIBRARIES = libquerydlg.a


libquerydlg_a_DLG_SOURCES = \
  VerbQueryDlg.dlg \
  AdjQueryDlg.dlg \
  ArtQueryDlg.dlg \
  QueryDlg.dlg \
  MCQueryDlg.dlg \
  SimpleQueryDlg.dlg


libquerydlg_a_LIBADD  = $(libquerydlg_a_SOURCES:.cpp=.o)
libquerydlg_a_QOBJ_SOURCES =   \
  $(libquerydlg_a_DLG_SOURCES:.dlg=.cpp) \
  $(libquerydlg_a_DLG_SOURCES:.dlg=Data.cpp) \
  QueryDlgBase.cpp

libquerydlg_a_SOURCES = $(libquerydlg_a_QOBJ_SOURCES) \
                        $(libquerydlg_a_QOBJ_SOURCES:.cpp=.moc.cpp) \
                        MyProgress.cpp
                        
EXTRA_DIST = \
  $(libquerydlg_a_QOBJ_SOURCES:.cpp=.h) \
  $(libquerydlg_a_DLG_SOURCES) \
  MyProgress.h


CLEANFILES += *.rpo  *~


%Data.cpp %Data.h: %.dlg
	-qtarch -g $<

doc:


messages:

QueryDlgBase.moc.cpp: $(srcdir)/QueryDlgBase.h
	$(MOC) $< -o QueryDlgBase.moc.cpp

QueryDlg.moc.cpp: $(srcdir)/QueryDlg.h
	$(MOC) $< -o QueryDlg.moc.cpp

QueryDlgData.moc.cpp: $(srcdir)/QueryDlgData.h
	$(MOC) $< -o QueryDlgData.moc.cpp

MCQueryDlg.moc.cpp: $(srcdir)/MCQueryDlg.h
	$(MOC) $< -o MCQueryDlg.moc.cpp

MCQueryDlgData.moc.cpp: $(srcdir)/MCQueryDlgData.h
	$(MOC) $< -o MCQueryDlgData.moc.cpp

VerbQueryDlg.moc.cpp: $(srcdir)/VerbQueryDlg.h
	$(MOC) $< -o VerbQueryDlg.moc.cpp

VerbQueryDlgData.moc.cpp: $(srcdir)/VerbQueryDlgData.h
	$(MOC) $< -o VerbQueryDlgData.moc.cpp

SimpleQueryDlg.moc.cpp: $(srcdir)/SimpleQueryDlg.h
	$(MOC) $< -o SimpleQueryDlg.moc.cpp

SimpleQueryDlgData.moc.cpp: $(srcdir)/SimpleQueryDlgData.h
	$(MOC) $< -o SimpleQueryDlgData.moc.cpp

ArtQueryDlg.moc.cpp: $(srcdir)/ArtQueryDlg.h
	$(MOC) $< -o ArtQueryDlg.moc.cpp

ArtQueryDlgData.moc.cpp: $(srcdir)/ArtQueryDlgData.h
	$(MOC) $< -o ArtQueryDlgData.moc.cpp

AdjQueryDlg.moc.cpp: $(srcdir)/AdjQueryDlg.h
	$(MOC) $< -o AdjQueryDlg.moc.cpp

AdjQueryDlgData.moc.cpp: $(srcdir)/AdjQueryDlgData.h
	$(MOC) $< -o AdjQueryDlgData.moc.cpp

clean-metasources:
	-rm -f $(libquerydlg_a_METASOURCES) *.moc.*

release: 
