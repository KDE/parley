#########################################################
# this 10 paths are KDE specific. Use them:
# kde_htmldir       Where your docs should go to. (contains lang subdirs)
# kde_appsdir       Where your application file (.kdelnk) should go to. 
# kde_icondir       Where your icon should go to.
# kde_minidir       Where your mini icon should go to.
# kde_datadir       Where you install application data. (Use a subdir)
# kde_locale        Where translation files should go to.(contains lang subdirs)
# kde_cgidir        Where cgi-bin executables should go to.
# kde_confdir       Where config files should go to.
# kde_mimedir       Where mimetypes should go to.
# kde_toolbardir    Where general toolbar icons should go to.
# kde_wallpaperdir  Where general wallpapers should go to.

kvoctrain_QOBJ_SOURCES =  \
      header.cpp kvoctrain.cpp \
      kvoctrainview.cpp \
      rowtable.cpp 


kvoctrain_SOURCES = \
 $(kvoctrain_QOBJ_SOURCES:.cpp=.moc.cpp)  \
 $(kvoctrain_QOBJ_SOURCES)  \
 main.cpp \
 kva_init.cpp kva_config.cpp kva_header.cpp \
 kva_io.cpp kva_clip.cpp kva_query.cpp

spotlight2kvtml_SOURCES = spotlight2kvtml.cpp 

spotlight2kvtml_LDADD = $(LIB_QT) -lkdecore -ldl

kvoctrain_LDADD +=   -lkfile -lkdeui -lkdecore -ldl -lkhtml -lkjava \
                    ./docprop-dialogs/libdocpropdlg.a \
                    ./entry-dialogs/libentrydlg.a \
                    ./kvt-core/libkvoctraincore.a \
                    ./option-dialogs/liboptiondlg.a \
                    ./query-dialogs/libquerydlg.a \
                    ./queryoption-dialogs/libqueryoptiondlg.a \
                    ./statistik-dialogs/libstatdlg.a \
                    ./common-dialogs/libcommondlg.a \
                     $(LIB_QT) -lXext -lX11 $(LIBSOCKET)

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
 
spotlight2kvtml_HEADER =


kvoctrain_HEADER = \
  $(kvoctrain_QOBJ_SOURCES:.cpp=.h)


EXTRA_DIST = $(APPS_FILES) $(kvoctrain_HEADER)\
 kvoctrain.desktop x-kvtml.desktop \
 kvoctrain.lsm.tpl \
 pics/kvoctrain.xpm pics/16x16-kvoctrain.xpm pics/22x22-kvoctrain.xpm \
 pics/32x32-kvoctrain.xpm pics/48x48-kvoctrain.xpm \
 sample.kvtml sample-de.kvtml sample-en.kvtml kvoctrain.dtd \
 langen2kvtml


refresh-dlg:
	find . -name *.dlg  -exec touch {} \;

install-data-local:
	$(mkinstalldirs) $(kde_appsdir)/Applications/
	$(INSTALL_DATA) kvoctrain.desktop $(kde_appsdir)/Applications/kvoctrain.desktop
	$(mkinstalldirs) $(kde_icondir)/
	$(INSTALL_DATA) pics/kvoctrain.xpm $(kde_icondir)/kvoctrain.xpm
	$(INSTALL_DATA) pics/48x48-kvoctrain.xpm $(kde_icondir)/hicolor/48x48/apps/kvoctrain.xpm
	$(INSTALL_DATA) pics/32x32-kvoctrain.xpm $(kde_icondir)/hicolor/32x32/apps/kvoctrain.xpm
	$(INSTALL_DATA) pics/22x22-kvoctrain.xpm $(kde_icondir)/hicolor/22x22/apps/kvoctrain.xpm
	$(INSTALL_DATA) pics/16x16-kvoctrain.xpm $(kde_icondir)/locolor/16x16/apps/kvoctrain.xpm
	$(mkinstalldirs) $(kde_mimedir)/application
	$(mkinstalldirs) $(kde_datadir)/kvoctrain
	for pic in $(APPS_FILES); do \
          file=`echo $$pic | sed 's,.*/,,'`; \
          $(INSTALL_DATA)  "pics/$$file" "$(kde_datadir)/kvoctrain/$$file"; \
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
	-rm -f $(kde_mimedir)/application/x-kvtml.desktop
	-rm -f $(kde_icondir)/large/kvoctrain.xpm
	-rm -f $(kde_icondir)/mini/kvoctrain.xpm
	for pic in $(APPS_FILES); do \
          file=`echo $$pic | sed 's,.*/,,'`; \
          rm -f "$(kde_datadir)/kvoctrain/$$file"; \
	done

