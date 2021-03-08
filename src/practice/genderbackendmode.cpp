/*
    SPDX-FileCopyrightText: 2010 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/


#include "genderbackendmode.h"

#include <KLocalizedString>

#include <KEduVocDocument>
#include <KEduVocWordtype>

using namespace Practice;

GenderBackendMode::GenderBackendMode(AbstractFrontend* frontend, QObject* parent,
                                     Practice::SessionManagerBase* sessionManager,
                                     KEduVocDocument* doc)
    : MultipleChoiceBackendMode(frontend, parent, sessionManager)
    ,  m_doc( *doc )
{
}

bool GenderBackendMode::setTestEntry(TestEntry* current)
{
    Practice::AbstractBackendMode::setTestEntry(current);

    m_articles = m_doc.identifier(m_current->languageTo()).article();

    KEduVocWordFlag::Flags singular = KEduVocWordFlag::Singular;
    KEduVocWordFlag::Flags definite = KEduVocWordFlag::Definite;
    KEduVocWordFlag::Flags indefinite = KEduVocWordFlag::Indefinite;
    KEduVocWordFlag::Flags masculine = KEduVocWordFlag::Masculine;
    KEduVocWordFlag::Flags feminine = KEduVocWordFlag::Feminine;
    KEduVocWordFlag::Flags neuter = KEduVocWordFlag::Neuter;

    m_masculine = m_articles.article(singular | definite | masculine);
    if (m_masculine.isEmpty()) {
        m_masculine = m_articles.article(singular | indefinite | masculine);
    }

    m_feminine = m_articles.article(singular | definite | feminine);
    if (m_feminine.isEmpty()) {
        m_feminine = m_articles.article(singular | indefinite | feminine);
    }

    m_neuter = m_articles.article(singular | definite | neuter);
    if (m_neuter.isEmpty()) {
        m_neuter = m_articles.article(singular | indefinite | neuter);
    }

    // best bet... if it is defined, it must exist, or if none of them is defined
    m_neuterExists = (!m_neuter.isEmpty()) || (m_masculine.isEmpty() && m_feminine.isEmpty());

    prepareChoices(current);
    populateFrontEnd();

    return true;
}
void GenderBackendMode::prepareChoices(TestEntry* entry)
{
    Q_ASSERT(entry->entry()->translation(entry->languageTo())->wordType()->wordType() & KEduVocWordFlag::Noun);

    setQuestion(i18n("Choose the right article for \"%1\"", entry->entry()->translation(entry->languageFrom())->text()));

    // set the word (possibly without the article)
    QString noun = entry->entry()->translation(m_current->languageTo())->text();

    // strip the article
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    QStringList qsl = noun.split(QRegExp(QStringLiteral("\\s")), QString::SkipEmptyParts);
#else
    QStringList qsl = noun.split(QRegExp(QStringLiteral("\\s")), Qt::SkipEmptyParts);
#endif
    QMutableStringListIterator qsli(qsl);
    while (qsli.hasNext())
        if (m_articles.isArticle(qsli.next()))
            qsli.remove();

    noun = qsl.join(QStringLiteral(" "));

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

    qDebug() << entry->entry()->translation(entry->languageTo())->wordType()->wordType();
    if (entry->entry()->translation(entry->languageTo())->wordType()->wordType() & KEduVocWordFlag::Masculine) {
        setCorrectAnswer(0);
        qDebug() << "male";
    } else if (entry->entry()->translation(entry->languageTo())->wordType()->wordType() & KEduVocWordFlag::Feminine) {
        setCorrectAnswer(1);
        qDebug() << "female";
    } else {
        setCorrectAnswer(2);
        qDebug() << "neuter";
    }
}

void GenderBackendMode::updateGrades()
{
    KEduVocText articleGrade = m_current->entry()->translation(m_current->languageTo())->article();
    articleGrade.incPracticeCount();
    articleGrade.setPracticeDate(QDateTime::currentDateTime());
    updateGrade(articleGrade, m_frontend->resultState() == AbstractFrontend::AnswerCorrect,
                m_current->statisticBadCount() == 0);

    m_current->entry()->translation(m_current->languageTo())->setArticle(articleGrade);
}

grade_t GenderBackendMode::currentPreGradeForEntry() const
{
    KEduVocTranslation *translation = m_current->entry()->translation(m_current->languageTo());
    if (translation) {
        return translation->article().preGrade();
    }
    return KV_NORM_GRADE;
}

grade_t GenderBackendMode::currentGradeForEntry() const
{
    KEduVocTranslation *translation = m_current->entry()->translation(m_current->languageTo());
    if (translation) {
        return translation->article().grade();
    }
    return KV_NORM_GRADE;
}

