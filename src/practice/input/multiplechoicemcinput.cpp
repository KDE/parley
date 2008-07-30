//
// C++ Implementation: multiplechoicemcinput
//
// Description:
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/


#include "multiplechoicemcinput.h"

#include "../practiceentry.h"
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
#include <QGraphicsView>

#include <keduvocwordtype.h>

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
        // we use an int so we can play with bitwise flags
        int cwt = currentEntry->expression()->translation(Prefs::solutionLanguage())->wordType()->wordType();
        // filter everything that isn't a part of speech flag.
        cwt &= KEduVocWordFlag::partsOfSpeech;
        int wt = 0;

        while (list.size() < Prefs::numberMultipleChoiceAnswers() && timeout < 50) // prevent infinite loop
        {
            ++timeout;
            r = KRandom::random() % source.size();
            s = source[r]->expression()->translation(Prefs::solutionLanguage())->text();
            if (!list.contains(s))
            {
                if (Prefs::multipleChoiceWordTypeConsistancy() && cwt)
                {
                    wt = source[r]->expression()->translation(Prefs::solutionLanguage())->wordType()->wordType();
                    wt &= KEduVocWordFlag::partsOfSpeech;
                    if (wt == cwt)
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

