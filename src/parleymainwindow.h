/***************************************************************************

                         main part of Parley

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
                    (C) 2008 Daniel Laidig <d.laidig@gmx.de>
    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PARLEYMAINWINDOW_H
#define PARLEYMAINWINDOW_H

#include "parleydocument.h"

#include <KXmlGuiWindow>

class KRecentFilesAction;
class KActionMenu;
class KAction;
class QLabel;
class QDockWidget;
class Editor;
class WelcomeScreen;
class VocabularyView;
class VocabularyModel;
class VocabularyFilter;
class LessonView;
class WordTypeView;
class LeitnerView;
class LessonModel;
class WordTypeModel;
class LeitnerModel;
class ConjugationWidget;
class SummaryWordWidget;
class ScriptManager;
class ParleyPracticeMainWindow;

class ParleyMainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    /** construtor with appName (executable name) and filename to open */
    explicit ParleyMainWindow(const QString& appName, const KUrl &filename = KUrl());

    /** enum for the different components that can be displayed */
    enum Component {NoComponent, WelcomeComponent, EditorComponent, PracticeComponent};

    /**
     * setup the action (menus etc)
     */
    void initActions();

    /** setup the welcome screen */
    void initWelcomeScreen();

    /** save the app-specific options on slotAppExit or by an Options dialog */
    void saveOptions();

    /** This will look at the lesson list and also the combo box to determine what should be displayed in the table. */
    void updateTableFilter();

    /**
     * Return the ParleyDocument member object
     * @return member m_document
     */
    ParleyDocument* parleyDocument();

    /** return the editor instance */
    Editor* editor();

public slots:
    /** Update the title bar of the main window with the current document */
    void slotUpdateWindowCaption();

    /** When quitting, ask for confirmation if the doc has not been saved */
    bool queryClose();
    /** overloaded for Message box on last window exit */
    bool queryExit();
    /** set up options */
    void slotGeneralOptions();
    void slotApplyPreferences();

    void slotCloseDocument();

    /** General doc properties like title, author etc */
    void slotDocumentProperties();
    void slotShowStatistics();

    void configurePractice();

    void startPractice();

    /**
     * Show the tip of the day (force it to be shown)
     */
    void tipOfDay();

    /**
     * Show the tip of the day - the startup version that can be disabled
     */
    void startupTipOfDay();

    void showWelcomeScreen();
    void showEditor();
    void showPractice();

    void switchComponent(Component component);

    /**
     * Shows or hides actions that are only relevant when a document is opened
     */
    void showDocumentActions(bool open, bool edit);

signals:
    void documentChanged();

private:
    Component m_currentComponent;

    WelcomeScreen *m_welcomeScreen;
    Editor *m_editor;
    ParleyPracticeMainWindow *m_practice;

    KRecentFilesAction* m_recentFilesAction;
    KRecentFilesAction* m_downloadedFilesAction;

    /** m_document is the current vocabulary document. */
    ParleyDocument   *m_document;

    friend class ParleyDocument;

    /// the name of the executable
    QString m_appName;
};

#endif // PARLEYMAINWINDOW_H
