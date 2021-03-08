/*
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef VOCABULARYMIMEDATA_H
#define VOCABULARYMIMEDATA_H

#include <KEduVocExpression>
#include <KEduVocWordtype>
#include <QMimeData>

class KEduVocTranslation;


class VocabularyMimeData : public QMimeData
{
    Q_OBJECT
public:
    struct MimeExpression {
        struct WordTypeStrings {
            QStringList wordType; // better have this hierarchical
            KEduVocWordFlags grammarType;
        };

        // all data as deep copy
        KEduVocExpression expression;
        // the word types cannot be copied, thus per translation (int) save parent types and the type
        QMap<int, WordTypeStrings> wordTypes;
    };

    void setTranslations(const QList<KEduVocTranslation*> &translation);
    QList<KEduVocTranslation *> translationList() const;
    QList<VocabularyMimeData::MimeExpression> expressionList() const;

    QVariant retrieveData(const QString & mimeType, QVariant::Type type) const Q_DECL_OVERRIDE;

    QStringList formats() const Q_DECL_OVERRIDE;


private:
    QList<KEduVocTranslation*> m_translations;
    QList <MimeExpression> m_expressions;
    QString m_text;
};


#endif
