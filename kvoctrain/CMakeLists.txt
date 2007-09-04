
add_subdirectory( common-dialogs )
add_subdirectory( examples )
add_subdirectory( pics )

include_directories( ${CMAKE_SOURCE_DIR}/libkdeedu/keduvocdocument/
                     ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/docprop-dialogs/
                     ${CMAKE_SOURCE_DIR}/kvoctrain/kvoctrain/entry-dialogs/
)


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
    common-dialogs/languageoptions.cpp
    common-dialogs/queryoptions.cpp
    common-dialogs/thresholdoptions.cpp
    common-dialogs/profilesdialog.cpp
    common-dialogs/kvtlanguagemodel.cpp
    common-dialogs/kvtlanguageview.cpp
)

kde4_add_ui_files( libcommondlg_SRCS
    common-dialogs/blockoptionsbase.ui
    common-dialogs/groupoptionsbase.ui
    common-dialogs/languageoptionsbase.ui
    common-dialogs/queryoptionsbase.ui
    common-dialogs/thresholdoptionsbase.ui
    common-dialogs/viewoptionsbase.ui
    common-dialogs/generaloptionsbase.ui
)

set(querydialogs_SRCS
    query-dialogs/kvtquerymanager.cpp
    query-dialogs/kvtquery.cpp
    query-dialogs/AdjQueryDlg.cpp
    query-dialogs/ArtQueryDlg.cpp
    query-dialogs/MCQueryDlg.cpp
    query-dialogs/QueryDlgBase.cpp
    query-dialogs/RandomQueryDlg.cpp
    query-dialogs/SimpleQueryDlg.cpp
    query-dialogs/VerbQueryDlg.cpp
)

kde4_add_ui_files(querydialogs_SRCS
    query-dialogs/AdjQueryDlgForm.ui
    query-dialogs/ArtQueryDlgForm.ui
    query-dialogs/MCQueryDlgForm.ui
    query-dialogs/RandomQueryDlgForm.ui
    query-dialogs/SimpleQueryDlgForm.ui
    query-dialogs/VerbQueryDlgForm.ui
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
    entry-dialogs/AuxInfoEntryPage.cpp
    entry-dialogs/CommonEntryPage.cpp
    entry-dialogs/EntryDlg.cpp
    entry-dialogs/FromToEntryPage.cpp
    entry-dialogs/MCEntryPage.cpp
    entry-dialogs/PhoneticEntryPage.cpp
    entry-dialogs/TenseEntryPage.cpp
)

kde4_add_ui_files(entrydialogs_SRCS
    entry-dialogs/AdjEntryPageForm.ui
    entry-dialogs/AuxInfoEntryPageForm.ui
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
    newdocument-wizard/kvtnewdocumentwizardfinalpage.cpp
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

set(kvoctrain_SRCS ${libcommondlg_SRCS} ${querydialogs_SRCS} ${docprop_SRCS}
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


kde4_add_executable(kvoctrain ${kvoctrain_SRCS})

target_link_libraries(kvoctrain  keduvocdocument ${KDE4_KDEPRINT_LIBS} ${KDE4_KNEWSTUFF2_LIBS} )

install(TARGETS kvoctrain  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES kvoctrain.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES kvoctrain.knsrc  DESTINATION  ${CONFIG_INSTALL_DIR} )
install( FILES kvoctrainui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kvoctrain )

