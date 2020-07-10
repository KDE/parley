/***************************************************************************

    Copyright 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "collection.h"

// KEduVocDocument library
#include <KEduVocDocument>

// Parley
#include "../utils.h"
#include "entryfilter.h"


// ----------------------------------------------------------------

Collection::Collection(QUrl *url, QObject* parent)
    : QObject(parent)
    , m_doc(new KEduVocDocument(this))
{
    // We ignore file locks here because we open the file for readonly
    // purposes only.
    m_doc->open(*url, KEduVocDocument::FileOpenReadOnly);
}

Collection::~Collection()
{
}

KEduVocDocument *Collection::eduVocDocument()
{
    return m_doc;
}

void Collection::numDueWords(WordCount &wc)
{
    // Get the entries from the collection. Cache them for future use.
    if (m_allTestEntries.isEmpty()) {
	EntryFilter  filter(m_doc, this);
	m_allTestEntries = filter.entries(false);
    }

    // Count the number of words due for each grade level.
    foreach (const TestEntry *entry, m_allTestEntries) {
	int languageTo = entry->languageTo();
	KEduVocExpression *exp = entry->entry();

        int grade    = exp->translation(languageTo)->grade();
        int pregrade = exp->translation(languageTo)->preGrade();
        if (exp->translation(languageTo)->text().isEmpty()) {
            wc.invalid++;
        }
        else if (pregrade > 0) {
            wc.pregrades[pregrade]++;
            wc.initialWords++;
            wc.grades[0]++;
        }
        else {
            wc.grades[grade]++;
        }
    }

    wc.totalWords = m_allTestEntries.count();
    //kDebug() << m_doc->title() << wc.totalWords << "entries";
}

