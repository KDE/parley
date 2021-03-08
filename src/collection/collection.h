/*
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-FileCopyrightText: 2014 Inge Wallin <inge@lysator.liu.se>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef COLLECTION_H
#define COLLECTION_H

// Qt
#include <QObject>

// keduvocdocument library
#include <KEduVocDocument>

// Parley
#include "testentry.h"


struct WordCount;


class Collection : public QObject
{
    Q_OBJECT

public:
    explicit Collection(QUrl *url, QObject* parent = 0);
    ~Collection();

    KEduVocDocument *eduVocDocument();
    void numDueWords(WordCount &wc);

 private:
    // The contents of the document
    KEduVocDocument *m_doc;
    QList<TestEntry*> m_allTestEntries;

};

#endif
