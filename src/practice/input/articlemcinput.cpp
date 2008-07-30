//
// C++ Implementation: articlemcinput
//
// Description:
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "articlemcinput.h"


#include <KDebug>
#include <KSvgRenderer>
#include <KRandom>
#include <QRadioButton>
#include <QVBoxLayout>
#include <KRandomSequence>
#include <QString>
#include <KLocalizedString>
#include <QStringList>
#include <QGraphicsView>
#include "../practiceentry.h"

#include "prefs.h"


ArticleMCInput::ArticleMCInput(KSvgRenderer * renderer, QGraphicsView * view, const QString& elementId, KEduVocDocument * doc, QWidget * parent)
: MCInput(renderer, view, elementId, parent), m_doc(doc)
{}

void ArticleMCInput::slotSetAnswers()
{
    QStringList list;
    QString def, indef;
    QStringList parts;

    KEduVocArticle articles = m_doc->identifier(Prefs::solutionLanguage()).article();

    // set the choices
    if(articles.isEmpty())
    {
        list.append(i18nc("@label the gender of the word: Masculine", "Masculine"));
        list.append(i18nc("@label the gender of the word: Female", "Feminine"));
        list.append(i18nc("@label the gender of the word: Neuter", "Neuter"));
    }
    else
    {
        QString answer;
        answer = makeAnswer(KEduVocWordFlag::Singular | KEduVocWordFlag::Masculine);
        if (!answer.isEmpty() && !list.contains(answer))
            list.append(answer);


        answer = makeAnswer(KEduVocWordFlag::Singular |KEduVocWordFlag::Feminine);
        if (!answer.isEmpty() && !list.contains(answer)) // the contains makes sure there are no dupes
            list.append(answer);


        answer = makeAnswer(KEduVocWordFlag::Singular | KEduVocWordFlag::Neuter);
        if (!answer.isEmpty() && !list.contains(answer))
            list.append(answer);


        answer = makeAnswer(KEduVocWordFlag::Plural | KEduVocWordFlag::Masculine);
        if (!answer.isEmpty() && !list.contains(answer))
            list.append(answer);


        answer = makeAnswer(KEduVocWordFlag::Plural | KEduVocWordFlag::Feminine);
        if (!answer.isEmpty() && !list.contains(answer))
            list.append(answer);


        answer = makeAnswer(KEduVocWordFlag::Plural | KEduVocWordFlag::Neuter);
        if (!answer.isEmpty() && !list.contains(answer))
            list.append(answer);


        answer = makeAnswer(KEduVocWordFlag::Dual | KEduVocWordFlag::Masculine);
        if (!answer.isEmpty() && !list.contains(answer))
            list.append(answer);


        answer = makeAnswer(KEduVocWordFlag::Dual | KEduVocWordFlag::Feminine);
        if (!answer.isEmpty() && !list.contains(answer))
            list.append(answer);


        answer = makeAnswer(KEduVocWordFlag::Dual | KEduVocWordFlag::Neuter);
        if (!answer.isEmpty() && !list.contains(answer))
            list.append(answer);
    }

    setAvailableAnswers(list);
}

QString ArticleMCInput::makeAnswer(KEduVocWordFlags flags)
{
        QStringList parts;
        KEduVocArticle articles = m_doc->identifier(Prefs::solutionLanguage()).article();

        QString article;
        QString def = articles.article(flags | KEduVocWordFlag::Definite);
        QString indef = articles.article(flags | KEduVocWordFlag::Indefinite);
        if (!def.isEmpty()) parts.append(def);
        if (!indef.isEmpty()) parts.append(indef);
        return parts.join(" / ");
}

void ArticleMCInput::slotEmitAnswer()
{
    foreach(QRadioButton* b, findChildren<QRadioButton*>())
        if (b->isChecked())
        {
            // removes the number and Male:/Female:/Neuter:
            // it doesn't trigger when there is ONLY Male, Female, or Neuter, though
            emit signalAnswer(b->text().remove(QRegExp("^&\\d ")));
        }
    emit signalAnswer(""); // none were selected.
}

