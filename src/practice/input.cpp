//
// C++ Implementation: input
//
// Description: Implementation of the input-gathering class
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/
#include "input.h"
#include "statistics.h"
#include "practiceentry.h"
#include "prefs.h"

#include <KDebug>
#include <KSvgRenderer>
#include <KRandom>
#include <QRadioButton>
#include <QVBoxLayout>
#include <KRandomSequence>
#include <QString>
#include <KLocalizedString>
#include <QStringList>

#include "keduvocwordtype.h"
#include "keduvocdocument.h"
#include "keduvocarticle.h"

TextualInput::TextualInput(KSvgRenderer * renderer, QGraphicsView * view, const QString& elementId, QWidget* parent)
        : QLineEdit(parent),
        m_renderer(renderer)
{
    if (!renderer->elementExists(elementId))
    {
        setVisible(false);
        kDebug() << "!! Element id doesn't exist:";
        kDebug() << elementId << ":" << renderer->elementExists(elementId);
    }

     QRect bounds = m_renderer->boundsOnElement(elementId).toRect();
     setGeometry(view->mapToScene(bounds).boundingRect().toRect());

     connect(this, SIGNAL(textChanged(const QString&)), this, SIGNAL(answerChanged(const QString&)));
}

void TextualInput::slotEmitAnswer()
{
    emit signalAnswer(text());
}

void TextualInput::slotChangeAnswerColor(float correct)
{
    QPalette pal;
    if (correct == 1.0)
        pal.setColor(QPalette::Text, Qt::green);
    else
        pal.setColor(QPalette::Text, Qt::red);

    setPalette(pal);
    setText(text());
}


void TextualInput::slotShowSolution(const QString& solution)
{
    QPalette pal;
    pal.setColor(QPalette::Text, Qt::green);
    setPalette(pal);
    setText(solution);
}

void TextualInput::slotClear()
{
    QPalette pal;
    pal.setColor(QPalette::Text, Qt::black);
    setPalette(pal);
    setText("");
}

MCInput::MCInput(KSvgRenderer * renderer, QGraphicsView * view, const QString& elementId, QWidget* parent)
        : QGroupBox(parent),
        m_renderer(renderer)
{
    if (!renderer->elementExists(elementId))
    {
        setVisible(false);
        kDebug() << "!! Element id doesn't exist:";
        kDebug() << elementId << ":" << renderer->elementExists(elementId);
    }

     QRect bounds = m_renderer->boundsOnElement(elementId).toRect();
     setGeometry(view->mapToScene(bounds).boundingRect().toRect());

    setAttribute(Qt::WA_OpaquePaintEvent, true);
}

void MCInput::slotShortcutTriggered(int shortcutNumber)
{
    if (shortcutNumber > Prefs::numberMultipleChoiceAnswers())
        return; // bogus false positive

    // Shortcut number 0 is triggered by return/enter and is used for activating the currently selected option.
    // Therefore, we check if any buttons are checked, and if so, emit the signal
    // if none are checked, we ignore this shortcut
    if (shortcutNumber == 0)
        // we emit only if a button is checked
        foreach(QRadioButton* b, findChildren<QRadioButton*>())
            if (b->isChecked())
            {
                emit triggered();
                return;
            }

    foreach(QRadioButton* b, findChildren<QRadioButton*>())
    {
        if (b->text().startsWith(QString("&%1 ").arg(shortcutNumber)))
        {
            b->setChecked(true);
            emit triggered();
            return;
        }
    }
    // we didn't find anything.
}

void MCInput::setAvailableAnswers(const QStringList list)
{
    // clean up from last time
    delete layout();

    foreach(QRadioButton* b, findChildren<QRadioButton*>())
    {
        delete b;
    }


    if (list.size() == 0)
    {
        kDebug() << "Source list empty. Aborted.";
        return;
    }

    // start fresh and new!

    QVBoxLayout *vbox = new QVBoxLayout;

    int n = 1;
    foreach(QString s, list)
    {
        vbox->addWidget(new QRadioButton(QString("&%1 %2").arg(n++).arg(s)));
    }

     vbox->addStretch(1);
     setLayout(vbox);
}


MCInput::~MCInput()
{
    foreach(QRadioButton* b, findChildren<QRadioButton*>())
    {
        delete b;
    }
}

void MCInput::slotEmitAnswer()
{
    foreach(QRadioButton* b, findChildren<QRadioButton*>())
        if (b->isChecked())
        {
            emit signalAnswer(b->text().remove(QRegExp("^&\\d ")));
        }
    emit signalAnswer(""); // none were selected.
}


MultipleChoiceMCInput::MultipleChoiceMCInput(KSvgRenderer * renderer, QGraphicsView * view, const QString& elementId, QWidget* parent)
    : MCInput(renderer, view, elementId, parent)
{}


void MultipleChoiceMCInput::slotSetAnswers(PracticeEntry* currentEntry, const QList<PracticeEntry*> source)
{
    QStringList list;
    QString s;
    int timeout = 0;
    long r;

    list.append(currentEntry->expression()->translation(Prefs::solutionLanguage())->text());

    // if we only have a few entries, we use them all.
    if (source.size() < Prefs::numberMultipleChoiceAnswers())
    {
       foreach(PracticeEntry* pe, source)
       {
            s = pe->expression()->translation(Prefs::solutionLanguage())->text();
            if (!list.contains(s))
                list.append(s);
       }
    }

    else
    {
        KEduVocWordType* cwt = currentEntry->expression()->translation(Prefs::solutionLanguage())->wordType();
        KEduVocWordType *wt = 0;

        while (list.size() < Prefs::numberMultipleChoiceAnswers() && timeout < 50) // prevent infinite loop
        {
            ++timeout;
            r = KRandom::random() % source.size();
            s = source[r]->expression()->translation(Prefs::solutionLanguage())->text();
            if (!list.contains(s))
            {
                if (Prefs::multipleChoiceWordTypeConsistancy() && cwt)
                {
                    wt = source[r]->expression()->translation(Prefs::solutionLanguage())->wordType();
                    if (wt && wt->wordType() == cwt->wordType())
                        list.append(s);
                    // ignore gender of nouns in our word type comparison
                    else if (wt &&
                                (  wt->wordType() == KEduVocWordType::Noun
                                || wt->wordType() == KEduVocWordType::NounMale
                                || wt->wordType() == KEduVocWordType::NounNeutral
                                || wt->wordType() == KEduVocWordType::NounFemale)
                                &&
                                (  cwt->wordType() == KEduVocWordType::Noun
                                || cwt->wordType() == KEduVocWordType::NounMale
                                || cwt->wordType() == KEduVocWordType::NounNeutral
                                || cwt->wordType() == KEduVocWordType::NounFemale))
                            list.append(s);
                }
                else
                    list.append(s);
            }
        }
    }
    KRandomSequence(0).randomize(list);

    setAvailableAnswers(list);
}



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
        list.append(i18nc("@label the gender of the word: Male", "Male"));
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

#include "input.moc"
