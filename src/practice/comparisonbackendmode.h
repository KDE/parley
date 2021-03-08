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

    bool setTestEntry(TestEntry* current) Q_DECL_OVERRIDE;
    grade_t currentPreGradeForEntry() const Q_DECL_OVERRIDE;
    grade_t currentGradeForEntry() const Q_DECL_OVERRIDE;

    void checkAnswer() Q_DECL_OVERRIDE;

public Q_SLOTS:
    void hintAction() Q_DECL_OVERRIDE;

protected:
    void updateGrades() Q_DECL_OVERRIDE;

private:

    SessionManagerBase* m_sessionManager;
    KEduVocDocument* m_doc;
    QStringList m_lastAnswers;
};

}

#endif
