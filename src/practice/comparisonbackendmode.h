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
