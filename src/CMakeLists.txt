include_directories( ${parley_INCLUDE_DIRS}
		     ${CMAKE_CURRENT_SOURCE_DIR}/collection
		     ${CMAKE_CURRENT_SOURCE_DIR}/editor  # FIXME: Remove when moving the rest of the files in vocabulary/
)

# installation of kconfig files .kcfg containing app settings
add_subdirectory( settings )

set(parley_LIB_SRCS
    collection/collection.cpp
    collection/collection.h
    collection/containermimedata.cpp
    collection/containermimedata.h
    collection/containermodel.cpp
    collection/containermodel.h
    collection/containerview.cpp
    collection/containerview.h
    collection/entryfilter.cpp
    collection/entryfilterdialog.cpp
    collection/entryfilterdialog.h
    collection/entryfilter.h
    collection/lessonmodel.cpp
    collection/lessonmodel.h
    collection/readonlycontainermodel.cpp
    collection/readonlycontainermodel.h
    collection/testentry.cpp
    collection/testentry.h
    collection/vocabularyfilter.cpp
    collection/vocabularyfilter.h
    collection/vocabularymimedata.cpp
    collection/vocabularymimedata.h
    collection/vocabularymodel.cpp
    collection/vocabularymodel.h
    collection/wordclassmodel.cpp
    collection/wordclassmodel.h
    dashboard/barwidget.cpp
    dashboard/barwidget.h
    dashboard/buttondelegate.cpp
    dashboard/buttondelegate.h
    dashboard/collectionwidget.cpp
    dashboard/collectionwidget.h
    dashboard/dashboard.cpp
    dashboard/dashboard.h
    dashboard/gradereferencewidget.cpp
    dashboard/gradereferencewidget.h
    editor/audiowidget.cpp
    editor/audiowidget.h
    editor/browserwidget.cpp
    editor/browserwidget.h
    editor/comparisonwidget.cpp
    editor/comparisonwidget.h
    editor/conjugationwidget.cpp
    editor/conjugationwidget.h
    editor/declensionwidget.cpp
    editor/declensionwidget.h
    editor/editor.cpp
    editor/editor.h
    editor/imagechooserwidget.cpp
    editor/imagechooserwidget.h
    editor/inflectionwidget.cpp
    editor/inflectionwidget.h
    editor/latexwidget.cpp
    editor/latexwidget.h
    editor/lessonview.cpp
    editor/lessonview.h
    editor/multiplechoicewidget.cpp
    editor/multiplechoicewidget.h
    editor/summarywordwidget.cpp
    editor/summarywordwidget.h
    editor/synonymwidget.cpp
    editor/synonymwidget.h
    editor/translateshelladapter.cpp
    editor/translateshelladapter.h
    editor/vocabularycolumnsdialog.cpp
    editor/vocabularycolumnsdialog.h
    editor/vocabularydelegate.cpp
    editor/vocabularydelegate.h
    editor/vocabularyheaderview.cpp
    editor/vocabularyheaderview.h
    editor/vocabularyview.cpp
    editor/vocabularyview.h
    editor/wordtypeview.cpp
    editor/wordtypeview.h
    parleyactions.cpp
    parleyactions.h
    parleydocument.cpp
    parleydocument.h
    parleymainwindow.cpp
    parleymainwindow.h
    practice/abstractbackendmode.cpp
    practice/abstractbackendmode.h
    practice/abstractfrontend.cpp
    practice/abstractfrontend.h
    practice/abstractwidget.cpp
    practice/abstractwidget.h
    practice/audiobutton.cpp
    practice/audiobutton.h
    practice/boxeswidget.cpp
    practice/boxeswidget.h
    practice/comparisonbackendmode.cpp
    practice/comparisonbackendmode.h
    practice/comparisonmodewidget.cpp
    practice/comparisonmodewidget.h
    practice/configure/blockoptions.cpp
    practice/configure/blockoptions.h
    practice/configure/configurepracticedialog.cpp
    practice/configure/configurepracticedialog.h
    practice/configure/generalpracticeoptions.cpp
    practice/configure/generalpracticeoptions.h
    practice/configure/specificpracticeoptions.cpp
    practice/configure/specificpracticeoptions.h
    practice/configure/thresholdoptions.cpp
    practice/configure/thresholdoptions.h
    practice/conjugationbackendmode.cpp
    practice/conjugationbackendmode.h
    practice/conjugationdata.h
    practice/conjugationmodewidget.cpp
    practice/conjugationmodewidget.h
    practice/examplesentencebackendmode.cpp
    practice/examplesentencebackendmode.h
    practice/flashcardbackendmode.cpp
    practice/flashcardbackendmode.h
    practice/flashcardmodewidget.cpp
    practice/flashcardmodewidget.h
    practice/genderbackendmode.cpp
    practice/genderbackendmode.h
    practice/guifrontend.cpp
    practice/guifrontend.h
    practice/imagecache.cpp
    practice/imagecache.h
    practice/imagewidget.cpp
    practice/imagewidget.h
    practice/latexrenderer.cpp
    practice/latexrenderer.h
    practice/mixedlettersmodewidget.cpp
    practice/mixedlettersmodewidget.h
    practice/multiplechoicebackendmode.cpp
    practice/multiplechoicebackendmode.h
    practice/multiplechoicedata.h
    practice/multiplechoicemodewidget.cpp
    practice/multiplechoicemodewidget.h
    practice/practicemainwindow.cpp
    practice/practicemainwindow.h
    practice/practicestatemachine.cpp
    practice/practicestatemachine.h
    practice/practicesummarycomponent.cpp
    practice/practicesummarycomponent.h
    practice/sessionmanagerbase.cpp
    practice/sessionmanagerbase.h
    practice/sessionmanagercontinuous.cpp
    practice/sessionmanagercontinuous.h
    practice/sessionmanagerfixed.cpp
    practice/sessionmanagerfixed.h
    practice/statustogglebutton.cpp
    practice/statustogglebutton.h
    practice/statustoggle.cpp
    practice/statustoggle.h
    practice/summarybarwidget.cpp
    practice/summarybarwidget.h
    practice/themedbackgroundrenderer.cpp
    practice/themedbackgroundrenderer.h
    practice/writtenbackendmode.cpp
    practice/writtenbackendmode.h
    practice/writtenpracticevalidator.cpp
    practice/writtenpracticevalidator.h
    practice/writtenpracticewidget.cpp
    practice/writtenpracticewidget.h
    settings/documentproperties.cpp
    settings/documentproperties.h
    settings/generaloptions.cpp
    settings/generaloptions.h
    settings/kgametheme/kgametheme.cpp
    settings/kgametheme/kgametheme.h
    settings/kgametheme/kgamethemeselector.cpp
    settings/kgametheme/kgamethemeselector.h
    settings/languageproperties.cpp
    settings/languageproperties.h
    settings/languagepropertiespage.cpp
    settings/languagepropertiespage.h
    settings/parleyprefs.cpp
    settings/parleyprefs.h
    settings/translationshelloptions.cpp
    settings/translationshelloptions.h
    settings/viewoptions.cpp
    settings/viewoptions.h
    statistics/conjugationoptions.cpp
    statistics/conjugationoptions.h
    statistics/lessonstatisticsview.cpp
    statistics/lessonstatisticsview.h
    statistics/statisticslegendwidget.cpp
    statistics/statisticslegendwidget.h
    statistics/statisticsmainwindow.cpp
    statistics/statisticsmainwindow.h
    statistics/statisticsmodel.cpp
    statistics/statisticsmodel.h
    utils.cpp
    utils.h

)

