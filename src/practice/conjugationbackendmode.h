/*
    SPDX-FileCopyrightText: 2010 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/


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

    bool setTestEntry(TestEntry* current) override;

    /** Return the worst pregrade for any pronoun of the current entry */
    grade_t currentPreGradeForEntry() const override;
    /** Return the worst grade for any pronoun of the current entry */
    grade_t currentGradeForEntry() const override;

    void updateGrades() override;

public Q_SLOTS:
    void hintAction() override;
    void checkAnswer() override;

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
