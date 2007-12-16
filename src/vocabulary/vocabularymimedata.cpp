/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "vocabularymimedata.h"

#include <keduvoctranslation.h>

void VocabularyMimeData::addTranslation(KEduVocTranslation * translation)
{
    m_translations.append(translation);
    setText(text() + translation->text() + '\n');
}

QList< KEduVocTranslation * > VocabularyMimeData::translationList() const
{
    return m_translations;
}


#include "vocabularymimedata.moc"


