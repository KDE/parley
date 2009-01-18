/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef PARLEYDOCUMENT_H
#define PARLEYDOCUMENT_H

#include <keduvocdocument.h>
#include <QObject>

#include "../config-parley.h"

class ParleyMainWindow;
class QTimer;

/**
	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class ParleyDocument : public QObject
{
Q_OBJECT
public:
    ParleyDocument(ParleyMainWindow *parent);

    ~ParleyDocument();

    /**
     * Pointer to the current document.
     * @return 
     */
    KEduVocDocument *document();

    void enableAutoBackup(bool enable);

public slots:
    /** open a new application window */
    void slotFileNew();

    /** open a document */
    void slotFileOpen();

    /** opens a file from the recent files menu */
    void slotFileOpenRecent(const KUrl& url);
    /** open a downloaded (knewstuff/get hot new stuff) document */
    void openGHNS();
    /** download new vocabularies */
    void slotGHNS();

    void open(const KUrl &);
    void close();

    /** merge a document */
    void slotFileMerge();
    /** save a document */
    void save();
    void saveAs(KUrl file = KUrl());

    void exportDialog();

    void newDocument(bool wizard = false);

    void slotSaveSelection();

signals:
    void documentChanged(KEduVocDocument *newDocument);

private:
    /**
     * Add some pre defined types and usages.
     */
    void initializeDefaultGrammar(KEduVocDocument *doc);
    /**
     *
     * 
     * When starting the first time, create some entries to get started.
     */
    void createExampleEntries(KEduVocDocument *doc);

    QTimer *m_backupTimer;
    KEduVocDocument *m_doc;
    ParleyMainWindow *m_parleyApp;
};

#endif
