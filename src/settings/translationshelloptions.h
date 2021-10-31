/*
    SPDX-FileCopyrightText: 2021 Andreas Cord-Landwehr <cordlandwehr@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef TRANSLATIONSHELLOPTIONS_H
#define TRANSLATIONSHELLOPTIONS_H

#include "translateshelladapter.h"
#include "ui_translationshellpage.h"

class TranslationShellOptions : public QWidget, public Ui::TranslationShellPage
{
    Q_OBJECT
public:
    explicit TranslationShellOptions(QWidget *parent = nullptr);

private slots:
    void runTranslateShellTest();

private:
    TranslateShellAdapter mTranslationShellAdapter;
};

#endif
