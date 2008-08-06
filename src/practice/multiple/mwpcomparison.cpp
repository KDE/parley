//
// C++ Implementation: mwpcomparison
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

#include <KRandom>
#include "mwpcomparison.h"
#include <QLineEdit>
#include <QStringList>
#include <QList>
#include <QMap>

void MWPComparison::setup(const QStringList& elementids, const QList<QLineEdit*> kids, const QMap<QString, QLineEdit*> map)
{
    m_kids = kids;
    m_elementIds = elementids;
    m_map = map;
}

void MWPComparison::clear()
{
    kDebug() << "hit";
    foreach(QLineEdit* k, m_kids)
    {
        k->setText("");
    }
}

void MWPComparison::setTexts(const QStringList& texts)
{
    kDebug() << texts;
    if (texts.size() < 3)
    {
        kDebug() << "input data <" << texts << "> is of length" << texts.size() << "; we need length 3";
        return; // bad news
    }

    clear();

    // in this mode we only set one; the provide the other 2
    int r = KRandom::random() % 2;
    kDebug() << r;
    m_map[m_elementIds[r]]->setText(texts[r]);

}

QStringList MWPComparison::answer()
{
    // 0 = absolute, 1 = comparative, 2 = superlative

    QStringList qsl;
    qsl << m_map[m_elementIds[0]]->text() << m_map[m_elementIds[1]]->text() << m_map[m_elementIds[2]]->text();
    kDebug() << qsl;
    return qsl;
}

