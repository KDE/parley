INCLUDES = $(all_includes) -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core

noinst_LIBRARIES = libcommondlg.a

libcommondlg_a_UI_SOURCES = \
   NumLessonDlg.ui \
   ProgressDlg.ui \
   LessonInputDlg.ui \
   CharSetDlg.ui

libcommondlg_a_QOBJ_SOURCES =   \
   $(libcommondlg_a_UI_SOURCES:.ui=Form.cpp) \
   $(libcommondlg_a_UI_SOURCES:.ui=.cpp) \
   aboutdialog.cpp

libcommondlg_a_SOURCES = $(libcommondlg_a_QOBJ_SOURCES) $(libcommondlg_a_QOBJ_SOURCES:.cpp=.moc.cpp)
libcommondlg_a_LIBADD= $(libcommondlg_a_SOURCES:.cpp=.o)

EXTRA_DIST = \
   $(libcommondlg_a_UI_SOURCES) \
   $(libcommondlg_a_QOBJ_SOURCES:.cpp=.h)

%Form.h: %.ui
	-$(UIC) -o $@ $<

%Form.cpp: %.ui %Form.h
	-$(UIC) -o $@ -i ${@:.cpp=.h} $<

CLEANFILES += *~

METASOURCES = AUTO