# set the include path for X, qt and KDE
INCLUDES= $(all_includes)  -I$(top_srcdir)/kvoctrain/kvoctrain -I$(top_srcdir)/kvoctrain/kvoctrain/kvt-core
# claim, which subdirectories you want to install
# you can add here more. This one gets installed 
bin_PROGRAMS = kvoctrain  spotlight2kvtml

# the library search path. 
kvoctrain_LDFLAGS = $(all_libraries) $(KDE_RPATH)
spotlight2kvtml_LDFLAGS = $(all_libraries) $(KDE_RPATH)

CLEANFILES += *.i *.rpo *~

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

# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs
messages:
	cpp -P -I.. -DVERSION=\"$(VERSION)\" -D__ONLY_TO_BE_SEEN_BY_XGETTEXT kvoctrain.cpp >kvoctrain.i
	cpp -P -I.. -DVERSION=\"$(VERSION)\" -D__ONLY_TO_BE_SEEN_BY_XGETTEXT kv_resource.h >kv_resource.i
	cpp -P -I.. -DVERSION=\"$(VERSION)\" -D__ONLY_TO_BE_SEEN_BY_XGETTEXT kva_header.cpp >kva_header.i
	cpp -P -I.. -DVERSION=\"$(VERSION)\" -D__ONLY_TO_BE_SEEN_BY_XGETTEXT kvoctrainexpr.h >kvoctrainexpr.i
	cpp -P -I.. -DVERSION=\"$(VERSION)\" -D__ONLY_TO_BE_SEEN_BY_XGETTEXT option-dialogs/GeneralOptionsDlg.h >GeneralOptionsDlg.i
	cpp -P -I.. -DVERSION=\"$(VERSION)\" -D__ONLY_TO_BE_SEEN_BY_XGETTEXT entry-dialogs/EntryDlg.h >EntryDlg.i
	cpp -P -I.. -DVERSION=\"$(VERSION)\" -D__ONLY_TO_BE_SEEN_BY_XGETTEXT docprop-dialogs/DocPropDlg.h >DocPropDlg.i
	$(XGETTEXT) -C -ki18n -ki18n_noop -x $(includedir)/kde.pot `ls $(i18n_files)` && mv messages.po ../po/kvoctrain.pot
	-rm *.i

doc:
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || (cd $$subdir && $(MAKE) doc); \
	done

#-----------------------------------------------------------

# avoid exhausted mem ;-((  FIXME
kvd_rb_kvtml2.o: kvd_rb_kvtml2.cpp
	$(CXXCOMPILE) -O1 -Wall -c $<

#-----------------------------------------------------------

header.moc.cpp: $(srcdir)/header.h
	$(MOC) $< -o header.moc.cpp

kvoctrain.moc.cpp: $(srcdir)/kvoctrain.h
	$(MOC) $< -o kvoctrain.moc.cpp

kvoctrainview.moc.cpp: $(srcdir)/kvoctrainview.h
	$(MOC) $< -o kvoctrainview.moc.cpp

rowtable.moc.cpp: $(srcdir)/rowtable.h
	$(MOC) $< -o rowtable.moc.cpp

release:


clean-metasources:
	-rm -f $(kvoctrain_METASOURCES) *.moc.*
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || (cd $$subdir && $(MAKE) clean-metasources); \
	done

