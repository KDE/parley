/***************************************************************************
Copyright 2009 Frederik Gladhorn <gladhorn@kde.org>
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "parleyactions.h"

#include "prefs.h"

#include <KStandardAction>
#include <KLocalizedString>
#include <KActionCollection>
#include <knewstuff3/knewstuffaction.h>
#include <KToggleAction>

namespace ParleyActions {
    namespace Private {
        KAction* createCustomAction(const QObject* recvr, const char* slot, QObject* parent,
            const QString& name,
            const QString& text,
            const QString& helpText,
            const QString& iconName = QString(),
            bool toggle = false)
        {
            // Create KAction or KToggleAction
            KAction* pAction;
            if (toggle) {
                pAction = new KToggleAction(parent);
            } else {
                pAction = new KAction(parent);
            }
            // Set ObjectName, Text and HelpText
            pAction->setObjectName(name);
            pAction->setText(text);
            pAction->setHelpText(helpText);
            
            // Icon
            if (!iconName.isEmpty()) {
                pAction->setIcon(KIcon(iconName));
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

KAction* ParleyActions::create(ParleyAction id, const QObject* recvr, const char* slot, QObject* parent)
{
    KAction* pAction = 0;
    
    switch (id) {
        case FileNew:
            pAction = KStandardAction::openNew(recvr, slot, parent);
            pAction->setHelpText(i18n("Creates a new vocabulary collection"));
            break;
        case FileOpen:
            pAction = KStandardAction::open(recvr, slot, parent);
            pAction->setHelpText(i18n("Opens an existing vocabulary collection"));
            break;
        case FileOpenDownloaded:
            pAction = Private::createCustomAction(recvr, slot, parent, 
                "file_open_downloaded", i18n("Open &Downloaded Vocabularies..."), 
                i18n("Open downloaded vocabulary collections"), "get-hot-new-stuff");
            break;
        case FileSave:            
            pAction = KStandardAction::save(recvr, slot, parent);
            pAction->setHelpText(i18n("Save the active vocabulary collection"));
            break;
        case FileSaveAs:
            pAction = KStandardAction::saveAs(recvr, slot, parent);
            pAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
            pAction->setHelpText(i18n("Save the active vocabulary collection with a different name"));
            break;
        case FileExport:
            pAction = Private::createCustomAction(recvr, slot, parent, 
                "file_export", i18n("&Export..."), 
                i18n("Export to HTML or CSV"), "document-export");
            break;
        case FileProperties:
            pAction = Private::createCustomAction(recvr, slot, parent, 
                "file_properties", i18n("&Properties..."), 
                i18n("Edit document properties"), "document-properties");
            break;
        case FileClose:
            pAction = KStandardAction::close(recvr, slot, parent);
            pAction->setHelpText(i18n("Close the current collection"));
            break;
        case FileQuit: 
            pAction = KStandardAction::quit(recvr, slot, parent);
            pAction->setHelpText(i18n("Quit Parley"));
            break;
        case Preferences:
            pAction = KStandardAction::preferences(recvr, slot, parent);
            pAction->setHelpText(i18n("Show the configuration dialog"));
            break;
        case LanguagesProperties:            
            pAction = Private::createCustomAction(recvr, slot, parent, 
                "edit_languages", i18n("&Languages..."), 
                i18n("Edit which languages are in the collection and their grammar properties."), "set-language");
            break;
        case RemoveGrades:
            pAction = Private::createCustomAction(recvr, slot, parent, 
                "vocab_remove_grades", i18n("Remove Grades"), 
                i18n("Remove all grades from the current document"), "edit-clear");
            break;
        case CheckSpelling:
            pAction = KStandardAction::spelling(recvr, slot, parent);
            break;
        case ToggleShowSublessons:
            pAction = Private::createCustomAction(recvr, slot, parent,
                "lesson_showsublessonentries", i18n("Show Entries from Child Lessons"), 
                i18n("Enable to also see the entries of child lessons in each lesson."), 
                QString(), true);
            pAction->setChecked(Prefs::showSublessonentries());
            break;
        case AutomaticTranslation:
            pAction = Private::createCustomAction(recvr, slot, parent, 
                "lesson_automatictranslation", i18n("Automatic Translation"), 
                i18n("Enable automatic translation of the lesson entries."),
                QString(), true);
            pAction->setChecked(Prefs::automaticTranslation());
            break;
        case StartPractice:
            pAction = Private::createCustomAction(recvr, slot, parent, 
                "practice_start", i18n("Start Practice..."), 
                i18n("Start practicing"), "practice-start");
            break;           
        case ConfigurePractice:
            pAction = Private::createCustomAction(recvr, slot, parent, 
                "practice_configure", i18n("Configure Practice..."), 
                i18n("Change practice settings"), "practice-setup");
            break;  
        case EnterEditMode:
            pAction = Private::createCustomAction(recvr, slot, parent,
                "document_edit", i18n("Edit..."),
                i18n("Edit the document"), "document-edit");          
            break;
        case ToggleSearchBar:
            pAction = Private::createCustomAction(recvr, slot, parent, 
                "config_show_search", i18n("Show Se&arch"), 
                i18n("Toggle display of the search bar"),
                QString(), true);
            pAction->setChecked(Prefs::showSearch());
            break;
        case SearchVocabulary:
            pAction = KStandardAction::find(recvr, slot, parent);
            break;       
        case ShowScriptManager:
            pAction = Private::createCustomAction(recvr, slot, parent, 
                "show_script_manager", i18n("&Script Manager"), 
                i18n("Enable and disable scripts"), "set-language");                              
            break;
    }

    Q_ASSERT(pAction);        
    return pAction;
}


KRecentFilesAction* ParleyActions::createRecentFilesAction(const QObject* recvr, const char* slot, QObject* parent)
{
    return KStandardAction::openRecent(recvr, slot, parent);
}

KAction* ParleyActions::createDownloadAction(const QObject* recvr, const char* slot, KActionCollection* collection)
{
    KAction *pAction = KNS3::standardAction(i18n("Download New Vocabularies..."), recvr, slot, collection, "file_ghns");
    pAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
    pAction->setHelpText(i18n("Downloads new vocabulary collections"));
    return pAction;
}

KAction* ParleyActions::createUploadAction(const QObject* recvr, const char* slot, KActionCollection* collection)
{
    KAction *pAction = KNS3::standardActionUpload(i18n("&Upload Vocabulary Document..."), recvr, slot, collection, "file_upload");
    pAction->setHelpText(i18n("Share the current vocabulary collection with other users"));
    return pAction;
}
