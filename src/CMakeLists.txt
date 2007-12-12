
add_subdirectory( settings )

include_directories( ${CMAKE_SOURCE_DIR}/libkdeedu/keduvocdocument/)

########### next target ###############
kde4_add_kcfg_files( libcommondlg_SKEL_SRCS
    settings/prefs.kcfgc
    settings/documentsettings.kcfgc
    settings/languagesettings.kcfgc
)

set(libcommondlg_SRCS ${libcommondlg_SKEL_SRCS}
    settings/parleyprefs.cpp
    settings/generaloptions.cpp
    settings/viewoptions.cpp
)

kde4_add_ui_files( libcommondlg_SRCS
    settings/viewoptionsbase.ui
    settings/generaloptionsbase.ui
)

set(vocabulary_SRCS
    vocabulary/vocabularymodel.cpp
    vocabulary/vocabularyview.cpp
    vocabulary/vocabularydelegate.cpp
    vocabulary/lessonmodel.cpp
    vocabulary/lessonview.cpp
)

set(practice_SRCS
    practice/testentry.cpp
    practice/testentrymanager.cpp
    practice/practicedialog.cpp
    practice/AdjQueryDlg.cpp
    practice/ArtQueryDlg.cpp
    practice/MCQueryDlg.cpp
    practice/writtenpracticedialog.cpp
    practice/SimpleQueryDlg.cpp
    practice/VerbQueryDlg.cpp
    practice/mixedletterpracticedialog.cpp
    practice/showsolutiondialog.cpp
    practice/practicesummarydialog.cpp
    practice/answervalidator.cpp
)

kde4_add_ui_files(practice_SRCS
    practice/AdjQueryDlgForm.ui
    practice/ArtQueryDlgForm.ui
    practice/MCQueryDlgForm.ui
    practice/writtenpracticedialog.ui
    practice/SimpleQueryDlgForm.ui
    practice/VerbQueryDlgForm.ui
    practice/mixedletterpracticedialog.ui
    practice/showsolutionwidget.ui
    practice/practicesummarywidget.ui
)

set(configure-practice_SRCS
    configure-practice/configurepracticedialog.cpp
    configure-practice/configurepracticewidget.cpp
    configure-practice/advancedpracticeoptions.cpp
    configure-practice/blockoptions.cpp
    configure-practice/thresholdoptions.cpp
)

kde4_add_ui_files(configure-practice_SRCS
    configure-practice/thresholdoptions.ui
    configure-practice/advancedpracticeoptions.ui
    configure-practice/blockoptions.ui
    configure-practice/configurepracticewidget.ui
    configure-practice/comparisonoptionswidget.ui
    configure-practice/conjugationoptionswidget.ui
    configure-practice/writtenpracticeoptionswidget.ui
)

set(docprop_SRCS
    docprop-dialogs/LangPropPage.cpp
    docprop-dialogs/TenseOptPage.cpp
    docprop-dialogs/TitlePage.cpp
#     docprop-dialogs/TypeOptPage.cpp
   # docprop-dialogs/UsageOptPage.cpp
)

kde4_add_ui_files(docprop_SRCS
    docprop-dialogs/LangPropPageForm.ui
    docprop-dialogs/TitlePageForm.ui
    docprop-dialogs/optionlistform.ui
    docprop-dialogs/typeeditform.ui
)

set(entrydialogs_SRCS
    entry-dialogs/AdjEntryPage.cpp
  #  entry-dialogs/additionaleditpage.cpp
  #  entry-dialogs/CommonEntryPage.cpp
  #  entry-dialogs/EntryDlg.cpp
  #  entry-dialogs/FromToEntryPage.cpp
    entry-dialogs/MCEntryPage.cpp
  #  entry-dialogs/PhoneticEntryPage.cpp
  #  entry-dialogs/TenseEntryPage.cpp
  #  entry-dialogs/wordtypewidget.cpp
    entry-dialogs/comparisonwidget.cpp
    entry-dialogs/imagechooserwidget.cpp
    entry-dialogs/audiowidget.cpp
)

kde4_add_ui_files(entrydialogs_SRCS
    entry-dialogs/AdjEntryPageForm.ui
    entry-dialogs/additionaleditpage.ui
    entry-dialogs/CommonEntryPageForm.ui
    entry-dialogs/FromToEntryPageForm.ui
#     entry-dialogs/MCEntryPageForm.ui
    entry-dialogs/TenseEntryPageForm.ui
    entry-dialogs/wordtypewidget.ui
    entry-dialogs/multiplechoicewidget.ui
    entry-dialogs/comparisonwidget.ui
    entry-dialogs/imagechooserwidget.ui
    entry-dialogs/audiowidget.ui
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
    language-dialogs/grammardialog.cpp
)

kde4_add_ui_files(languagedialogs_SRCS
    language-dialogs/languagepage.ui
)

set(parley_SRCS
    ${vocabulary_SRCS} ${libcommondlg_SRCS} ${practice_SRCS}
    ${configure-practice_SRCS} ${docprop_SRCS}
    ${entrydialogs_SRCS} ${statisticsdialogs_SRCS}
    ${languagedialogs_SRCS} ${newdocument-wizard_SRCS}
    parleydocument.cpp
    parley.cpp
    main.cpp
    kva_init.cpp
)

set(parley_icons
    ${CMAKE_CURRENT_SOURCE_DIR}/ox16-app-parley.png 
    ${CMAKE_CURRENT_SOURCE_DIR}/ox32-app-parley.png 
    ${CMAKE_CURRENT_SOURCE_DIR}/ox48-app-parley.png 
    ${CMAKE_CURRENT_SOURCE_DIR}/ox64-app-parley.png 
    ${CMAKE_CURRENT_SOURCE_DIR}/ox128-app-parley.png 
)
kde4_add_win32_app_icon(parley_SRCS ${parley_icons})

kde4_add_executable(parley ${parley_SRCS})
kde4_install_icons( ${ICON_INSTALL_DIR} )

target_link_libraries(parley  keduvocdocument ${KDE4_KNEWSTUFF2_LIBS}  ${KDE4_PHONON_LIBS})

install(TARGETS parley  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES parley.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES parley.knsrc  DESTINATION  ${CONFIG_INSTALL_DIR} )
install( FILES parleyui.rc  DESTINATION  ${DATA_INSTALL_DIR}/parley )

