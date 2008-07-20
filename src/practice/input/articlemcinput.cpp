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

void ArticleMCInput::slotSetAnswers(PracticeEntry* currentEntry)
{
    QStringList list;
    QString def, indef;

    KEduVocArticle articles = m_doc->identifier(Prefs::solutionLanguage()).article();

    // set the choices
    if(articles.isEmpty())
    {
        list.append(i18nc("@label the gender of the word: Masculine", "Male"));
        list.append(i18nc("@label the gender of the word: Female", "Female"));
        list.append(i18nc("@label the gender of the word: Neutral", "Neutral"));
    }
    else
    {
        QString article;
        def = articles.article( KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Masculine );
        indef = articles.article( KEduVocArticle::Singular, KEduVocArticle::Indefinite, KEduVocArticle::Masculine );
        bool male = !(def.isEmpty() && indef.isEmpty());

        if((!def.isEmpty()) && (!indef.isEmpty())) {
            article = def + " / " + indef;
        } else {
            article = def + indef;
        }
        list.append(i18nc("@label the gender of the word: male", "Male:\t") + article);

        def = articles.article( KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Feminine );
        indef = articles.article( KEduVocArticle::Singular, KEduVocArticle::Indefinite, KEduVocArticle::Feminine );
        bool female = !(def.isEmpty() && indef.isEmpty());
        if((!def.isEmpty()) && (!indef.isEmpty())) {
            article = def + " / " + indef;
        } else {
            article = def + indef;
        }
        list.append(i18nc("@label the gender of the word: female", "Female:\t") + article);

        def = articles.article( KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Neutral );
        indef = articles.article( KEduVocArticle::Singular, KEduVocArticle::Indefinite, KEduVocArticle::Neutral );
        bool neutral = !(def.isEmpty() && indef.isEmpty());
        if((!def.isEmpty()) && (!indef.isEmpty())) {
            article = def + " / " + indef;
        } else {
            article = def + indef;
        }
        if (!(!neutral && male && female))
            list.append(i18nc("@label the gender of the word: neutral", "Neutral:\t") + article);
    }

    setAvailableAnswers(list);
}

void ArticleMCInput::slotEmitAnswer()
{
    foreach(QRadioButton* b, findChildren<QRadioButton*>())
        if (b->isChecked())
        {
            // removes the number and Male:/Female:/Neutral:
            // it doesn't trigger when there is ONLY Male, Female, or Neutral, though
            emit signalAnswer(b->text().remove(QRegExp("^&\\d .+:\t")));
        }
    emit signalAnswer(""); // none were selected.
}

