METASOURCES = AUTO

kvoctrain_SOURCES = kva_io.cpp \
   kvoctrain.cpp  kvoctrainview.cpp rowtable.cpp \
   main.cpp kva_init.cpp kva_config.cpp kva_header.cpp \
   kva_clip.cpp kva_query.cpp

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

SUBDIRS = common-dialogs query-dialogs queryoption-dialogs \
          option-dialogs docprop-dialogs entry-dialogs  kvt-core \
          statistik-dialogs examples pics

KDE_ICON = kvoctrain

bin_SCRIPTS = langen2kvtml

apps_DATA = kvoctrain.desktop
appsdir = $(kde_appsdir)/Edutainment/Languages

INCLUDES= -I$(top_srcdir)/kvoctrain -I$(top_srcdir)/kvoctrain/kvt-core \
 -I$(top_srcdir)/kvoctrain/kvoctrain \
 -I$(top_builddir)/kvoctrain/kvoctrain/option-dialogs \
 -I$(top_builddir)/kvoctrain/kvoctrain/common-dialogs \
 -I$(top_builddir)/kvoctrain/kvoctrain/docprop-dialogs \
 -I$(top_srcdir)/kvoctrain/kvoctrain/queryoption-dialogs \
 -I$(top_builddir)/kvoctrain/kvoctrain/queryoption-dialogs \
 -I$(top_builddir)/kvoctrain/kvoctrain/query-dialogs \
 -I$(top_builddir)/kvoctrain/kvoctrain/entry-dialogs \
 -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core \
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
             entry-dialogs/*.cpp \
             option-dialogs/*.cpp \
             docprop-dialogs/*.cpp \
             statistik-dialogs/*.cpp \
             kvt-core/*.cpp \
             kvt-core/kvt-xml/*.cpp \
             $(spotlight2kvtml_SOURCES) \
             $(kvoctrain_SOURCES)

messages:
	$(EXTRACTRC) `find . -name "*.ui"` > rc.cpp
	$(XGETTEXT) `ls $(i18n_files)` rc.cpp -kI18N_NOOP2 -o $(podir)/kvoctrain.pot
	-rm rc.cpp

messages-local:
	$(MAKE) messages EXTRACTRC=extractrc podir=. XGETTEXT="xgettext -C -kI18N_NOOP -ki18n"
	iconv -futf-8 -tutf-16 kvoctrain.pot > /dev/null

