include_directories( ${parley_INCLUDE_DIRS}
		     ${CMAKE_CURRENT_SOURCE_DIR}/collection )

# installation of kconfig files .kcfg containing app settings
add_subdirectory( settings )

set(parley_LIB_SRCS
    parleyactions.cpp
    parleydocument.cpp
    parleymainwindow.cpp
    collection/testentry.cpp
    collection/entryfilter.cpp
    collection/readonlycontainermodel.cpp
    collection/containermodel.cpp
    collection/lessonmodel.cpp
    collection/vocabularymodel.cpp
    collection/wordclassmodel.cpp
    collection/vocabularymimedata.cpp
    collection/containermimedata.cpp
    configure-practice/configurepracticedialog.cpp
    configure-practice/generalpracticeoptions.cpp
    configure-practice/blockoptions.cpp
    configure-practice/thresholdoptions.cpp
    configure-practice/specificpracticeoptions.cpp
    editor/editor.cpp
    editor/audiowidget.cpp
    editor/browserwidget.cpp
    editor/comparisonwidget.cpp
    editor/inflectionwidget.cpp
    editor/conjugationwidget.cpp
    editor/declensionwidget.cpp
    editor/imagechooserwidget.cpp
    editor/multiplechoicewidget.cpp
    editor/summarywordwidget.cpp
    editor/synonymwidget.cpp
    editor/latexwidget.cpp
    practice/sessionmanagerbase.cpp
    practice/sessionmanagercontinuous.cpp
    practice/sessionmanagerfixed.cpp
    practice/abstractfrontend.cpp
    practice/abstractbackendmode.cpp
    practice/practicestatemachine.cpp
    practice/comparisonbackendmode.cpp
    practice/comparisonmodewidget.cpp
    practice/conjugationbackendmode.cpp
    practice/conjugationmodewidget.cpp
    practice/guifrontend.cpp
    practice/abstractwidget.cpp
    practice/writtenpracticewidget.cpp
    practice/writtenpracticevalidator.cpp
    practice/writtenbackendmode.cpp
    practice/examplesentencebackendmode.cpp
    practice/flashcardmodewidget.cpp
    practice/flashcardbackendmode.cpp
    practice/multiplechoicemodewidget.cpp
    practice/multiplechoicebackendmode.cpp
    practice/genderbackendmode.cpp
    practice/mixedlettersmodewidget.cpp
    practice/imagewidget.cpp
    practice/practicemainwindow.cpp
    practice/practicesummarycomponent.cpp
    practice/boxeswidget.cpp
    practice/audiobutton.cpp
    practice/summarybarwidget.cpp
    practice/themedbackgroundrenderer.cpp
    practice/imagecache.cpp
    practice/statustoggle.cpp
    practice/statustogglebutton.cpp
    practice/latexrenderer.cpp
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
    settings/generaloptions.cpp
    settings/parleyprefs.cpp
    settings/viewoptions.cpp
    settings/documentproperties.cpp
    settings/languageproperties.cpp
    settings/languagepropertiespage.cpp
    settings/kgametheme/kgametheme.cpp
    settings/kgametheme/kgamethemeselector.cpp
    statistics/conjugationoptions.cpp
    statistics/lessonstatistics.cpp
    statistics/statisticsmodel.cpp
    statistics/statisticsmainwindow.cpp
    statistics/statisticslegendwidget.cpp
    vocabulary/vocabularyview.cpp
    vocabulary/vocabularyheaderview.cpp
    vocabulary/vocabularydelegate.cpp
    vocabulary/vocabularyfilter.cpp
    vocabulary/containerview.cpp
    vocabulary/lessonview.cpp
    vocabulary/wordtypeview.cpp
    vocabulary/vocabularycolumnsdialog.cpp
    dashboard/dashboard.cpp
    dashboard/buttondelegate.cpp
    dashboard/gradereferencewidget.cpp
    dashboard/barwidget.cpp
    dashboard/collectionwidget.cpp
)

kde4_add_ui_files(parley_LIB_SRCS
    collection/entryfilter.ui
    configure-practice/generalpracticeoptions.ui
    configure-practice/blockoptions.ui
    configure-practice/thresholdoptions.ui
    configure-practice/specificpracticeoptions.ui
    editor/multiplechoicewidget.ui
    editor/comparisonwidget.ui
    editor/conjugationwidget.ui
    editor/declensionwidget.ui
    editor/imagechooserwidget.ui
    editor/audiowidget.ui
    editor/synonymwidget.ui
    editor/browserwidget.ui
    editor/summarywordwidget.ui
    editor/latexwidget.ui
    practice/practicesummarywidget.ui
    practice/practice_mainwindow.ui
    practice/practice_widget_comparison.ui
    practice/practice_widget_conjugation.ui
    practice/practice_widget_flashcard.ui
    practice/practice_widget_multiplechoice.ui
    practice/practice_widget_written.ui
    settings/optionlistform.ui
    settings/viewoptionsbase.ui
    settings/generaloptionsbase.ui
    settings/languagepropertiespage.ui
    settings/documentproperties.ui
    settings/kgametheme/kgamethemeselector.ui
    statistics/statisticsmainwindow.ui
    dashboard/dashboard.ui
)

kde4_add_kcfg_files(parley_LIB_SRCS
    settings/prefs.kcfgc
    settings/documentsettings.kcfgc
    settings/languagesettings.kcfgc
)

#### XSLT Support ####
# to transform the document xml to html or other
if(HAVE_LIBXSLT)
    set(parley_LIB_SRCS ${parley_LIB_SRCS}
        export/exportdialog.cpp
    )

    kde4_add_ui_files(parley_LIB_SRCS
        export/exportdialog.ui
    )
endif(HAVE_LIBXSLT)
#### End XSLT Support ####

kde4_add_app_icon(parley_LIB_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/hi*-app-parley.png")

# This library is used both by parley itself and the unit tests.
kde4_add_library(parley_LIB STATIC ${parley_LIB_SRCS})

set(parley_SRCS
    main.cpp
)

kde4_add_executable(parley ${parley_SRCS})

kde4_install_icons( ${ICON_INSTALL_DIR} )

target_link_libraries(parley parley_LIB ${parley_LINK_LIBS})


########### install files ###############
install(TARGETS parley  ${INSTALL_TARGETS_DEFAULT_ARGS} )

# desktop files appear in the menus
install( PROGRAMS parley.desktop DESTINATION  ${XDG_APPS_INSTALL_DIR} )

# hot new stuff config
install( FILES parley.knsrc parley-themes.knsrc DESTINATION  ${CONFIG_INSTALL_DIR} )

# RC files - define menus and toolbars
install( FILES parleyui.rc  DESTINATION  ${DATA_INSTALL_DIR}/parley )
install( FILES dashboard/dashboardui.rc DESTINATION  ${DATA_INSTALL_DIR}/parley )
install( FILES editor/editorui.rc DESTINATION  ${DATA_INSTALL_DIR}/parley )
install( FILES statistics/statisticsui.rc DESTINATION  ${DATA_INSTALL_DIR}/parley )
install( FILES practice/practiceui.rc DESTINATION  ${DATA_INSTALL_DIR}/parley )
install( FILES practice/practicesummaryui.rc DESTINATION  ${DATA_INSTALL_DIR}/parley )
