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

#ifndef PARLEYACTIONS_H
#define PARLEYACTIONS_H

#include <KAction>
#include <KRecentFilesAction>

class KActionCollection;

namespace ParleyActions {
    enum ParleyAction {
        FileNew,
        FileOpen,
        FileOpenDownloaded,
        FileOpenRecent,
        FileSave,
        FileSaveAs,
        FileExport,
        FileProperties,
        FileClose,
        FileQuit,
        Preferences
    };
    
    KAction* create(ParleyAction id, const QObject *recvr, const char *slot,
                    QObject *parent);
                    
    KRecentFilesAction* createRecentFilesAction(const QObject *recvr, const char *slot,
                    QObject *parent);
                    
    KAction* createDownloadAction(const QObject *recvr, const char *slot,
                                         KActionCollection *collection);

}

#endif
