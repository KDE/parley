/***************************************************************************
    Copyright 2010 Daniel Laidig <laidig@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "statustoggle.h"
#include "themedbackgroundrenderer.h"
#include "abstractfrontend.h"
#include "statustogglebutton.h"

using namespace Practice;

StatusToggle::StatusToggle(QWidget* parent)
    : ImageWidget(parent), m_renderer(0), m_resultState(AbstractFrontend::QuestionState)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setScalingEnabled(false);
    setMinimumSize(minimumSizeHint());
    setMaximumSize(minimumSizeHint());

    QSize size = minimumSizeHint();
    size*=0.4;
    m_toggle = new StatusToggleButton(this);
    m_toggle->setMinimumSize(size);
    m_toggle->setMaximumSize(size);
    m_toggle->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_toggle->setScalingEnabled(false);
    m_toggle->move(width()-m_toggle->width(), height()-m_toggle->height());

    connect(m_toggle, SIGNAL(clicked()), this, SIGNAL(toggle()));
}

void StatusToggle::setRenderer(ThemedBackgroundRenderer *renderer)
{
    m_renderer = renderer;

    QSize size = minimumSizeHint();

    m_defaultStatus = m_renderer->getPixmapForId("status-default", size);
    m_correctStatus = m_renderer->getPixmapForId("status-correct", size);
    m_wrongStatus = m_renderer->getPixmapForId("status-wrong", size);

    size = size*0.4;

    m_toggleCorrect = m_renderer->getPixmapForId("toggle-correct", size);
    m_toggleCorrectHover = m_renderer->getPixmapForId("toggle-correct-hover", size);
    m_toggleCorrectPressed = m_renderer->getPixmapForId("toggle-correct-pressed", size);

    m_toggleWrong = m_renderer->getPixmapForId("toggle-wrong", size);
    m_toggleWrongHover = m_renderer->getPixmapForId("toggle-wrong-hover", size);
    m_toggleWrongPressed = m_renderer->getPixmapForId("toggle-wrong-pressed", size);

    updatePixmap();
    updateToggle();
}

QSize StatusToggle::minimumSizeHint() const
{
    return QSize(128,128);
}

void StatusToggle::setResultState(AbstractFrontend::ResultState state)
{
    if (m_resultState == state)
        return;
    m_resultState = state;
    updatePixmap();
    updateToggle();
}

void StatusToggle::updatePixmap()
{
    switch (m_resultState) {
    case AbstractFrontend::QuestionState:
    case AbstractFrontend::AnswerSynonym:
        setPixmap(m_defaultStatus);
        break;
    case AbstractFrontend::AnswerCorrect:
        setPixmap(m_correctStatus);
        break;
    case AbstractFrontend::AnswerWrong:
        setPixmap(m_wrongStatus);
        break;
    default:
        setPixmap(QPixmap());
    }
}

void StatusToggle::updateToggle()
{
    switch (m_resultState) {
    case AbstractFrontend::AnswerCorrect:
        m_toggle->setPixmaps(m_toggleWrong, m_toggleWrongHover, m_toggleWrongPressed);
        break;
    case AbstractFrontend::AnswerWrong:
        m_toggle->setPixmaps(m_toggleCorrect, m_toggleCorrectHover, m_toggleCorrectPressed);
        break;
    default:
        m_toggle->setPixmaps(QPixmap(), QPixmap(), QPixmap());
    }
}

#include "statustoggle.moc"