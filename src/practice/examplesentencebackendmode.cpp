/***************************************************************************
    Copyright 2010 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "examplesentencebackendmode.h"

#include "defaultbackend.h"

#include <KLocalizedString>

using namespace Practice;


ExampleSentenceBackendMode::ExampleSentenceBackendMode(const Practice::PracticeOptions& practiceOptions, AbstractFrontend* frontend, QObject* parent, TestEntryManager* testEntryManager, KEduVocDocument* doc)
:WrittenBackendMode(practiceOptions, frontend, parent,testEntryManager,doc)
{

}

bool ExampleSentenceBackendMode::setTestEntry(TestEntry* current)
{
    Practice::WrittenBackendMode::setTestEntry(current);
    QString sentence = current->entry()->translation(m_practiceOptions.languageTo())->example();
    if (sentence.isEmpty()) {
        return false;
    }

    QString answer = current->entry()->translation(m_practiceOptions.languageTo())->text();
    int pos = -1;
    while ((pos = sentence.indexOf(answer)) >= 0) {
        sentence.remove(pos, answer.length());
        sentence.insert(pos, "<font color=\"#FF0000\"><b>...</b></font>");
    }
    m_frontend->setQuestion(sentence);
    return true;
}

#include "examplesentencebackendmode.moc"
