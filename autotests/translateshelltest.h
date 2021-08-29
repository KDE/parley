/*
    SPDX-FileCopyrightText: 2021 Andreas Cord-Landwehr <cordlandwehr@kde.org>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef TRANSLATESHELLTEST_H
#define TRANSLATESHELLTEST_H

#include <QObject>
#include <QTest>

class TranslateShellTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void translationShellOutputParserTest();
    void translateShellProcessInteractionTest();
};

#endif
