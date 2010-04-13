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


#include "genderbackendmode.h"
#include "defaultbackend.h"

#include <klocale.h>
#include <keduvocwordtype.h>

using namespace Practice;
 

GenderBackendMode::GenderBackendMode(const PracticeOptions& practiceOptions, AbstractFrontend* frontend, QObject* parent, Practice::TestEntryManager* testEntryManager, KEduVocDocument* doc)
: MultipleChoiceBackendMode(practiceOptions, frontend, parent, testEntryManager)
{
    m_articles = doc->identifier(Prefs::solutionLanguage()).article();
    
    KEduVocWordFlag::Flags singular = KEduVocWordFlag::Singular;
    KEduVocWordFlag::Flags definite = KEduVocWordFlag::Definite;
    KEduVocWordFlag::Flags indefinite = KEduVocWordFlag::Indefinite;
    KEduVocWordFlag::Flags masculine = KEduVocWordFlag::Masculine;
    KEduVocWordFlag::Flags feminine = KEduVocWordFlag::Feminine;
    KEduVocWordFlag::Flags neuter = KEduVocWordFlag::Neuter;
    
    m_masculine = m_articles.article( singular | definite | masculine );
    if (m_masculine.isEmpty()) {
        m_masculine = m_articles.article( singular | indefinite | masculine );
    }
    
    m_feminine = m_articles.article( singular | definite | feminine );
    if (m_feminine.isEmpty()) {
        m_feminine = m_articles.article( singular | indefinite | feminine );
    }
       
    m_neuter = m_articles.article( singular | definite | neuter );
    if (m_neuter.isEmpty()) {
        m_neuter = m_articles.article( singular | indefinite | neuter );
    }
    
    // best bet... if it is defined, it must exist, or if none of them is defined
    m_neuterExists = (!m_neuter.isEmpty()) || (m_masculine.isEmpty() && m_feminine.isEmpty());
}

void GenderBackendMode::prepareChoices(TestEntry* entry)
{
    Q_ASSERT(entry->entry()->translation(m_practiceOptions.languageTo())->wordType()->wordType() & KEduVocWordFlag::Noun);
    
    setQuestion(i18n("Choose the right article for \"%1\"", m_current->entry()->translation(m_practiceOptions.languageFrom())->text()));
        
    // set the word (possibly without the article)
    QString noun = entry->entry()->translation(Prefs::solutionLanguage())->text();
    
    // strip the article
    QStringList qsl = noun.split(QRegExp("\\s"), QString::SkipEmptyParts);
    QMutableStringListIterator qsli(qsl);
    while (qsli.hasNext())
        if (m_articles.isArticle(qsli.next()))
            qsli.remove();
        
        noun = qsl.join(" ");
    
    QString solution(noun);
    
    // set the choices
    QStringList choices;
    
    if (!m_masculine.isEmpty()) {
        choices.append(m_masculine + ' ' + noun);
    } else {
        choices.append(i18nc("@label the gender of the word: masculine", "%1 is masculine", noun));
    }    
    if (!m_feminine.isEmpty()) {
        choices.append(m_feminine + ' ' + noun);
    } else {
        choices.append(i18nc("@label the gender of the word: feminine", "%1 is feminine", noun));
    }   
    if (m_neuterExists && !m_neuter.isEmpty()) {
        choices.append(m_neuter + ' ' + noun);
    } else {
        choices.append(i18nc("@label the gender of the word: neuter", "%1 is neuter", noun));
    }
    
    setChoices(choices);
    
    kDebug() << entry->entry()->translation(m_practiceOptions.languageTo())->wordType()->wordType();
    if (entry->entry()->translation(m_practiceOptions.languageTo())->wordType()->wordType() & KEduVocWordFlag::Masculine) {
        setCorrectAnswer(0);
        kDebug() << "male";
    } else if (entry->entry()->translation(m_practiceOptions.languageTo())->wordType()->wordType() & KEduVocWordFlag::Feminine) {
        setCorrectAnswer(1);
        kDebug() << "female";
    } else {
        setCorrectAnswer(2);
        kDebug() << "neuter";
    }
}

void GenderBackendMode::userAnswerRight()
{
    kDebug() << "article right";
}

void GenderBackendMode::userAnswerWrong()
{
    kDebug() << "article wrong";
}



#include "genderbackendmode.moc"
