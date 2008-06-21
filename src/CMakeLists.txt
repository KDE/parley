add_subdirectory( settings )
add_subdirectory( practice/themes )

include_directories( ${CMAKE_SOURCE_DIR}/libkdeedu/keduvocdocument/
)

if(HAVE_LIBXSLT)
    include_directories( ${LIBXML2_INCLUDE_DIR} ${LIBXSLT_INCLUDE_DIR} )

    set(export_SRCS
        export/export.cpp
        export/xsldialog.cpp
    )

    kde4_add_ui_files( export_SRCS
        export/xsldialog.ui
    )
endif(HAVE_LIBXSLT)

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
    settings/pluginoptions.cpp
    settings/plugintablemodel.cpp
)

kde4_add_ui_files( libcommondlg_SRCS
    settings/viewoptionsbase.ui
    settings/generaloptionsbase.ui
    settings/pluginoptionsbase.ui
)

set(vocabulary_SRCS
    vocabulary/vocabularymodel.cpp
    vocabulary/vocabularyview.cpp
    vocabulary/vocabularyheaderview.cpp
    vocabulary/vocabularydelegate.cpp
    vocabulary/vocabularyfilter.cpp
    vocabulary/vocabularymimedata.cpp
    vocabulary/containermodel.cpp
    vocabulary/basiccontainermodel.cpp
    vocabulary/lessonmodel.cpp
    vocabulary/wordtypemodel.cpp
    vocabulary/containerview.cpp
    vocabulary/containermimedata.cpp
    vocabulary/lessonview.cpp
    vocabulary/wordtypeview.cpp
)

set(practice_SRCS
    practice/kgametheme/kgametheme.cpp
    practice/kgametheme/kgamethemeselector.cpp
    practice/parleypracticemainwindow.cpp
    practice/practiceview.cpp
    practice/input.cpp
    practice/prompt.cpp
    practice/statistics.cpp
    practice/stdbuttons.cpp
    practice/hint.cpp
    practice/answervalidator.cpp
    practice/practiceentrymanager.cpp
    practice/timer.cpp
    entryfilter.cpp
)

kde4_add_ui_files(practice_SRCS
    practice/kgametheme/kgamethemeselector.ui
    entryfilter.ui
)

#for the parleypractice app
set(parleypractice_SRCS
	${practice_SRCS}
	${libcommondlg_SKEL_SRCS}
    practice/main.cpp
)

set(practiceold_SRCS
    practiceold/testentry.cpp
    practiceold/testentrymanager.cpp
    practiceold/practicedialog.cpp
    practiceold/AdjQueryDlg.cpp
    practiceold/ArtQueryDlg.cpp
    practiceold/MCQueryDlg.cpp
    practiceold/writtenpracticedialog.cpp
    practiceold/SimpleQueryDlg.cpp
    practiceold/VerbQueryDlg.cpp
    practiceold/mixedletterpracticedialog.cpp
    practiceold/showsolutiondialog.cpp
    practiceold/practicesummarydialog.cpp
    practiceold/answervalidatorold.cpp
    entryfilter.cpp
)

kde4_add_ui_files(practiceold_SRCS
    practiceold/AdjQueryDlgForm.ui
    practiceold/ArtQueryDlgForm.ui
    practiceold/MCQueryDlgForm.ui
    practiceold/writtenpracticedialog.ui
    practiceold/SimpleQueryDlgForm.ui
    practiceold/VerbQueryDlgForm.ui
    practiceold/mixedletterpracticedialog.ui
    practiceold/showsolutionwidget.ui
    practiceold/practicesummarywidget.ui
    entryfilter.ui
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
)

kde4_add_ui_files(docprop_SRCS
    docprop-dialogs/LangPropPageForm.ui
    docprop-dialogs/TitlePageForm.ui
    docprop-dialogs/optionlistform.ui
)

set(entrydialogs_SRCS
    entry-dialogs/multiplechoicewidget.cpp
    entry-dialogs/comparisonwidget.cpp
    entry-dialogs/conjugationwidget.cpp
    entry-dialogs/declensionwidget.cpp
    entry-dialogs/imagechooserwidget.cpp
    entry-dialogs/audiowidget.cpp
    entry-dialogs/browserwidget.cpp
    entry-dialogs/synonymwidget.cpp
)

kde4_add_ui_files(entrydialogs_SRCS
    entry-dialogs/multiplechoicewidget.ui
    entry-dialogs/comparisonwidget.ui
    entry-dialogs/conjugationwidget.ui
    entry-dialogs/declensionwidget.ui
    entry-dialogs/imagechooserwidget.ui
    entry-dialogs/audiowidget.ui
    entry-dialogs/synonymwidget.ui
    entry-dialogs/browserwidget.ui
)

set(statisticsdialogs_SRCS
    statistics-dialogs/statisticsmodel.cpp
    statistics-dialogs/StatisticsDialog.cpp
    statistics-dialogs/lessonstatistics.cpp
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

set(scripts_SRCS
    scripts/scriptdialog.cpp
    scripts/scriptmanager.cpp
    scripts/scriptobjectparley.cpp
    scripts/script.cpp
)

kde4_add_ui_files(languagedialogs_SRCS
    language-dialogs/languagepage.ui
)

set(parley_SRCS
    ${vocabulary_SRCS} ${libcommondlg_SRCS}
    ${practice_SRCS}
    ${practiceold_SRCS}
    ${configure-practice_SRCS} ${docprop_SRCS}
    ${entrydialogs_SRCS} ${statisticsdialogs_SRCS}
    ${languagedialogs_SRCS}
    ${scripts_SRCS}
    ${newdocument-wizard_SRCS}
    ${export_SRCS}
    parleydocument.cpp
    parley.cpp
    main.cpp
    modeltest/modeltest.cpp
)

kde4_add_app_icon(parley_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/hi*-app-parley.png")

kde4_add_executable(parley ${parley_SRCS})
kde4_install_icons( ${ICON_INSTALL_DIR} )

target_link_libraries(
    parley
    keduvocdocument
    ${KDE4_KHTML_LIBS}
    ${KDE4_KNEWSTUFF2_LIBS}
    ${KDE4_PHONON_LIBS}
    ${KDE4_KUTILS_LIBS} #for the KPluginSelector
    ${KDE4_KROSSUI_LIBS} #for Kross
)
if(HAVE_LIBXSLT)
    target_link_libraries(parley ${LIBXML2_LIBRARIES} ${LIBXSLT_LIBRARIES})
endif(HAVE_LIBXSLT)

install(TARGETS parley  ${INSTALL_TARGETS_DEFAULT_ARGS} )

######### parleypractice #############
kde4_add_executable(parleypractice ${parleypractice_SRCS})
install(TARGETS parleypractice  DESTINATION ${BIN_INSTALL_DIR} )
target_link_libraries(
    parleypractice
    keduvocdocument
    ${KDE4_KNEWSTUFF2_LIBS}
    ${KDE4_PHONON_LIBS}
    ${KDE4_KDEUI_LIBS}
)


########### install files ###############

install( FILES parley.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES parley.knsrc  DESTINATION  ${CONFIG_INSTALL_DIR} )
install( FILES parleyui.rc  DESTINATION  ${DATA_INSTALL_DIR}/parley )
install(FILES practice/parleypracticeui.rc DESTINATION  ${DATA_INSTALL_DIR}/parley)

install( FILES practice/themes/default_theme.svgz
               practice/themes/default.desktop
    DESTINATION  ${DATA_INSTALL_DIR}/parley/themes/ )
