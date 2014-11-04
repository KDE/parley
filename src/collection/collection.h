/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    Copyright 2014 Inge Wallin       <inge@lysator.liu.se>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef COLLECTION_H
#define COLLECTION_H

#include <QObject>

#include <keduvocdocument.h>

//#include "../config-parley.h"


//class ParleyMainWindow;
class QTimer;

class Collection : public QObject
{
    Q_OBJECT
public:
    Collection(QObject* parent = 0);
    ~Collection();


    KEduVocDocument *eduVocDocument();

    void setTitle(const QString& title);

    /** Enable/disable the timed auto backup
     */
    void enableAutoBackup(bool enable);

public slots:

    /** close the document*/
    void close();

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
    // The contents of the document
    KEduVocDocument *m_doc;

    QTimer           *m_backupTimer; // Timer for next autosave
};

#endif
