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

// Qt
#include <QObject>

// keduvocdocument library
#include <keduvocdocument.h>

// Parley
#include "testentry.h"


class WordCount;


class Collection : public QObject
{
    Q_OBJECT

public:
    Collection(QUrl *url, QObject* parent = 0);
    ~Collection();

    KEduVocDocument *eduVocDocument();
    void numDueWords(WordCount &wc);

 private:
    // The contents of the document
    KEduVocDocument *m_doc;
    QList<TestEntry*> m_allTestEntries;

};

#endif
