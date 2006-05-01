METASOURCES = AUTO

kvoctrain_SOURCES = kva_io.cpp kvoctrain.cpp kvoctrainview.cpp main.cpp \
		kva_init.cpp kva_config.cpp kva_header.cpp kva_clip.cpp kva_query.cpp \
		kvoctraintable.cpp kvoctraintableitem.cpp kvtnewstuff.cpp

spotlight2kvtml_SOURCES = spotlight2kvtml.cpp

spotlight2kvtml_LDADD =  $(LIB_KDECORE)

kvoctrain_LDADD = ../../libkdeedu/kdeeducore/libkdeeducore.la ./docprop-dialogs/libdocpropdlg.la \
	./entry-dialogs/libentrydlg.la ./kvt-core/libkvoctraincore.la \
	./query-dialogs/libquerydlg.la ./statistik-dialogs/libstatdlg.la ./common-dialogs/libcommondlg.la \
	-lknewstuff $(LIB_KDEPRINT)

EXTRA_DIST = kvoctrainui.rc

SUBDIRS = common-dialogs query-dialogs docprop-dialogs entry-dialogs \
	statistik-dialogs kvt-core examples pics



rcdir = $(kde_datadir)/kvoctrain
rc_DATA = kvoctrainui.rc
kde_conf_DATA = kvoctrainrc
xdg_apps_DATA = kvoctrain.desktop


INCLUDES = -I$(top_builddir)/libkdeedu/kdeeducore \
	-I$(srcdir)/../../libkdeedu/kdeeducore -I$(srcdir)/.. -I$(srcdir)/kvt-core -Icommon-dialogs \
	-I$(srcdir)/common-dialogs -I$(srcdir)/docprop-dialogs -Idocprop-dialogs -I$(srcdir)/query-dialogs \
	-Iquery-dialogs -I$(srcdir)/entry-dialogs -Ientry-dialogs $(all_includes)

bin_PROGRAMS = kvoctrain spotlight2kvtml

# the library search path.
kvoctrain_LDFLAGS = $(all_libraries) $(KDE_RPATH)
spotlight2kvtml_LDFLAGS = $(all_libraries) $(KDE_RPATH)

CLEANFILES = *.i.c


noinst_HEADERS = kvtnewstuff.h version.h
