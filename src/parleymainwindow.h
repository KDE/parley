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
#include "practice/sessionmanagercontinuous.h"
#include "practice/sessionmanagerfixed.h"

#include <QPointer>


class KRecentFilesAction;


class ParleyMainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    static ParleyMainWindow * instance();

    ~ParleyMainWindow();

    /** enum for the different components that can be displayed */
    enum Component {
        NoComponent,
        DashboardComponent,
        EditorComponent,
        ConfigurePracticeComponent,
        PracticeComponent,
        PracticeSummary
    };

    /**
     * setup the action (menus etc)
     */
    void initActions();

    /** add a new entry to the list of recent files */
    void addRecentFile(const KUrl &url, const QString &name);

    /** save the app-specific options on slotAppExit or by an Options dialog */
    void saveOptions();

    /** This will look at the lesson list and also the combo box to determine what should be displayed in the table. */
    void updateTableFilter();

    /** update the list of recent files in the dashboard */
    void updateRecentFilesModel();

    /**
     * Return the ParleyDocument member object
     * @return member m_document
     */
    ParleyDocument* parleyDocument();

    Component currentComponent();

public slots:
    /** Updates connections when the ParleyDocument pointer is changed to @p doc **/
    void documentUpdated(KEduVocDocument *doc);

    /** Opens a dialog for a new collection. **/
    void slotFileNew();

    /** Update the title bar of the main window with the current document */
    void slotUpdateWindowCaption();

    /** When quitting, ask for confirmation if the doc has not been saved */
    bool queryClose();

    /** remove an entry from the list of recent files */
    void removeRecentFile(const KUrl &url);

    QSize sizeHint() const;

    /** set up options */
    void slotGeneralOptions();
    void slotApplyPreferences();

    void slotCloseDocument();

    void configurePractice();

    void startPractice();
    void showPracticeSummary();
    void practiceFinished();


    /**
     * Show the tip of the day (force it to be shown)
     */
    void tipOfDay();

    /**
     * Show the tip of the day - the startup version that can be disabled
     */
    void startupTipOfDay();

    void showDashboard();
    void showEditor();
    void showPractice();
    void showPracticeConfiguration();

    void switchComponent(Component component);

    /**
     * Shows or hides actions that are only relevant when a document is opened
     */
    void showDocumentActions(bool open, bool edit);

    void setVisibleToolbar(const QString& name);

signals:
    void recentFilesChanged();
    void preferencesChanged();

private:
    ParleyMainWindow(const KUrl &filename = KUrl());
    static ParleyMainWindow *s_instance;

    Component      m_currentComponent;
    KXmlGuiWindow *m_currentComponentWindow;

    KRecentFilesAction *m_recentFilesAction;

    /** m_document is the current vocabulary document. */
    ParleyDocument   *m_document;

    Component m_componentBeforePractice;


    //Practice::SessionManagerContinuous m_sessionManager;
    Practice::SessionManagerFixed        m_sessionManager;

    friend int main(int argc, char* argv[]);
};

#endif // PARLEYMAINWINDOW_H
