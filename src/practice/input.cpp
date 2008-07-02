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

#include "keduvocwordtype.h"

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


MultipleChoiceInput::MultipleChoiceInput(KSvgRenderer * renderer, QGraphicsView * view, const QString& elementId, QWidget* parent)
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

}

void MultipleChoiceInput::slotShortcutTriggered(int shortcutNumber)
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

void MultipleChoiceInput::slotSetAnswers(PracticeEntry* currentEntry, const QList<PracticeEntry*> source)
{

    // clean up from last time
    delete layout();

    foreach(QRadioButton* b, findChildren<QRadioButton*>())
    {
        delete b;
    }



    if (source.size() == 0)
    {
        kDebug() << "Source list empty. Aborted.";
        return;
    }



    // start fresh and new!
    QVBoxLayout *vbox = new QVBoxLayout;
    QString s;
    QList<QString> list;
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

    int n = 1;
    foreach(s, list)
    {
        vbox->addWidget(new QRadioButton(QString("&%1 %2").arg(n++).arg(s)));
    }

     vbox->addStretch(1);
     setLayout(vbox);

}

MultipleChoiceInput::~MultipleChoiceInput()
{
    foreach(QRadioButton* b, findChildren<QRadioButton*>())
    {
        delete b;
    }
}

void MultipleChoiceInput::slotEmitAnswer()
{
    foreach(QRadioButton* b, findChildren<QRadioButton*>())
        if (b->isChecked())
        {
            emit signalAnswer(b->text().remove(QRegExp("^&\\d ")));
        }
    emit signalAnswer(""); // none were selected.
}

#include "input.moc"
