/*
    SPDX-FileCopyrightText: 2010 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/


#ifndef COMPARISONBACKENDMODE_H
#define COMPARISONBACKENDMODE_H

#include "abstractbackendmode.h"
#include "practice/sessionmanagerbase.h"

namespace Practice
{

class ComparisonBackendMode : public AbstractBackendMode
{
    Q_OBJECT

public:
    ComparisonBackendMode(AbstractFrontend *frontend,
                          QObject *parent, Practice::SessionManagerBase* sessionManager, KEduVocDocument* doc);

    bool setTestEntry(TestEntry* current) override;
    grade_t currentPreGradeForEntry() const override;
    grade_t currentGradeForEntry() const override;

    void checkAnswer() override;

public Q_SLOTS:
    void hintAction() override;

protected:
    void updateGrades() override;

private:

    SessionManagerBase* m_sessionManager;
    KEduVocDocument* m_doc;
    QStringList m_lastAnswers;
};

}

#endif
