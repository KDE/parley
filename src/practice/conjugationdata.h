/***************************************************************************
    Copyright 2010 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


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
