INCLUDES = $(all_includes) -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core

noinst_LIBRARIES = libcommondlg.a

libcommondlg_a_DLG_SOURCES = \
   ProgressDlg.dlg \
   LessonInputDlg.dlg \
   NumLessonDlg.dlg \
   CharSetDlg.dlg

libcommondlg_a_QOBJ_SOURCES =   \
   $(libcommondlg_a_DLG_SOURCES:.dlg=.cpp) \
   $(libcommondlg_a_DLG_SOURCES:.dlg=Data.cpp) \
   aboutdialog.cpp

libcommondlg_a_SOURCES = $(libcommondlg_a_QOBJ_SOURCES) $(libcommondlg_a_QOBJ_SOURCES:.cpp=.moc.cpp)
libcommondlg_a_LIBADD= $(libcommondlg_a_SOURCES:.cpp=.o)

EXTRA_DIST = \
   $(libcommondlg_a_QOBJ_SOURCES:.cpp=.h) \
   $(libcommondlg_a_DLG_SOURCES)

%Data.cpp %Data.h: %.dlg
	-qtarch -g $<

CLEANFILES += *.rpo *~

doc:

messages:

aboutdialog.moc.cpp: $(srcdir)/aboutdialog.h
	$(MOC) $< -o aboutdialog.moc.cpp

LessonInputDlg.moc.cpp: $(srcdir)/LessonInputDlg.h
	$(MOC) $< -o LessonInputDlg.moc.cpp

LessonInputDlgData.moc.cpp: $(srcdir)/LessonInputDlgData.h
	$(MOC) $< -o LessonInputDlgData.moc.cpp

NumLessonDlg.moc.cpp: $(srcdir)/NumLessonDlg.h
	$(MOC) $< -o NumLessonDlg.moc.cpp

NumLessonDlgData.moc.cpp: $(srcdir)/NumLessonDlgData.h
	$(MOC) $< -o NumLessonDlgData.moc.cpp

CharSetDlg.moc.cpp: $(srcdir)/CharSetDlg.h
	$(MOC) $< -o CharSetDlg.moc.cpp

CharSetDlgData.moc.cpp: $(srcdir)/CharSetDlgData.h
	$(MOC) $< -o CharSetDlgData.moc.cpp

ProgressDlg.moc.cpp: $(srcdir)/ProgressDlg.h
	$(MOC) $< -o ProgressDlg.moc.cpp

ProgressDlgData.moc.cpp: $(srcdir)/ProgressDlgData.h
	$(MOC) $< -o ProgressDlgData.moc.cpp

clean-metasources:
	-rm  $(libcommondlg_a_METASOURCES) *.moc.*

release:
