/*
    SPDX-FileCopyrightText: 2009 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MULTIPLECHOICEDATA_H
#define MULTIPLECHOICEDATA_H

#include <QMetaType>
#include <QString>
#include <QStringList>

namespace Practice
{
struct MultipleChoiceData {
    QString question;
    QStringList choices;
};

}

Q_DECLARE_METATYPE(Practice::MultipleChoiceData)

#endif
