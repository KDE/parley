
kvoctrain_QOBJ_SOURCES =  \
      header.cpp kvoctrain.cpp \
      kvoctrainview.cpp \
      rowtable.cpp 

METASOURCES = AUTO

kvoctrain_SOURCES = \
 $(kvoctrain_QOBJ_SOURCES)  \
 main.cpp kva_init.cpp kva_config.cpp kva_header.cpp \
 kva_io.cpp kva_clip.cpp kva_query.cpp

spotlight2kvtml_SOURCES = spotlight2kvtml.cpp 

spotlight2kvtml_LDADD = -lkdecore 

kvoctrain_LDADD =   -lkfile -lkhtml \
                    ./docprop-dialogs/libdocpropdlg.a \
                    ./entry-dialogs/libentrydlg.a \
                    ./kvt-core/libkvoctraincore.a \
                    ./kvt-core/kvt-xml/libkvtxml.a \
                    ./option-dialogs/liboptiondlg.a \
                    ./query-dialogs/libquerydlg.a \
                    ./queryoption-dialogs/libqueryoptiondlg.a \
                    ./statistik-dialogs/libstatdlg.a \
                    ./common-dialogs/libcommondlg.a 

SUBDIRS = common-dialogs query-dialogs queryoption-dialogs \
          option-dialogs entry-dialogs docprop-dialogs kvt-core \
          statistik-dialogs

APPS_FILES = pics/exit.xpm pics/fonts.xpm pics/kde.xpm pics/stop.xpm \
 pics/16x16-kvoctrain.xpm pics/new.xpm pics/lesson.xpm\
 pics/list-win.xpm pics/pron-win.xpm pics/contents.xpm \
 pics/delete-row.xpm pics/append-row.xpm pics/edit-row.xpm \
 pics/run.xpm pics/copy.xpm pics/open.xpm pics/save.xpm pics/help.xpm pics/paste.xpm \
 pics/conf-save.xpm pics/gen-conf.xpm pics/query-conf.xpm \
 pics/flags.xpm pics/reset.xpm  pics/statist.xpm \
 pics/delete-col.xpm pics/append-col.xpm \
 pics/searchclip.xpm pics/rand-less.xpm \
 pics/run-verb.xpm  pics/run-art.xpm  pics/run-adj.xpm \
 pics/run-query.xpm  pics/run-multi.xpm \
 pics/sort_alpha.xpm pics/sort_num.xpm pics/cleanup.xpm \
 pics/run-syno.xpm pics/run-anto.xpm  pics/run-exmp.xpm  pics/run-para.xpm \
 pics/sf-mail.xpm pics/sf-bugs.xpm pics/sf-home.xpm
 
refresh-dlg:
	find . -name *.dlg  -exec touch {} \;

install-data-local:
	$(mkinstalldirs) $(kde_appsdir)/Applications/
	$(INSTALL_DATA) $(srcdir)/kvoctrain.desktop $(kde_appsdir)/Applications/kvoctrain.desktop
	$(mkinstalldirs) $(kde_icondir)/
	$(INSTALL_DATA) $(srcdir)/pics/kvoctrain.xpm $(kde_icondir)/kvoctrain.xpm
	$(INSTALL_DATA) $(srcdir)/pics/48x48-kvoctrain.xpm $(kde_icondir)/hicolor/48x48/apps/kvoctrain.xpm
	$(INSTALL_DATA) $(srcdir)/pics/32x32-kvoctrain.xpm $(kde_icondir)/hicolor/32x32/apps/kvoctrain.xpm
	$(INSTALL_DATA) $(srcdir)/pics/22x22-kvoctrain.xpm $(kde_icondir)/hicolor/22x22/apps/kvoctrain.xpm
	$(INSTALL_DATA) $(srcdir)/pics/16x16-kvoctrain.xpm $(kde_icondir)/locolor/16x16/apps/kvoctrain.xpm
	$(mkinstalldirs) $(kde_datadir)/kvoctrain
	for pic in $(APPS_FILES); do \
          file=`echo $$pic | sed 's,.*/,,'`; \
          $(INSTALL_DATA) $(srcdir)/pics/$$file $(DESTDIR)$(kde_datadir)/kvoctrain/$$file; \
	done
	@echo ""
	@echo "***********************************************************"
	@echo "* Installed. Enjoy KVOCTRAIN                              *"
	@echo "* Please report bugs to: kvoctrain@ewald-arnold.de        *"
	@echo "***********************************************************"

uninstall-local:
	-rm -f $(kde_icondir)/kvoctrain.xpm
	-rm -f $(kde_minidir)/kvoctrain.xpm
	-rm -f $(kde_appsdir)/Applications/kvoctrain.desktop
	-rm -f $(kde_icondir)/large/kvoctrain.xpm
	-rm -f $(kde_icondir)/mini/kvoctrain.xpm
	for pic in $(APPS_FILES); do \
          file=`echo $$pic | sed 's,.*/,,'`; \
          rm -f "$(kde_datadir)/kvoctrain/$$file"; \
	done

# set the include path for X, qt and KDE
INCLUDES= -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core $(all_includes)
# claim, which subdirectories you want to install
# you can add here more. This one gets installed 
bin_PROGRAMS = kvoctrain spotlight2kvtml

# the library search path. 
kvoctrain_LDFLAGS = $(all_libraries) $(KDE_RPATH)
spotlight2kvtml_LDFLAGS = $(all_libraries) $(KDE_RPATH)

CLEANFILES = *.i *~

# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(kvoctrain_METASOURCES)

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
             *.i           \
             $(spotlight2kvtml_SOURCES) \
             $(kvoctrain_SOURCES)

messages:
	cpp -P -I.. -DVERSION=\"$(VERSION)\" -D__ONLY_TO_BE_SEEN_BY_XGETTEXT kvoctrain.cpp >kvoctrain.i
	cpp -P -I.. -DVERSION=\"$(VERSION)\" -D__ONLY_TO_BE_SEEN_BY_XGETTEXT kvt-core/kv_resource.h >kv_resource.i
	cpp -P -I.. -DVERSION=\"$(VERSION)\" -D__ONLY_TO_BE_SEEN_BY_XGETTEXT kva_header.cpp >kva_header.i
	cpp -P -I.. -DVERSION=\"$(VERSION)\" -D__ONLY_TO_BE_SEEN_BY_XGETTEXT kvt-core/kvoctrainexpr.h >kvoctrainexpr.i
	cpp -P -I.. -DVERSION=\"$(VERSION)\" -D__ONLY_TO_BE_SEEN_BY_XGETTEXT option-dialogs/GeneralOptionsDlg.h >GeneralOptionsDlg.i
	cpp -P -I.. -DVERSION=\"$(VERSION)\" -D__ONLY_TO_BE_SEEN_BY_XGETTEXT entry-dialogs/EntryDlg.h >EntryDlg.i
	cpp -P -I.. -DVERSION=\"$(VERSION)\" -D__ONLY_TO_BE_SEEN_BY_XGETTEXT docprop-dialogs/DocPropDlg.h >DocPropDlg.i
	cpp -P -I.. -DVERSION=\"$(VERSION)\" -D__ONLY_TO_BE_SEEN_BY_XGETTEXT kvt-core/kvoctrainexpr.h >kvoctrainexpr.i
	$(XGETTEXT) `ls $(i18n_files)` -o $(podir)/kvoctrain.pot
	-rm *.i

doc:
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || (cd $$subdir && $(MAKE) doc); \
	done

