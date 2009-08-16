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
    static ParleyDocument * instance();
    static void destroy();

    KEduVocDocument *document();

    void setTitle(const QString& title);

    void enableAutoBackup(bool enable);
    void fetchGrammar(int languageIndex);

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

    void newDocument(bool wizard);

signals:
    void documentChanged(KEduVocDocument *newDocument);

private:
    static ParleyDocument* s_instance;
    ParleyDocument();
    ~ParleyDocument();

    void initializeDefaultGrammar(KEduVocDocument *doc);
    void setDefaultDocumentProperties(KEduVocDocument *doc);

    ParleyMainWindow *m_parleyApp;
    KEduVocDocument *m_doc;
    QTimer *m_backupTimer;

    friend class ParleyMainWindow;
};

#endif
