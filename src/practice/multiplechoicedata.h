/***************************************************************************
    Copyright 2009 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef MULTIPLECHOICEDATA_H
#define MULTIPLECHOICEDATA_H

#include <QtCore/QStringList>
#include <QtCore/QString>
#include <QtCore/QMetaType>

namespace Practice
{

struct MultipleChoiceData {
    QString question;
    QStringList choices;
};

}

Q_DECLARE_METATYPE(Practice::MultipleChoiceData)

#endif