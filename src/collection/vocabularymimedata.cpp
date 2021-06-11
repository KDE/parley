/*
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    SPDX-License-Identifier: GPL-2.0-or-later
*/


#include "vocabularymimedata.h"

#include <KEduVocTranslation>


void VocabularyMimeData::setTranslations(const QList<KEduVocTranslation *> &translations)
{
    // list of pointers for drag and drop - for example to assign word types
    m_translations = translations;

    // sort the translations into entries to make deep copies for real copy and paste
    // to only include each expression once
    QList<KEduVocExpression *> expressions;
    foreach(KEduVocTranslation * translation, m_translations) {
        if (!expressions.contains(translation->entry())) {
            expressions.append(translation->entry());
        }
    }

    foreach(KEduVocExpression * expression, expressions) {
        MimeExpression exp;
        // deep copy
        exp.expression = KEduVocExpression(*expression);

        // copy word types
        // this sucks but there is not really a better was. copying pointers is not a good idea because copy and paste can be done between different documents.
        foreach(int i, expression->translationIndices()) {
            // generate text string representation
            m_text.append(expression->translation(i)->text());
            m_text.append(" - ");

            // fill in word types independent of pointers
            KEduVocWordType *type = expression->translation(i)->wordType();

            if (type) { // check if it has a type != 0
                exp.wordTypes[i].grammarType = expression->translation(i)->wordType()->wordType();

                // this may seem weird, but the root element is "word types" so no need to copy that one.
                while (type->parent()) {
                    exp.wordTypes[i].wordType.prepend(type->name());
                    type = static_cast<KEduVocWordType*>(type->parent());
                }
            }
        }
        m_expressions.append(exp);
        m_text.append('\n');
    }
}

QList< KEduVocTranslation * > VocabularyMimeData::translationList() const
{
    return m_translations;
}

QVariant VocabularyMimeData::retrieveData(const QString & mimeType, QVariant::Type type) const
{
    Q_UNUSED(type)
    // only use the expression list.expressions
    // the translation list may be invalid (eg when cut it is no longer valid.
    // translations can only be used internally for drag and drop!!!

    if (mimeType == QLatin1String("text/plain")) {
        return m_text;
    }
    return QVariant();
}

QStringList VocabularyMimeData::formats() const
{
    return QStringList() << QStringLiteral("text/plain");
}

QList< VocabularyMimeData::MimeExpression > VocabularyMimeData::expressionList() const
{
    return m_expressions;
}
