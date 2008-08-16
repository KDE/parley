//
// C++ Implementation: mcinput
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

#include "mcinput.h"

#include "../practiceentry.h"
#include "prefs.h"
#include "../activearea.h"

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

#include "keduvocwordtype.h"


MCInput::MCInput(KSvgRenderer * renderer, ActiveArea * area, const QString& elementId, QWidget* parent)
        : QGroupBox(parent),
        m_renderer(renderer), m_area(area)
{
    QString tId = area->translateElementId(elementId);
    if (tId.isEmpty()) setVisible(false);

     QRectF bounds = m_renderer->boundsOnElement(tId);
     bounds.translate(area->offset());
     setGeometry(bounds.toRect());

    setAutoFillBackground(false);
}

void MCInput::slotShortcutTriggered(int shortcutNumber)
{
    if (!m_area->active())
        return;

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
            if (!b->isEnabled())
                return;
            b->setChecked(true);
            if (b->isChecked())
                emit triggered();
            else
                kDebug() << "bad news";
            return;
        }
    }
    // we didn't find anything.
}

void MCInput::slotSetChoices(const QStringList& list)
{
    if (!m_area->active()) return;

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
    if (!m_area->active())
    {
        return;
    }
    foreach(QRadioButton* b, findChildren<QRadioButton*>())
    {
        if (b->isChecked())
        {
            emit signalAnswer(b->text().remove(QRegExp("^&\\d ")));
        }
     }
}


void MCInput::slotShowHint(const QString& solution)
{
    if (!m_area->active()) return;

    int n = 0;
    int r;
    QList<QRadioButton*> list = findChildren<QRadioButton*>();

    while (n < 50)
    {
        r = KRandom::random() % list.size();
        QRadioButton* b = list[r];
        if (b->isEnabled()  && (b->text().remove(QRegExp("^&\\d ")).toLower() != solution.toLower()))
        {
            b->setEnabled(false);
            return;
        }
        ++n;
    }
}


void MCInput::slotShowSolution(const QString& solution)
{
    if (!m_area->active()) return;

    foreach(QRadioButton* b, findChildren<QRadioButton*>())
    {
        if (b->isEnabled() && (b->text().remove(QRegExp("^&\\d ")).toLower() != solution.toLower()))
        {
            b->setEnabled(false);
        }
    }
}