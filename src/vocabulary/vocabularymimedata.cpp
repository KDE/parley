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

void VocabularyMimeData::setTranslations(QList<KEduVocTranslation *> translations)
{
    // list of pointers for drag and drop - for example to assign word types
    m_translations = translations;

    // sort the translations into entries to make deep copies for real copy and paste
    // to only include each expression once
    QList<KEduVocExpression *> expressions;
    foreach (KEduVocTranslation * translation, m_translations) {
        if (!expressions.contains(translation->entry())) {
            expressions.append(translation->entry());
        }
    }

foreach (KEduVocExpression * expression, expressions) {
            // deep copy
            m_expressions.append(KEduVocExpression(*expression));
}
}

QList< KEduVocTranslation * > VocabularyMimeData::translationList() const
{
    return m_translations;
}

QVariant VocabularyMimeData::retrieveData(const QString & mimeType, QVariant::Type type) const
{
    // only use the expression list.
    // the translation list may be invalid (eg when cut it is no longer valid.
    // translations can only be used internally for drag and drop!!!
    if (mimeType == "text/plain") {
        QString text;
        for (int j = 0; j < m_expressions.size(); j++) {
            QList<int>::const_iterator i;
            for (i = m_expressions.value(j).translationIndices().begin(); i != m_expressions.value(j).translationIndices().end(); i++) {
                if (m_expressions.value(j).translation(*i)) {
                    text.append(m_expressions.value(j).translation(*i)->text());
                    text.append(" - ");
                }
            }
            text.append('\n');
        }
        return text;
    }

    return QVariant();
}

QStringList VocabularyMimeData::formats() const
{
    return QStringList() << "text/plain";
}

QList< KEduVocExpression > VocabularyMimeData::expressionList() const
{
    return m_expressions;
}


#include "vocabularymimedata.moc"
