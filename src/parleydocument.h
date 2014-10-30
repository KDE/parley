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
    ParleyDocument(ParleyMainWindow* parleyMainWindow);
    ~ParleyDocument();


    KEduVocDocument *document();

    void setTitle(const QString& title);

    /** Enable/disable the timed auto backup
     @todo Move autobackup functionality into libkdeedu which is responsible for
    file opening/closing and locking already.*/
    void enableAutoBackup(bool enable);
    //void fetchGrammar(int languageIndex);

public slots:
    /** open a new application window */
    void slotFileNew();

    /** open a document */
    void slotFileOpen();

    /** opens a file from the recent files menu */
    void slotFileOpenRecent(const QUrl& url);
    /** open a downloaded (knewstuff/get hot new stuff) document */
    void openGHNS();
    /** download new vocabularies */
    void slotGHNS();
    /** upload the current file */
    void uploadFile();

    /** Opens the given url, displays an error message and returns false on failure */
    bool open(const QUrl &);

    /** close the document*/
    void close();
    /** When quitting, ask for confirmation if the doc has not been saved */
    bool queryClose();


    /** merge a document */
    void slotFileMerge();
    /** save a document */
    void save();
    void saveAs(QUrl file = QUrl());

    void exportDialog();

    void newDocument(bool wizard);

    /** General doc properties like title, author etc */
    void documentProperties();
    void languageProperties();

signals:
    /** Emitted when the document pointer is changed.
     @todo Stop using documentChanged(0) as a replacement for destoyed in editor classes.**/
    void documentChanged(KEduVocDocument *newDocument);
    void languagesChanged();
    void statesNeedSaving();

private:
    void initializeDefaultGrammar(KEduVocDocument *doc);
    void setDefaultDocumentProperties(KEduVocDocument *doc);

 private:
    ParleyMainWindow *m_parleyApp;   // Pointer to the owner of this document
    QTimer           *m_backupTimer; // Timer for next autosave

    // The contents of the document
    KEduVocDocument *m_doc;
};

#endif
