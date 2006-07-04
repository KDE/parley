
add_subdirectory( common-dialogs ) 
add_subdirectory( query-dialogs ) 
add_subdirectory( docprop-dialogs ) 
add_subdirectory( entry-dialogs ) 
add_subdirectory( statistik-dialogs ) 
add_subdirectory( kvt-core ) 
add_subdirectory( examples ) 
add_subdirectory( pics ) 

include_directories( ${CMAKE_SOURCE_DIR}/libkdeedu/kdeeducore/ ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/kvt-core/ ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/docprop-dialogs/ ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/entry-dialogs/ )


########### next target ###############
kde4_add_kcfg_files(libcommondlg_SKEL_SRCS
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/languagesettings.kcfgc
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/presettings.kcfgc
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/prefs.kcfgc
)

set(libcommondlg_SRCS ${libcommondlg_SKEL_SRCS}
	${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/ProgressDlg.cpp 
	${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/kvoctrainprefs.cpp 
	${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/generaloptions.cpp 
	${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/viewoptions.cpp
	${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/pasteoptions.cpp 
	${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/blockoptions.cpp
	${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/groupoptions.cpp 
	${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/languageoptions.cpp 
	${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/queryoptions.cpp 
	${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/thresholdoptions.cpp
	${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/profilesdialog.cpp
	)

kde4_add_ui_files( libcommondlg_SRCS
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/blockoptionsbase.ui
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/groupoptionsbase.ui
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/languageoptionsbase.ui
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/queryoptionsbase.ui
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/thresholdoptionsbase.ui
	${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/pasteoptionsbase.ui
	${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/viewoptionsbase.ui
	${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/generaloptionsbase.ui
	${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/ProgressDlgForm.ui
)

set(querydialogs_SRCS
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/query-dialogs/AdjQueryDlg.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/query-dialogs/ArtQueryDlg.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/query-dialogs/MCQueryDlg.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/query-dialogs/QueryDlgBase.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/query-dialogs/RandomQueryDlg.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/query-dialogs/SimpleQueryDlg.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/query-dialogs/VerbQueryDlg.cpp
)

kde4_add_ui_files(querydialogs_SRCS
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/query-dialogs/AdjQueryDlgForm.ui
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/query-dialogs/ArtQueryDlgForm.ui
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/query-dialogs/MCQueryDlgForm.ui
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/query-dialogs/RandomQueryDlgForm.ui
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/query-dialogs/SimpleQueryDlgForm.ui
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/query-dialogs/VerbQueryDlgForm.ui
)

set(docprop_SRCS
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/docprop-dialogs/DocOptionsPage.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/docprop-dialogs/DocPropDlg.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/docprop-dialogs/DocPropLangDlg.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/docprop-dialogs/LangPropPage.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/docprop-dialogs/LessOptPage.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/docprop-dialogs/TenseOptPage.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/docprop-dialogs/TitlePage.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/docprop-dialogs/TypeOptPage.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/docprop-dialogs/UsageOptPage.cpp
)

kde4_add_ui_files(docprop_SRCS
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/docprop-dialogs/DocOptionsPageForm.ui
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/docprop-dialogs/LangPropPageForm.ui
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/docprop-dialogs/LessOptPageForm.ui
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/docprop-dialogs/TenseOptPageForm.ui
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/docprop-dialogs/TitlePageForm.ui
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/docprop-dialogs/TypeOptPageForm.ui
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/docprop-dialogs/UsageOptPageForm.ui
)

set(entrydialogs_SRCS
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/entry-dialogs/AdjEntryPage.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/entry-dialogs/AuxInfoEntryPage.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/entry-dialogs/blockall.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/entry-dialogs/CommonEntryPage.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/entry-dialogs/EntryDlg.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/entry-dialogs/FromToEntryPage.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/entry-dialogs/MCEntryPage.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/entry-dialogs/MySpinBox.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/entry-dialogs/PhoneticEntryPage.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/entry-dialogs/TenseEntryPage.cpp
)

kde4_add_ui_files(entrydialogs_SRCS
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/entry-dialogs/AdjEntryPageForm.ui
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/entry-dialogs/AuxInfoEntryPageForm.ui
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/entry-dialogs/CommonEntryPageForm.ui
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/entry-dialogs/FromToEntryPageForm.ui
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/entry-dialogs/MCEntryPageForm.ui
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/entry-dialogs/TenseEntryPageForm.ui
)

set(statistikdialogs_SRCS
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/statistik-dialogs/GenStatPage.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/statistik-dialogs/StatistikDlg.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/statistik-dialogs/StatistikPage.cpp
)

kde4_add_ui_files(statistikdialogs_SRCS
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/statistik-dialogs/GenStatPageForm.ui
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/statistik-dialogs/StatistikPageForm.ui
)

set(kvtcore_SRCS
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/kvt-core/langset.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/kvt-core/LineList.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/kvt-core/QueryManager.cpp
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/kvt-core/UsageManager.cpp
)

set(kvoctrain_SRCS ${libcommondlg_SRCS} ${querydialogs_SRCS} ${docprop_SRCS} ${entrydialogs_SRCS} ${statistikdialogs_SRCS} ${kvtcore_SRCS}
	kva_io.cpp 
	kvoctrain.cpp 
	kvoctrainview.cpp 
	main.cpp 
	kva_init.cpp 
	kva_config.cpp 
	kva_header.cpp 
	kva_clip.cpp 
	kva_query.cpp
	kvoctraintable.cpp 
	kvoctraintableitem.cpp 
	kvtnewstuff.cpp
)

kde4_automoc(${kvoctrain_SRCS})

kde4_add_executable(kvoctrain ${kvoctrain_SRCS})

target_link_libraries(kvoctrain  ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} kdeeducore kdeprint knewstuff )

install_targets(/bin kvoctrain )


########### next target ###############

set(spotlight2kvtml_SRCS spotlight2kvtml.cpp )

kde4_automoc(${spotlight2kvtml_SRCS})

kde4_add_executable(spotlight2kvtml ${spotlight2kvtml_SRCS})

target_link_libraries(spotlight2kvtml  ${KDE4_KDECORE_LIBS} )

install_targets(/bin spotlight2kvtml )


########### install files ###############

install( FILES kvoctrain.desktop  DESTINATION  ${XDG_APPS_DIR} )
install( FILES kvoctrainrc  DESTINATION  ${CONFIG_INSTALL_DIR} )
install( FILES kvoctrainui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kvoctrain )




#original Makefile.am contents follow:

#METASOURCES = AUTO
#
#kvoctrain_SOURCES = kva_io.cpp kvoctrain.cpp kvoctrainview.cpp main.cpp \
#		kva_init.cpp kva_config.cpp kva_header.cpp kva_clip.cpp kva_query.cpp \
#		kvoctraintable.cpp kvoctraintableitem.cpp kvtnewstuff.cpp
#
#spotlight2kvtml_SOURCES = spotlight2kvtml.cpp
#
#spotlight2kvtml_LDADD =  $(LIB_KDECORE)
#
#kvoctrain_LDADD = ../../libkdeedu/kdeeducore/libkdeeducore.la ./docprop-dialogs/libdocpropdlg.la \
#	./entry-dialogs/libentrydlg.la ./kvt-core/libkvoctraincore.la \
#	./query-dialogs/libquerydlg.la ./statistik-dialogs/libstatdlg.la ./common-dialogs/libcommondlg.la \
#	-lknewstuff $(LIB_KDEPRINT)
#
#EXTRA_DIST = kvoctrainui.rc
#
#SUBDIRS = common-dialogs query-dialogs docprop-dialogs entry-dialogs \
#	statistik-dialogs kvt-core examples pics
#
#
#
#rcdir = $(kde_datadir)/kvoctrain
#rc_DATA = kvoctrainui.rc
#kde_conf_DATA = kvoctrainrc
#xdg_apps_DATA = kvoctrain.desktop
#
#
#INCLUDES = -I$(top_builddir)/libkdeedu/kdeeducore \
#	-I$(srcdir)/../../libkdeedu/kdeeducore -I$(srcdir)/.. -I$(srcdir)/kvt-core -Icommon-dialogs \
#	-I$(srcdir)/common-dialogs -I$(srcdir)/docprop-dialogs -Idocprop-dialogs -I$(srcdir)/query-dialogs \
#	-Iquery-dialogs -I$(srcdir)/entry-dialogs -Ientry-dialogs $(all_includes)
#
#bin_PROGRAMS = kvoctrain spotlight2kvtml
#
## the library search path.
#kvoctrain_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#spotlight2kvtml_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#
#CLEANFILES = *.i.c
#
#i18n_files=  query-dialogs/*.cpp \
#             entry-dialogs/*.cpp \
#             common-dialogs/*.cpp \
#             docprop-dialogs/*.cpp \
#             statistik-dialogs/*.cpp \
#             kvt-core/*.cpp \
#             $(spotlight2kvtml_SOURCES) \
#             $(kvoctrain_SOURCES)
#
#messages: rc.cpp
#	$(EXTRACTRC) `find . -name "*.ui"` >> rc.cpp
#	$(XGETTEXT) `ls $(i18n_files)` rc.cpp -kI18N_NOOP2 -o $(podir)/kvoctrain.pot
#	-rm rc.cpp
#
#messages-local:
#	$(MAKE) messages EXTRACTRC=extractrc podir=. XGETTEXT="xgettext -C -kI18N_NOOP -ki18n"
#	iconv -futf-8 -tutf-16 kvoctrain.pot > /dev/null
#
#noinst_HEADERS = kvtnewstuff.h version.h
