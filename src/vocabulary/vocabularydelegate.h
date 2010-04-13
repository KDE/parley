/***************************************************************************

    Copyright 2006, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
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

#ifndef VOCABULARYDELEGATE_H
#define VOCABULARYDELEGATE_H

#include "basiccontainermodel.h"
#include "../scripts/translator.h"

#include <QItemDelegate>
#include <QModelIndex>

class KEduVocDocument;

namespace Editor {

class VocabularyDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    VocabularyDelegate(QObject *parent = 0);

    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setTranslator(Translator * translator);

public slots:
    void setDocument(KEduVocDocument *doc);

private:
    KEduVocDocument *m_doc;
    Translator * m_translator;

    /** Returns the translations of the word of the given index */
    QSet<QString> getTranslations(const QModelIndex & index) const;

    // for the word type combo
    class WordTypeBasicModel;
};

class VocabularyDelegate::WordTypeBasicModel :public BasicContainerModel
{
public:
    explicit WordTypeBasicModel(QObject *parent = 0);
protected:
    KEduVocContainer * rootContainer() const;
};

}

#endif
