/*
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef PARLEYDOCUMENT_H
#define PARLEYDOCUMENT_H

#include <memory>
#include <KEduVocDocument>
#include <QObject>

#include <config-parley.h>

class ParleyMainWindow;
class QTimer;

/**
    @author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class ParleyDocument : public QObject
{
    Q_OBJECT
public:
    explicit ParleyDocument(ParleyMainWindow* parleyMainWindow);
    ~ParleyDocument();


    std::shared_ptr<KEduVocDocument> document();

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
     @todo Stop using documentChanged(0) as a replacement for destroyed in editor classes.**/
    void documentChanged(const std::shared_ptr<KEduVocDocument> &newDocument);
    void languagesChanged();
    void statesNeedSaving();

private:
    void initializeDefaultGrammar(KEduVocDocument *doc);
    void setDefaultDocumentProperties(KEduVocDocument *doc);

 private:
    ParleyMainWindow *m_parleyApp;   // Pointer to the owner of this document
    QTimer           *m_backupTimer; // Timer for next autosave

    // The contents of the document
    std::shared_ptr<KEduVocDocument> m_doc;
};

#endif
