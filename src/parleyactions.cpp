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

#include <KStandardAction>
#include <KLocalizedString>
#include <KActionCollection>
#include <knewstuff2/ui/knewstuffaction.h>

namespace ParleyActions {
    namespace Private {
        KAction* createCustomAction(const QObject* recvr, const char* slot, QObject* parent,
            const QString& name,
            const QString& text,
            const QString& helpText,
            const QString& iconName = QString())
        {
            KAction* pAction = new KAction(parent);
            pAction->setObjectName(name);
            pAction->setIcon(KIcon(iconName));
            pAction->setText(text);
            parent->connect(pAction, SIGNAL(triggered(bool)), recvr, slot);
            pAction->setHelpText(helpText);
            
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
    }
    /*
    pAction = Private::createCustomAction(recvr, slot, parent, 
                                          "", i18n(""), 
                                          i18n(""), "");
                                          */
    Q_ASSERT(pAction);        
    return pAction;
}


KRecentFilesAction* ParleyActions::createRecentFilesAction(const QObject* recvr, const char* slot, QObject* parent)
{
    return KStandardAction::openRecent(recvr, slot, parent);
}

KAction* ParleyActions::createDownloadAction(const QObject* recvr, const char* slot, KActionCollection* collection)
{
    KAction *pAction = KNS::standardAction(i18n("Download New Vocabularies..."), recvr, slot, collection, "file_ghns");
    pAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
    pAction->setHelpText(i18n("Downloads new vocabulary collections"));
    return pAction;
}




    