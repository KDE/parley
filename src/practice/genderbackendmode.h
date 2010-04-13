/***************************************************************************
    Copyright 2009 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef GENDERBACKENDMODE_H
#define GENDERBACKENDMODE_H

#include "multiplechoicebackendmode.h"
#include "practice/testentrymanager.h"

namespace Practice {

class GenderBackendMode : public MultipleChoiceBackendMode
{
    Q_OBJECT
    
public:
    GenderBackendMode(const PracticeOptions& PracticeOptions, AbstractFrontend *frontend, QObject *parent, Practice::TestEntryManager* testEntryManager, KEduVocDocument* doc);

protected:
    virtual void prepareChoices(TestEntry* current);
    virtual void userAnswerRight();
    virtual void userAnswerWrong();
    
private:
    KEduVocArticle m_articles;
    QString m_masculine;
    QString m_feminine;
    QString m_neuter;
    bool m_neuterExists;
};

}

#endif
