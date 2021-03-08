/*
    SPDX-FileCopyrightText: 2010 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/


#ifndef CONJUGATIONDATA_H
#define CONJUGATIONDATA_H

#include <QStringList>
#include <QString>
#include <QMetaType>

#include <KEduVocConjugation>

namespace Practice
{

struct ConjugationData {
    QString questionInfinitive;
    QString solutionInfinitive;
    QString tense;
    QStringList personalPronouns;
};

}

Q_DECLARE_METATYPE(Practice::ConjugationData)

#endif
