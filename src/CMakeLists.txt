
add_subdirectory( common-dialogs )
add_subdirectory( examples )
add_subdirectory( pics )

include_directories( ${CMAKE_SOURCE_DIR}/libkdeedu/keduvocdocument/)


########### next target ###############
kde4_add_kcfg_files( libcommondlg_SKEL_SRCS
    common-dialogs/languagesettings.kcfgc
    common-dialogs/presettings.kcfgc
    common-dialogs/prefs.kcfgc
)

set(libcommondlg_SRCS ${libcommondlg_SKEL_SRCS}
    common-dialogs/kvoctrainprefs.cpp
    common-dialogs/generaloptions.cpp
    common-dialogs/viewoptions.cpp
    common-dialogs/blockoptions.cpp
    common-dialogs/groupoptions.cpp
    common-dialogs/queryoptions.cpp
    common-dialogs/thresholdoptions.cpp
    common-dialogs/profilesdialog.cpp
)

kde4_add_ui_files( libcommondlg_SRCS
    common-dialogs/blockoptionsbase.ui
    common-dialogs/groupoptionsbase.ui
    common-dialogs/queryoptionsbase.ui
    common-dialogs/thresholdoptionsbase.ui
    common-dialogs/viewoptionsbase.ui
    common-dialogs/generaloptionsbase.ui
)

set(practice_SRCS
    practice/testentry.cpp
    practice/kvtquerymanager.cpp
    practice/kvtquery.cpp
    practice/practicedialog.cpp
    practice/AdjQueryDlg.cpp
    practice/ArtQueryDlg.cpp
    practice/MCQueryDlg.cpp
    practice/RandomQueryDlg.cpp
    practice/SimpleQueryDlg.cpp
    practice/VerbQueryDlg.cpp
    practice/startpracticedialog.cpp
    practice/startpracticewidget.cpp
)

kde4_add_ui_files(practice_SRCS
    practice/AdjQueryDlgForm.ui
    practice/ArtQueryDlgForm.ui
    practice/MCQueryDlgForm.ui
    practice/RandomQueryDlgForm.ui
    practice/SimpleQueryDlgForm.ui
    practice/VerbQueryDlgForm.ui
    practice/startpracticewidget.ui
)

set(docprop_SRCS
    docprop-dialogs/DocPropDlg.cpp
    docprop-dialogs/DocPropLangDlg.cpp
    docprop-dialogs/LangPropPage.cpp
    docprop-dialogs/TenseOptPage.cpp
    docprop-dialogs/TitlePage.cpp
    docprop-dialogs/TypeOptPage.cpp
    docprop-dialogs/UsageOptPage.cpp
)

kde4_add_ui_files(docprop_SRCS
    docprop-dialogs/LangPropPageForm.ui
    docprop-dialogs/TitlePageForm.ui
    docprop-dialogs/optionlistform.ui
    docprop-dialogs/typeeditform.ui
)

set(entrydialogs_SRCS
    entry-dialogs/AdjEntryPage.cpp
    entry-dialogs/additionaleditpage.cpp
    entry-dialogs/CommonEntryPage.cpp
    entry-dialogs/EntryDlg.cpp
    entry-dialogs/FromToEntryPage.cpp
    entry-dialogs/MCEntryPage.cpp
    entry-dialogs/PhoneticEntryPage.cpp
    entry-dialogs/TenseEntryPage.cpp
)

kde4_add_ui_files(entrydialogs_SRCS
    entry-dialogs/AdjEntryPageForm.ui
    entry-dialogs/additionaleditpage.ui
    entry-dialogs/CommonEntryPageForm.ui
    entry-dialogs/FromToEntryPageForm.ui
    entry-dialogs/MCEntryPageForm.ui
    entry-dialogs/TenseEntryPageForm.ui
)

set(statisticsdialogs_SRCS
    statistics-dialogs/GenStatPage.cpp
    statistics-dialogs/StatisticsDialog.cpp
    statistics-dialogs/StatisticsPage.cpp
)

kde4_add_ui_files(statisticsdialogs_SRCS
    statistics-dialogs/GenStatPageForm.ui
    statistics-dialogs/StatisticsPageForm.ui
)

set(newdocument-wizard_SRCS
    newdocument-wizard/kvtnewdocumentwizard.cpp
    newdocument-wizard/kvtnewdocumentwizardintropage.cpp
    newdocument-wizard/authortitlepage.cpp
    newdocument-wizard/firstidentifierlanguagepage.cpp
    newdocument-wizard/secondidentifierlanguagepage.cpp
)

kde4_add_ui_files(newdocument-wizard_SRCS
    newdocument-wizard/identifierlanguagepage.ui
    newdocument-wizard/secondidentifierlanguagepage.ui
)

set(languagedialogs_SRCS
    language-dialogs/languagedialog.cpp
    language-dialogs/editlanguagedialogpage.cpp
)

kde4_add_ui_files(languagedialogs_SRCS
    language-dialogs/languagepage.ui
)

set(parley_SRCS ${libcommondlg_SRCS} ${practice_SRCS} ${docprop_SRCS}
${entrydialogs_SRCS} ${statisticsdialogs_SRCS} ${languagedialogs_SRCS} ${newdocument-wizard_SRCS}
    kva_io.cpp
    kvoctrain.cpp
    main.cpp
    kva_init.cpp
    kvttabledelegate.cpp
    kvttablemodel.cpp
    kvttableview.cpp
    kvtlessonmodel.cpp
    kvtlessonview.cpp
    kvtsortfiltermodel.cpp
)


kde4_add_executable(parley ${parley_SRCS})

target_link_libraries(parley  keduvocdocument ${KDE4_KDEPRINT_LIBS} ${KDE4_KNEWSTUFF2_LIBS}  ${KDE4_PHONON_LIBS})

install(TARGETS parley  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES parley.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES parley.knsrc  DESTINATION  ${CONFIG_INSTALL_DIR} )
install( FILES parleyui.rc  DESTINATION  ${DATA_INSTALL_DIR}/parley )

