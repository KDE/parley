METASOURCES = AUTO

kvoctrain_SOURCES = kva_io.cpp kvoctrain.cpp kvoctrainview.cpp rowtable.cpp \
	main.cpp kva_init.cpp kva_config.cpp kva_header.cpp kva_clip.cpp kva_query.cpp \
	prefs.kcfgc

spotlight2kvtml_SOURCES = spotlight2kvtml.cpp 

spotlight2kvtml_LDADD =  $(LIB_KDECORE)

kvoctrain_LDADD =   ./docprop-dialogs/libdocpropdlg.la \
                    ./entry-dialogs/libentrydlg.la \
                    ./kvt-core/libkvoctraincore.la \
                    ./kvt-core/kvt-xml/libkvtxml.la \
                    ./option-dialogs/liboptiondlg.la \
                    ./query-dialogs/libquerydlg.la \
                    ./queryoption-dialogs/libqueryoptiondlg.la \
                    ./statistik-dialogs/libstatdlg.la \
                    ./common-dialogs/libcommondlg.la $(LIB_KHTML)

EXTRA_DIST = kvoctrainui.rc

SUBDIRS = common-dialogs query-dialogs queryoption-dialogs \
          option-dialogs docprop-dialogs entry-dialogs  kvt-core \
          statistik-dialogs examples pics

bin_SCRIPTS = langen2kvtml

rcdir = $(kde_datadir)/kvoctrain
rc_DATA = kvoctrainui.rc

xdg_apps_DATA = kvoctrain.desktop
kde_kcfg_DATA = kvoctrain.kcfg

INCLUDES= -I$(srcdir)/.. \
 -I$(srcdir)/kvt-core \
 -I$(srcdir)/option-dialogs \
 -Ioption-dialogs \
 -Icommon-dialogs \
 -I$(srcdir)/common-dialogs \
 -I$(srcdir)/docprop-dialogs \
 -Idocprop-dialogs \
 -I$(srcdir)/queryoption-dialogs \
 -Iqueryoption-dialogs \
 -I$(srcdir)/query-dialogs \
 -Iquery-dialogs \
 -I$(srcdir)/entry-dialogs \
 -Ientry-dialogs \
 $(all_includes)

bin_PROGRAMS = kvoctrain spotlight2kvtml

# the library search path. 
kvoctrain_LDFLAGS = $(all_libraries) $(KDE_RPATH)
spotlight2kvtml_LDFLAGS = $(all_libraries) $(KDE_RPATH)

CLEANFILES = *.i.c

i18n_files=  query-dialogs/*.cpp \
             entry-dialogs/*.cpp \
             common-dialogs/*.cpp \
             queryoption-dialogs/*.cpp \
             option-dialogs/*.cpp \
             docprop-dialogs/*.cpp \
             statistik-dialogs/*.cpp \
             kvt-core/*.cpp \
             kvt-core/kvt-xml/*.cpp \
             $(spotlight2kvtml_SOURCES) \
             $(kvoctrain_SOURCES)

messages: rc.cpp
	$(EXTRACTRC) `find . -name "*.ui"` >> rc.cpp
	$(XGETTEXT) `ls $(i18n_files)` rc.cpp -kI18N_NOOP2 -o $(podir)/kvoctrain.pot
	-rm rc.cpp

messages-local:
	$(MAKE) messages EXTRACTRC=extractrc podir=. XGETTEXT="xgettext -C -kI18N_NOOP -ki18n"
	iconv -futf-8 -tutf-16 kvoctrain.pot > /dev/null

