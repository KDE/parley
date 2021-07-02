/*
    SPDX-FileCopyrightText: 2009 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "parleyactions.h"

#include "prefs.h"

#include <KStandardAction>
#include <KLocalizedString>
#include <KActionCollection>
#include <KNewStuff3/kns3/knewstuffaction.h>
#include <KToggleAction>

#include <QIcon>

namespace ParleyActions
{
namespace Private
{
QAction* createCustomAction(const QObject* recvr, const char* slot, QObject* parent,
                            const QString& name,
                            const QString& text,
                            const QString& helpText,
                            const QString& iconName = QString(),
                            bool toggle = false)
{
    // Create QAction or KToggleAction
    QAction* pAction;
    if (toggle) {
        pAction = new KToggleAction(parent);
    } else {
        pAction = new QAction(parent);
    }
    // Set ObjectName, Text and HelpText
    pAction->setObjectName(name);
    pAction->setText(text);
    pAction->setToolTip(helpText);

    // Icon
    if (!iconName.isEmpty()) {
        QIcon foundIcon ( QIcon::fromTheme(iconName) );
        if ( foundIcon.isNull() ) {
            //Note: If you are using an alternative /usr/share/icons directory you need to
            //copy the /usr/share/icons/<theme>/index.theme into you alternate directory
            qDebug() << "Missing QIcon "<< iconName;
        }
        pAction->setIcon(foundIcon);
    }

    // Connect the action
    pAction->connect(pAction, SIGNAL(triggered(bool)), recvr, slot);

    // If parent is a KActionCollection, add the new action to it
    KActionCollection *collection = qobject_cast<KActionCollection *>(parent);
    if (pAction && collection)
        collection->addAction(pAction->objectName(), pAction);
    return pAction;
}
}
}

QAction* ParleyActions::create(ParleyAction id, const QObject* recvr, const char* slot, KActionCollection* parent)
{
    QAction* pAction = 0;

    switch (id) {
    case FileNew:
        pAction = KStandardAction::openNew(recvr, slot, parent);
        pAction->setToolTip(i18n("Creates a new vocabulary collection"));
        parent->setDefaultShortcut(pAction, QKeySequence::New);
        break;
    case FileOpen:
        pAction = KStandardAction::open(recvr, slot, parent);
        pAction->setToolTip(i18n("Opens an existing vocabulary collection"));
        parent->setDefaultShortcut(pAction, QKeySequence::Open);
        break;
    case FileOpenDownloaded:
        pAction = Private::createCustomAction(recvr, slot, parent,
                                              QStringLiteral("file_open_downloaded"), i18n("Open &Downloaded Vocabularies..."),
                                              i18n("Open downloaded vocabulary collections"), QStringLiteral("get-hot-new-stuff"));
        break;
    case FileSave:
        pAction = KStandardAction::save(recvr, slot, parent);
        pAction->setToolTip(i18n("Save the active vocabulary collection"));
        parent->setDefaultShortcut(pAction, QKeySequence::Save);
        break;
    case FileSaveAs:
        pAction = KStandardAction::saveAs(recvr, slot, parent);
        pAction->setShortcut(QKeySequence::SaveAs);
        parent->setDefaultShortcut(pAction, QKeySequence::SaveAs);
        pAction->setToolTip(i18n("Save the active vocabulary collection with a different name"));
        break;
    case FileExport:
        pAction = Private::createCustomAction(recvr, slot, parent,
                                              QStringLiteral("file_export"), i18n("&Export..."),
                                              i18n("Export to HTML or CSV"), QStringLiteral("document-export"));
        break;
    case FileProperties:
        pAction = Private::createCustomAction(recvr, slot, parent,
                                              QStringLiteral("file_properties"), i18n("&Properties..."),
                                              i18n("Edit document properties"), QStringLiteral("document-properties"));
        break;
    case FileClose:
        pAction = Private::createCustomAction(recvr, slot, parent,
                                              QStringLiteral("file_close"), i18n("Dashboard"),
                                              i18n("Close the current vocabulary collection and show the dashboard"), QStringLiteral("go-home"));
        break;
    case FileQuit:
        pAction = KStandardAction::quit(recvr, slot, parent);
        parent->setDefaultShortcut(pAction, QKeySequence::Quit);
        pAction->setToolTip(i18n("Quit Parley"));
        break;
    case Preferences:
        pAction = KStandardAction::preferences(recvr, slot, parent);
        parent->setDefaultShortcut(pAction, QKeySequence::Preferences);
        pAction->setToolTip(i18n("Show the configuration dialog"));
        break;
    case LanguagesProperties:
        pAction = Private::createCustomAction(recvr, slot, parent,
                                              QStringLiteral("edit_languages"), i18n("&Languages..."),
                                              i18n("Edit which languages are in the collection and their grammar properties."), QStringLiteral("set-language"));
        break;
    case RemoveGrades:
        pAction = Private::createCustomAction(recvr, slot, parent,
                                              QStringLiteral("vocab_remove_grades"), i18n("Remove Confidence Levels"),
                                              i18n("Remove all confidence levels from the current document"), QStringLiteral("edit-clear"));
        break;
    case CheckSpelling:
        pAction = KStandardAction::spelling(recvr, slot, parent);
        break;
    case ToggleShowSublessons:
        pAction = Private::createCustomAction(recvr, slot, parent,
                                              QStringLiteral("lesson_showsublessonentries"), i18n("Show Entries from Child Units"),
                                              i18n("Enable to also see the entries of child units in each unit."),
                                              QString(), true);
        pAction->setChecked(Prefs::showSublessonentries());
        break;
    case AutomaticTranslation:
        pAction = Private::createCustomAction(recvr, slot, parent,
                                              QStringLiteral("lesson_automatictranslation"), i18n("Automatic Translation"),
                                              i18n("Enable automatic translation of the unit entries."),
                                              QString(), true);
        pAction->setChecked(Prefs::automaticTranslation());
        break;
    case StartPractice:
        pAction = Private::createCustomAction(recvr, slot, parent,
                                              QStringLiteral("practice_start"), i18n("Start Practice..."),
                                              i18n("Start practicing"), QStringLiteral("practice-start"));
        break;
    case ConfigurePractice:
        pAction = Private::createCustomAction(recvr, slot, parent,
                                              QStringLiteral("practice_configure"), i18n("Configure Practice..."),
                                              i18n("Change practice settings"), QStringLiteral("practice-setup"));
        break;
    case ExportPracticeResults:
        pAction = Private::createCustomAction(recvr, slot, parent,
                                              QStringLiteral("practice_export"), i18n("Export results..."),
                                              i18n("Write a file with the results of the practice"), QStringLiteral("document-export"));
        break;
    case EnterEditMode:
        pAction = Private::createCustomAction(recvr, slot, parent,
                                              QStringLiteral("document_edit"), i18n("Editor"),
                                              i18n("Switch to vocabulary editor"), QStringLiteral("document-edit"));
        break;
    case ToggleSearchBar:
        pAction = Private::createCustomAction(recvr, slot, parent,
                                              QStringLiteral("config_show_search"), i18n("Show Se&arch"),
                                              i18n("Toggle display of the search bar"),
                                              QString(), true);
        pAction->setChecked(Prefs::showSearch());
        break;
    case SearchVocabulary:
        pAction = KStandardAction::find(recvr, slot, parent);
        parent->setDefaultShortcut(pAction, QKeySequence::Find);
        break;
    case ShowScriptManager:
        pAction = Private::createCustomAction(recvr, slot, parent,
                                              QStringLiteral("show_script_manager"), i18n("&Script Manager"),
                                              i18n("Enable and disable scripts"), QStringLiteral("set-language"));
        break;
    }

    Q_ASSERT(pAction);
    return pAction;
}



KRecentFilesAction* ParleyActions::createRecentFilesAction(const QObject* recvr, const char* slot, QObject* parent)
{
    return KStandardAction::openRecent(recvr, slot, parent);
}

QAction* ParleyActions::createDownloadAction(const QObject* recvr, const char* slot, KActionCollection* collection)
{
    QAction *pAction = new QAction(QIcon::fromTheme(QStringLiteral("get-hot-new-stuff")), i18n("Download New Vocabularies..."), collection);
    collection->addAction(QStringLiteral("file_ghns"), pAction);
    QObject::connect(pAction, SIGNAL(triggered(bool)), recvr, slot);

    pAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_G));
    collection->setDefaultShortcut(pAction, QKeySequence(Qt::CTRL | Qt::Key_G));
    pAction->setToolTip(i18n("Downloads new vocabulary collections"));
    return pAction;
}