ki18n_wrap_ui(parley_LIB_SRCS
    collection/entryfilterdialog.ui
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
    practice/configure/generalpracticeoptions.ui
    practice/configure/blockoptions.ui
    practice/configure/thresholdoptions.ui
    practice/configure/specificpracticeoptions.ui
    settings/optionlistform.ui
    settings/viewoptionsbase.ui
    settings/generaloptionsbase.ui
    settings/languagepropertiespage.ui
    settings/documentproperties.ui
    settings/kgametheme/kgamethemeselector.ui
    settings/translationshellpage.ui
    statistics/statisticsmainwindow.ui
    dashboard/dashboard.ui
)

kconfig_add_kcfg_files(parley_LIB_SRCS
    settings/prefs.kcfgc
    settings/documentsettings.kcfgc
    settings/languagesettings.kcfgc
)

qt5_add_dbus_adaptor(parley_LIB_SRCS editor/org.kde.parley.xml
                     editor/editor.h Editor::EditorWindow)

#### XSLT Support ####
# to transform the document xml to html or other
if(HAVE_LIBXSLT)
    set(parley_LIB_SRCS ${parley_LIB_SRCS}
        exportdialog.cpp
        exportdialog.h
    )

    ki18n_wrap_ui(parley_LIB_SRCS
        exportdialog.ui
    )
endif(HAVE_LIBXSLT)
#### End XSLT Support ####

# This library is used both by parley itself and the unit tests.
add_library(parley_LIB STATIC ${parley_LIB_SRCS})

set(parley_SRCS main.cpp)
add_executable(parley ${parley_SRCS})

target_link_libraries(parley parley_LIB ${parley_LINK_LIBS})
target_link_libraries(parley_LIB ${parley_LINK_LIBS})

########### install files ###############
install(TARGETS parley  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )

# desktop files appear in the menus
install( PROGRAMS org.kde.parley.desktop DESTINATION  ${KDE_INSTALL_APPDIR} )

# hot new stuff config
install( FILES parley.knsrc parley-themes.knsrc DESTINATION  ${KDE_INSTALL_KNSRCDIR} )


# UI files - define menus and toolbars
# @todo frameworks find a definitive locations for files and then use KXMLGUI_INSTALL_DIR
install( FILES parleyui.rc  DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/parley )
#install( FILES editor/editorui.rc DESTINATION  ${KDE_INSTALL_DATADIR}/parley )
install( FILES editor/editorui.rc DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/parley )
install( FILES dashboard/dashboardui.rc DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/parley )
# install( FILES statistics/statisticsui.rc DESTINATION  ${KDE_INSTALL_DATADIR}/parley )
# install( FILES practice/practiceui.rc DESTINATION  ${KDE_INSTALL_DATADIR}/parley )
# install( FILES practice/practicesummaryui.rc DESTINATION  ${KDE_INSTALL_DATADIR}/parley )
install( FILES statistics/statisticsui.rc DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/parley )
install( FILES practice/practiceui.rc DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/parley )
install( FILES practice/practicesummaryui.rc DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/parley )
