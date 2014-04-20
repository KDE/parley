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


#ifndef CONJUGATIONBACKENDMODE_H
#define CONJUGATIONBACKENDMODE_H

#include "abstractbackendmode.h"
#include "practice/sessionmanagerbase.h"

namespace Practice
{

class ConjugationBackendMode : public AbstractBackendMode
{
    Q_OBJECT

public:
    ConjugationBackendMode(AbstractFrontend *frontend,
                           QObject *parent, Practice::SessionManagerBase* sessionManager, KEduVocDocument* doc);

    virtual bool setTestEntry(TestEntry* current);

    /** Return the worst grade for any pronoun of the current entry */
    virtual grade_t currentGradeForEntry();

    virtual void updateGrades();

public Q_SLOTS:
    virtual void hintAction();
    virtual void checkAnswer();

private:
    QStringList validPersonalPronouns();

    SessionManagerBase* m_sessionManager;
    KEduVocDocument* m_doc;

    QString m_currentTense;
    QList<KEduVocWordFlags> m_pronounFlags;
    KEduVocConjugation m_conjugation;

    QStringList m_lastAnswers;
};

}

#endif
