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

METASOURCES = AUTO

