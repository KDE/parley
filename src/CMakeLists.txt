include_directories( ${parley_INCLUDE_DIRS}
		     ${CMAKE_CURRENT_SOURCE_DIR}/collection
		     ${CMAKE_CURRENT_SOURCE_DIR}/editor  # FIXME: Remove when moving the rest of the files in vocabulary/
)

# installation of kconfig files .kcfg containing app settings
add_subdirectory( settings )

set(parley_LIB_SRCS
    parleyactions.cpp
    parleydocument.cpp
    parleymainwindow.cpp
    utils.cpp
    collection/collection.cpp
    collection/testentry.cpp
    collection/entryfilter.cpp
    collection/entryfilterdialog.cpp
    collection/vocabularymimedata.cpp
    collection/containermimedata.cpp
    collection/readonlycontainermodel.cpp
    collection/containermodel.cpp
    collection/lessonmodel.cpp
    collection/vocabularymodel.cpp
    collection/vocabularyfilter.cpp
    collection/wordclassmodel.cpp
    collection/containerview.cpp
    editor/editor.cpp
    editor/vocabularycolumnsdialog.cpp
    editor/lessonview.cpp
    editor/vocabularydelegate.cpp
    editor/vocabularyheaderview.cpp
    editor/vocabularyview.cpp
    editor/wordtypeview.cpp
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
    practice/abstractwidget.cpp
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
    practice/configure/configurepracticedialog.cpp
    practice/configure/generalpracticeoptions.cpp
    practice/configure/blockoptions.cpp
    practice/configure/thresholdoptions.cpp
    practice/configure/specificpracticeoptions.cpp
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
    statistics/lessonstatisticsview.cpp
    statistics/statisticsmodel.cpp
    statistics/statisticsmainwindow.cpp
    statistics/statisticslegendwidget.cpp
    dashboard/dashboard.cpp
    dashboard/buttondelegate.cpp
    dashboard/gradereferencewidget.cpp
    dashboard/barwidget.cpp
    dashboard/collectionwidget.cpp
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
if(KF5NewStuff_VERSION VERSION_GREATER "5.56.0")
    install( FILES parley.knsrc parley-themes.knsrc DESTINATION  ${KDE_INSTALL_KNSRCDIR} )
else()
    install( FILES parley.knsrc parley-themes.knsrc DESTINATION  ${KDE_INSTALL_CONFDIR} )
endif()

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
