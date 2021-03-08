/*
    SPDX-FileCopyrightText: 2009 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/


#ifndef MULTIPLECHOICEDATA_H
#define MULTIPLECHOICEDATA_H

#include <QStringList>
#include <QString>
#include <QMetaType>

namespace Practice
{

struct MultipleChoiceData {
    QString question;
    QStringList choices;
};

}

Q_DECLARE_METATYPE(Practice::MultipleChoiceData)

#endif
