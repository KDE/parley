# installation of kconfig files .kcfg containing app settings
add_subdirectory( settings )

# libkdeedu/keduvocdocument contains the library for reading and writing vocabulary files
include_directories( ${CMAKE_SOURCE_DIR}/libkdeedu/keduvocdocument/
)

# xslt to transform the document xml to html or other
if(HAVE_LIBXSLT)
    include_directories( ${LIBXML2_INCLUDE_DIR} ${LIBXSLT_INCLUDE_DIR} )

    set(export_SRCS
        export/exportdialog.cpp
    )

    kde4_add_ui_files( export_SRCS
        export/exportdialog.ui
    )
endif(HAVE_LIBXSLT)

# create kconfig
kde4_add_kcfg_files( settings_SKEL_SRCS
    settings/prefs.kcfgc
    settings/documentsettings.kcfgc
    settings/languagesettings.kcfgc
)

# settings, document properties and grammar options
set(settings_SRCS ${settings_SKEL_SRCS}
# general settings dialog
    settings/parleyprefs.cpp
    settings/generaloptions.cpp
    settings/viewoptions.cpp

# script settings
    settings/documentproperties.cpp
    settings/languageproperties.cpp
    settings/languagepropertiespage.cpp
)

# settings ui
kde4_add_ui_files( settings_SRCS
    settings/optionlistform.ui
    settings/viewoptionsbase.ui
    settings/generaloptionsbase.ui
    settings/languagepropertiespage.ui
    settings/documentproperties.ui
)

# views on the vocabulary files contents - lessons and the actual words
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
    vocabulary/leitnerview.cpp
    vocabulary/leitnermodel.cpp
    vocabulary/vocabularycolumnsdialog.cpp
)

set(editor_SRCS
    editor/editor.cpp
)

set(practice_SRCS
    practiceold/vocabularypractice.cpp
    practiceold/testentry.cpp
    practiceold/testentrymanager.cpp
    practiceold/entryfilter.cpp
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

    practice/abstractfrontend.cpp
    practice/abstractbackend.cpp
    practice/fakebackend.cpp
    practice/defaultbackend.cpp
    practice/guifrontend.cpp
    practice/abstractwidget.cpp
    practice/writtenpracticewidget.cpp
)

kde4_add_ui_files(practice_SRCS
    practiceold/entryfilter.ui
    practiceold/AdjQueryDlgForm.ui
    practiceold/ArtQueryDlgForm.ui
    practiceold/MCQueryDlgForm.ui
    practiceold/writtenpracticedialog.ui
    practiceold/SimpleQueryDlgForm.ui
    practiceold/VerbQueryDlgForm.ui
    practiceold/mixedletterpracticedialog.ui
    practiceold/showsolutionwidget.ui
    practiceold/practicesummarywidget.ui

    practice/practice_mainwindow.ui
    practice/practice_widget_written.ui
)

# settings for the practice
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
    configure-practice/multiplechoiceoptionswidget.ui
    configure-practice/flashcardsoptionswidget.ui
)

# dock widgets to help editing vocabulary
set(entrydialogs_SRCS
    entry-dialogs/multiplechoicewidget.cpp
    entry-dialogs/comparisonwidget.cpp
    entry-dialogs/conjugationwidget.cpp
    entry-dialogs/declensionwidget.cpp
    entry-dialogs/imagechooserwidget.cpp
    entry-dialogs/audiowidget.cpp
    entry-dialogs/browserwidget.cpp
    entry-dialogs/synonymwidget.cpp
    entry-dialogs/summarywordwidget.cpp
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
    entry-dialogs/summarywordwidget.ui
)

# overview over grades
set(statisticsdialogs_SRCS
    statistics-dialogs/statisticsmodel.cpp
    statistics-dialogs/StatisticsDialog.cpp
    statistics-dialogs/lessonstatistics.cpp
)

# scripting interface using kross
set(scripts_SRCS
    scripts/scriptdialog.cpp
    scripts/scriptmanager.cpp
    scripts/scripting/parley.cpp
    scripts/scripting/document.cpp
    scripts/scripting/lesson.cpp
    scripts/scripting/expression.cpp
    scripts/scripting/translation.cpp
    scripts/scripting/text.cpp
    scripts/scripting/container.cpp
    scripts/scripting/identifier.cpp
    scripts/script.cpp
    scripts/translator.cpp

)

# the first thing you see when startin parley
set(welcomescreen_SRCS
    welcomescreen/welcomescreen.cpp
    welcomescreen/buttondelegate.cpp
)

kde4_add_ui_files(welcomescreen_SRCS
    welcomescreen/welcomescreen.ui
)

# put it all together
set(parley_SRCS
    ${vocabulary_SRCS}
    ${editor_SRCS}
    ${settings_SRCS}
    ${practice_SRCS}
    ${configure-practice_SRCS}
    ${entrydialogs_SRCS}
    ${statisticsdialogs_SRCS}
    ${scripts_SRCS}
    ${export_SRCS}
    ${welcomescreen_SRCS}
    parleydocument.cpp
    parleymainwindow.cpp
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
    ${KDE4_KROSSCORE_LIBS} ${KDE4_KROSSUI_LIBS} #for Kross
    ${QT_QTCORE_LIBRARY}
)
if(HAVE_LIBXSLT)
    target_link_libraries(parley ${LIBXML2_LIBRARIES} ${LIBXSLT_LIBRARIES})
endif(HAVE_LIBXSLT)

########### install files ###############
install(TARGETS parley  ${INSTALL_TARGETS_DEFAULT_ARGS} )

# desktop files appear in the menus
install( FILES parley.desktop DESTINATION  ${XDG_APPS_INSTALL_DIR} )

# hot new stuff config
install( FILES parley.knsrc  DESTINATION  ${CONFIG_INSTALL_DIR} )

# main window ui (menus)
install( FILES parleyui.rc  DESTINATION  ${DATA_INSTALL_DIR}/parley )

# editor ui
install( FILES editor/editorui.rc DESTINATION  ${DATA_INSTALL_DIR}/parley )


