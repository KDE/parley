include_directories( ${LIBATTICA_INCLUDE_DIR} )

# installation of kconfig files .kcfg containing app settings
add_subdirectory( settings )

# libkdeedu/keduvocdocument contains the library for reading and writing vocabulary files
include_directories( ${CMAKE_SOURCE_DIR}/libkdeedu/keduvocdocument/ )

set(parley_SRCS
    main.cpp
    parleyactions.cpp
    parleydocument.cpp
    parleymainwindow.cpp
    configure-practice/configurepracticedialog.cpp
    configure-practice/advancedpracticeoptions.cpp
    configure-practice/blockoptions.cpp
    configure-practice/thresholdoptions.cpp
    editor/editor.cpp
    editor/audiowidget.cpp
    editor/browserwidget.cpp
    editor/comparisonwidget.cpp
    editor/conjugationwidget.cpp
#    editor/declensionwidget.cpp
    editor/imagechooserwidget.cpp
    editor/multiplechoicewidget.cpp
    editor/summarywordwidget.cpp
    editor/synonymwidget.cpp
    editor/latexwidget.cpp
    modeltest/modeltest.cpp
    practice/testentry.cpp
    practice/entryfilter.cpp
    practice/testentrymanager.cpp
    practice/practiceoptions.cpp
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
    vocabulary/vocabularycolumnsdialog.cpp
    welcomescreen/welcomescreen.cpp
    welcomescreen/buttondelegate.cpp
)

kde4_add_ui_files(parley_SRCS
    configure-practice/advancedpracticeoptions.ui
    configure-practice/blockoptions.ui
    configure-practice/thresholdoptions.ui
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
    practice/entryfilter.ui
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
    welcomescreen/welcomescreen.ui
)

kde4_add_kcfg_files(parley_SRCS
    settings/prefs.kcfgc
    settings/documentsettings.kcfgc
    settings/languagesettings.kcfgc
)

#### XSLT Support ####
# to transform the document xml to html or other
if(HAVE_LIBXSLT)
    include_directories( ${LIBXML2_INCLUDE_DIR} ${LIBXSLT_INCLUDE_DIR} )

    set(parley_SRCS ${parley_SRCS}
        export/exportdialog.cpp
    )

    kde4_add_ui_files(parley_SRCS
        export/exportdialog.ui
    )
endif(HAVE_LIBXSLT)
#### End XSLT Support ####

kde4_add_app_icon(parley_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/hi*-app-parley.png")

kde4_add_executable(parley ${parley_SRCS})
kde4_install_icons( ${ICON_INSTALL_DIR} )

target_link_libraries(
    parley
    keduvocdocument
    ${KDE4_KHTML_LIBS}
    ${KDE4_KNEWSTUFF3_LIBS}
    ${KDE4_PHONON_LIBS}
    ${KDE4_KUTILS_LIBS} #for the KPluginSelector
    ${KDE4_KROSSCORE_LIBS} ${KDE4_KROSSUI_LIBS} #for Kross
    ${QT_QTCORE_LIBRARY}
)
if(HAVE_LIBXSLT)
    target_link_libraries(parley ${LIBXML2_LIBRARIES} ${LIBXSLT_LIBRARIES})
endif(HAVE_LIBXSLT)

if (X11_Xrender_FOUND)
   target_link_libraries(parley ${X11_LIBRARIES} ${X11_Xrender_LIB})
endif(X11_Xrender_FOUND)


########### install files ###############
install(TARGETS parley  ${INSTALL_TARGETS_DEFAULT_ARGS} )

# desktop files appear in the menus
install( FILES parley.desktop DESTINATION  ${XDG_APPS_INSTALL_DIR} )

# hot new stuff config
install( FILES parley.knsrc parley-themes.knsrc DESTINATION  ${CONFIG_INSTALL_DIR} )

# UI files - define menus and toolbars
install( FILES parleyui.rc  DESTINATION  ${DATA_INSTALL_DIR}/parley )
install( FILES editor/editorui.rc DESTINATION  ${DATA_INSTALL_DIR}/parley )
install( FILES welcomescreen/welcomescreenui.rc DESTINATION  ${DATA_INSTALL_DIR}/parley )
install( FILES statistics/statisticsui.rc DESTINATION  ${DATA_INSTALL_DIR}/parley )
install( FILES practice/practiceui.rc DESTINATION  ${DATA_INSTALL_DIR}/parley )
install( FILES practice/practicesummaryui.rc DESTINATION  ${DATA_INSTALL_DIR}/parley )


