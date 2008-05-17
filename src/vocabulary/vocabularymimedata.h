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

#ifndef VOCABULARYMIMEDATA_H
#define VOCABULARYMIMEDATA_H

#include <keduvocexpression.h>
#include <QMimeData>

class KEduVocTranslation;

class VocabularyMimeData :public QMimeData {
    Q_OBJECT
public:
    void setTranslations(QList<KEduVocTranslation*> translation);
    QList<KEduVocTranslation *> translationList() const;
    QList<KEduVocExpression> expressionList() const;

    QVariant retrieveData ( const QString & mimeType, QVariant::Type type ) const;

    QStringList formats () const;

private:
    QList<KEduVocTranslation*> m_translations;
    QList<KEduVocExpression> m_expressions;
};


#endif
