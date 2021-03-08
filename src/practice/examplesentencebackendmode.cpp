/*
    SPDX-FileCopyrightText: 2010 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/


#include "examplesentencebackendmode.h"

#include <KLocalizedString>

using namespace Practice;

ExampleSentenceBackendMode::ExampleSentenceBackendMode(AbstractFrontend* frontend, QObject* parent,
                                                       SessionManagerBase* sessionManager,
                                                       KEduVocDocument* doc)
    : WrittenBackendMode(frontend, parent, sessionManager, doc)
{
}

bool ExampleSentenceBackendMode::setTestEntry(TestEntry* current)
{
    Practice::WrittenBackendMode::setTestEntry(current);
    QString sentence = current->entry()->translation(current->languageTo())->example();
    if (sentence.isEmpty()) {
        return false;
    }

    QString answer = current->entry()->translation(current->languageTo())->text();
    int pos = -1;
    while ((pos = sentence.indexOf(answer)) >= 0) {
        sentence.remove(pos, answer.length());
        sentence.insert(pos, QLatin1String("<font color=\"#FF0000\"><b>...</b></font>"));
    }
    m_frontend->setQuestion(sentence);
    return true;
}
