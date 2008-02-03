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
#include <KDebug>

void VocabularyMimeData::addTranslation(KEduVocTranslation * translation)
{
    ///@todo the order is important ... also if they have the same parent entry...
    m_translations.append(translation);

}

QList< KEduVocTranslation * > VocabularyMimeData::translationList() const
{
    return m_translations;
}

QVariant VocabularyMimeData::retrieveData(const QString & mimeType, QVariant::Type type) const
{
    if (mimeType == "text/plain") {
        QString text;
        foreach (KEduVocTranslation * translation, m_translations) {
            text.append(translation->text());
        }
        return text;
    }
    // maybe this makes sense somewhere... also put translations belonging together into the same row would be nice ;)
    if (mimeType == "text/html") {
        QString text;
        text.append("<table>");
        foreach (KEduVocTranslation * translation, m_translations) {
            text.append(QString("<tr><td>%1</td></tr>").arg(translation->text()));
        }
        text.append("</table>");
        return text;
    }
    return QVariant();
}

QStringList VocabularyMimeData::formats() const
{
    return QStringList() << "text/plain";
}


#include "vocabularymimedata.moc"
