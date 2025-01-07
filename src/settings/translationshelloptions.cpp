/*
    SPDX-FileCopyrightText: 2021 Andreas Cord-Landwehr <cordlandwehr@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "translationshelloptions.h"
#include <KLed>
#include <KLocalizedString>

using namespace Qt::Literals::StringLiterals;

TranslationShellOptions::TranslationShellOptions(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    KLed *installStatusLed = new KLed(this);
    installStatusLed->setLook(KLed::Look::Flat);
    QLabel *installStatusText = new QLabel(this);
    if (mTranslationShellAdapter.isTranslateShellAvailable()) {
        installStatusLed->setState(KLed::State::On);
        installStatusText->setText(i18n("translate-shell is available."));
    } else {
        installStatusLed->setState(KLed::State::Off);
        installStatusText->setText(i18n("translate-shell executable could not be found."));
        buttonRunTest->setEnabled(false);
    }
    formLayoutInstalledState->insertRow(0, installStatusLed, installStatusText);

    connect(buttonRunTest, &QToolButton::clicked, this, &TranslationShellOptions::runTranslateShellTest);
    labelResultArea->setText(u"..."_s);
}

void TranslationShellOptions::runTranslateShellTest()
{
    auto translationResult = mTranslationShellAdapter.translate(u"parley"_s, u"en"_s, u"de"_s);
    if (translationResult.m_suggestions.count() > 0) {
        labelResultArea->setText(translationResult.m_suggestions.first());
    } else {
        labelResultArea->setText(i18n("Error"));
    }
}

#include "moc_translationshelloptions.cpp"
