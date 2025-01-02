/*
    main part of Parley
    SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    SPDX-FileCopyrightText: 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-FileCopyrightText: 2008 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PARLEYMAINWINDOW_H
#define PARLEYMAINWINDOW_H

#include "parleydocument.h"
#include "practice/sessionmanagerfixed.h"
#include <KXmlGuiWindow>

#include <QPointer>

class KRecentFilesAction;

class ParleyMainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    static ParleyMainWindow *instance();

    ~ParleyMainWindow() override;

    /** enum for the different components that can be displayed */
    enum Component {
        NoComponent,
        DashboardComponent,
        EditorComponent,
        ConfigurePracticeComponent,
        PracticeComponent,
        PracticeSummary,
    };

    /**
     * setup the action (menus etc)
     */
    void initActions();

    /** add a new entry to the list of recent files */
    void addRecentFile(const QUrl &url, const QString &name);

    /** save the app-specific options on slotAppExit or by an Options dialog */
    void saveOptions();

    /** update the list of recent files in the dashboard */
    void updateRecentFilesModel();

    /**
     * Return the ParleyDocument member object
     * @return member m_document
     */
    ParleyDocument *parleyDocument();

    Component currentComponent();

    QSize sizeHint() const override;

public Q_SLOTS:
    /** Updates connections when the ParleyDocument pointer is changed to @p doc **/
    void documentUpdated(const std::shared_ptr<KEduVocDocument> &doc);

    /** Opens a dialog for a new collection. **/
    void slotFileNew();

    /** Update the title bar of the main window with the current document */
    void slotUpdateWindowCaption();

    /** When quitting, ask for confirmation if the doc has not been saved */
    bool queryClose() override;

    /** remove an entry from the list of recent files */
    void removeRecentFile(const QUrl &url);

    /** set up options */
    void slotGeneralOptions();
    void slotApplyPreferences();

    void slotCloseDocument();

    void configurePractice();

    void startPractice();
    void showPracticeSummary();
    void practiceFinished();

    void showDashboard();
    void showEditor();
    void showPractice();
    void showPracticeConfiguration();

    void switchComponent(ParleyMainWindow::Component component);

    /**
     * Shows or hides actions that are only relevant when a document is opened
     */
    void showDocumentActions(bool open, bool edit);

    void setVisibleToolbar(const QString &name);

Q_SIGNALS:
    void recentFilesChanged();
    void preferencesChanged();

private:
    ParleyMainWindow(const QUrl &filename = QUrl());
    static ParleyMainWindow *s_instance;

    Component m_currentComponent;
    QWidget *m_currentCentralWidget{nullptr};
    KXMLGUIClient *m_currentGuiClient{nullptr};

    KRecentFilesAction *m_recentFilesAction{nullptr};

    /** m_document is the current vocabulary document. */
    ParleyDocument *m_document{nullptr};

    Component m_componentBeforePractice;

    // Practice::SessionManagerContinuous m_sessionManager;
    Practice::SessionManagerFixed m_sessionManager;

    friend int main(int argc, char *argv[]);
};

#endif // PARLEYMAINWINDOW_H
