/*
    SPDX-FileCopyrightText: 2010 Daniel Laidig <laidig@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "statustoggle.h"
#include "themedbackgroundrenderer.h"
#include "statustogglebutton.h"

#include <KLocalizedString>

#include <QAction>

using namespace Practice;

StatusToggle::StatusToggle(QWidget* parent)
    : ImageWidget(parent), m_renderer(0), m_resultState(AbstractFrontend::QuestionState)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setScalingEnabled(false);
    setMinimumSize(minimumSizeHint());
    setMaximumSize(minimumSizeHint());

    QSize size = minimumSizeHint();
    size *= 0.4;
    m_toggle = new StatusToggleButton(this);
    m_toggle->setMinimumSize(size);
    m_toggle->setMaximumSize(size);
    m_toggle->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_toggle->setScalingEnabled(false);
    m_toggle->move(width() - m_toggle->width(), height() - m_toggle->height());

    connect(m_toggle, &StatusToggleButton::clicked, this, &StatusToggle::toggle);
}

void StatusToggle::setRenderer(ThemedBackgroundRenderer *renderer)
{
    m_renderer = renderer;

    QSize size = minimumSizeHint();

    m_defaultStatus = m_renderer->getPixmapForId(QStringLiteral("status-default"), size);
    m_correctStatus = m_renderer->getPixmapForId(QStringLiteral("status-correct"), size);
    m_wrongStatus = m_renderer->getPixmapForId(QStringLiteral("status-wrong"), size);

    size = size * 0.4;

    m_toggleCorrect = m_renderer->getPixmapForId(QStringLiteral("toggle-correct"), size);
    m_toggleCorrectHover = m_renderer->getPixmapForId(QStringLiteral("toggle-correct-hover"), size);
    m_toggleCorrectPressed = m_renderer->getPixmapForId(QStringLiteral("toggle-correct-pressed"), size);

    m_toggleWrong = m_renderer->getPixmapForId(QStringLiteral("toggle-wrong"), size);
    m_toggleWrongHover = m_renderer->getPixmapForId(QStringLiteral("toggle-wrong-hover"), size);
    m_toggleWrongPressed = m_renderer->getPixmapForId(QStringLiteral("toggle-wrong-pressed"), size);

    updatePixmap();
    updateToggle();
}

QSize StatusToggle::minimumSizeHint() const
{
    return QSize(128, 128);
}

void StatusToggle::setResultState(AbstractFrontend::ResultState state)
{
    if (m_resultState == state)
        return;
    m_resultState = state;
    switch (m_resultState) {
    case AbstractFrontend::QuestionState:
    case AbstractFrontend::AnswerSynonym:
        setToolTip(QString());
        m_toggle->setToolTip(QString());
        break;
    case AbstractFrontend::AnswerCorrect:
        setToolTip(i18n("This answer will be counted as correct.\nWords will only be counted as correct if they are answered correctly on the first attempt."));
        m_toggle->setToolTip(i18n("Count this answer as wrong"));
        break;
    case AbstractFrontend::AnswerWrong:
        setToolTip(i18n("This answer will be counted as wrong.\nWords will only be counted as correct if they are answered correctly on the first attempt."));
        m_toggle->setToolTip(i18n("Count this answer as correct"));
        break;
    default:
        setPixmap(QPixmap());
    }
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
