kde4_header()

add_subdirectory( common-dialogs ) 
add_subdirectory( query-dialogs ) 
add_subdirectory( docprop-dialogs ) 
add_subdirectory( entry-dialogs ) 
add_subdirectory( statistik-dialogs ) 
add_subdirectory( kvt-core ) 
add_subdirectory( examples ) 
add_subdirectory( pics ) 

include_directories( ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/kvt-core/ ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############
kde4_add_dcop_skels(libcommondlg_SKEL_SRCS
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/languagesettings.h
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/presettings.h
    ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/common-dialogs/prefs.h
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

set(kvoctrain_SRCS ${libcommondlg_SRCS}
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

target_link_libraries(kvoctrain  ${QT_AND_KDECORE_LIBS} )

install_targets(/bin kvoctrain )


########### next target ###############

set(spotlight2kvtml_SRCS spotlight2kvtml.cpp )

kde4_automoc(${spotlight2kvtml_SRCS})

kde4_add_executable(spotlight2kvtml ${spotlight2kvtml_SRCS})

target_link_libraries(spotlight2kvtml  ${QT_AND_KDECORE_LIBS} )

install_targets(/bin spotlight2kvtml )


########### install files ###############

install_files( ${XDG_APPS_DIR} FILES kvoctrain.desktop )
install_files( ${CONFIG_INSTALL_DIR} FILES kvoctrainrc )
install_files( ${DATA_INSTALL_DIR}/kvoctrain FILES kvoctrainui.rc )

kde4_footer()



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
