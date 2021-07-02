/*
    SPDX-FileCopyrightText: 2009 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PARLEYACTIONS_H
#define PARLEYACTIONS_H

#include <QAction>

class KActionCollection;
class KRecentFilesAction;

///@brief contains all parley actions
namespace ParleyActions
{
enum ParleyAction {
    FileNew,
    FileOpen, FileOpenDownloaded,
    FileSave, FileSaveAs,
    FileExport,
    FileProperties,
    FileClose, FileQuit,
    Preferences,
    LanguagesProperties,
    RemoveGrades,
    CheckSpelling,
    ToggleShowSublessons,
    AutomaticTranslation,
    StartPractice, ConfigurePractice, ExportPracticeResults,
    EnterEditMode,
    ToggleSearchBar, SearchVocabulary,
    ShowScriptManager
};

QAction* create(ParleyAction id, const QObject *recvr, const char *slot, KActionCollection *parent);

KRecentFilesAction* createRecentFilesAction(const QObject *recvr, const char *slot, QObject *parent);

QAction* createDownloadAction(const QObject *recvr, const char *slot, KActionCollection *collection);
}

#endif
