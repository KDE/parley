//
// C++ Implementation: statistics
//
// Description: Implementation of the statistics-tracking eduwidget
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/
#include "statistics.h"

#include <KDebug>
#include <KLocalizedString>
#include <QGraphicsSvgItem>
#include <KSvgRenderer>

#include <KStandardDirs>

Statistics::Statistics(QObject * parent)
    : QObject(parent)
{
    m_tainted = false;
    m_answerChecked = false;
    m_attempted = 0;
    m_correct = 0;
    m_cycles = 0;
    m_tainted = 0;
    m_taintedCorrect = 0;
    m_taintedIncorrect = 0;
    m_skipped = 0;
    m_streakLength = 0;

}

Statistics::~Statistics() {}

LCDStatistics::LCDStatistics(QWidget * parent)
        :  QLCDNumber(parent)
{
    display(0.0);
}

SvgBarStatistics::SvgBarStatistics(const QRectF& background, QGraphicsItem * parent)
    : QGraphicsSvgItem(parent),
      m_backgroundRect(background)
{
    KSvgRenderer * krenderer = new KSvgRenderer();
    krenderer->load(KStandardDirs::locate("data", "parley/defaulttheme/widgets.svgz"));
    setSharedRenderer(krenderer);
    setElementId("progress_bar");
    setPos(m_backgroundRect.x(), m_backgroundRect.y());
    scale((m_backgroundRect.width())/boundingRect().width(), 1.0);
    kDebug() << boundingRect() << scenePos();
    setZValue(10); // higher than the rest
}

SvgBarStatistics::~SvgBarStatistics()
{}



// TODO figure out what to do with grade...
void Statistics::slotCorrection(float grade, ErrorType error)
{
    // this is true when the answer supplied was correct.
    if ((grade == 1.0) && (error == Correct))
       slotCorrect();
    else
        slotIncorrect(error);
}

void Statistics::slotIncorrect(ErrorType error)
{
    ++m_attempted;
    // I don't know a better way to do this...
    if (error & SpellingMistake) ++m_errorReasons[0];

    if (error & CapitalizationMistake) ++m_errorReasons[1];

    if (error & AccentMistake) ++m_errorReasons[2];

    if (error & ArticleWrong) ++m_errorReasons[3];

    if (error & ArticleMissing) ++m_errorReasons[4];

    if (error & FalseFriend) ++m_errorReasons[5];

    if (error & Synonym) ++m_errorReasons[6];

    if (error & Empty) ++m_errorReasons[7];

    if (error & UnrelatedWord) ++m_errorReasons[8];

    if (error & Incomplete) ++m_errorReasons[9];

    if (error & Correct) ++m_errorReasons[10]; // this should never be used...

    if (error & UnknownMistake) ++m_errorReasons[11];

    m_tainted = false;

    m_answerChecked = true;

    m_streakLength = 0;

    emit signalUpdateDisplay(this);
}

void Statistics::slotCorrect()
{
    ++m_attempted;
    if (m_tainted)
    {
        ++m_taintedCorrect;
        m_tainted = false;
        m_streakLength = 0;
    }
    else
    {
        ++m_correct;
        ++m_streakLength;
    }

    m_answerChecked = true;

    emit signalUpdateDisplay(this);
}

void Statistics::slotSolutionShown()
{
    if (m_answerChecked) // the answer has already been checked and counted.
        return; // do nothing
    else // the user requested that the answer be shown before they provided an answer
        slotIncorrect(SolutionShown);
}


void Statistics::slotSkippedKnown()
{
    ++m_skipReasons[Known];
}

void Statistics::slotSkippedUnknown()
{
    ++m_attempted;
    m_tainted = false;
    m_streakLength = 0;
    emit signalUpdateDisplay(this);
}

void Statistics::slotTaintAnswer(TaintReason reason)
{
    ++m_taintReasons[reason];
}


void Statistics::slotSetFinished()
{
    // TODO do something here ;)
}


const QString Statistics::gradeToString(int i)
{
    switch (i)
    {

        case KV_NORM_GRADE:
            return i18n(KV_NORM_TEXT);
            break;

        case KV_LEV1_GRADE:
            return i18n(KV_LEV1_TEXT);
            break;

        case KV_LEV2_GRADE:
            return i18n(KV_LEV2_TEXT);
            break;

        case KV_LEV3_GRADE:
            return i18n(KV_LEV3_TEXT);
            break;

        case KV_LEV4_GRADE:
            return i18n(KV_LEV4_TEXT);
            break;

        case KV_LEV5_GRADE:
            return i18n(KV_LEV5_TEXT);
            break;

        case KV_LEV6_GRADE:
            return i18n(KV_LEV6_TEXT);
            break;

        case KV_LEV7_GRADE:
            return i18n(KV_LEV7_TEXT);
            break;

        default:
            return i18n(KV_LEV1_TEXT);
            break;
    }
}


void LCDStatistics::slotUpdateDisplay(Statistics* stats)
{
    kDebug() << stats->percentCorrect();
    display(stats->percentCorrect());
}

void SvgBarStatistics::slotUpdateDisplay(Statistics*stats)
{
    kDebug() << boundingRect();
    kDebug() << stats->percentCorrect() << "*" << m_backgroundRect.width() << "/" << mapToScene(boundingRect()).boundingRect().width() << "=";
    scale((m_backgroundRect.width() * stats->percentCorrect())/mapToScene(boundingRect()).boundingRect().width(), 1.0);
}



