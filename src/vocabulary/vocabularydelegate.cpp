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

#include "vocabularydelegate.h"
#include "vocabularymodel.h"
#include "vocabularyfilter.h"

#include "prefs.h"
#include "languagesettings.h"
#include "basiccontainermodel.h"

#include <keduvocexpression.h>
#include <keduvocwordtype.h>
#include <KPassivePopup>
#include <KComboBox>
#include <KDebug>
#include <KLineEdit>
#include <KLocale>
#include <QTreeView>
#include <QHeaderView>
#include <QDBusInterface>

VocabularyDelegate::VocabularyDelegate(QObject *parent) : QItemDelegate(parent)
{
    m_doc = 0;
}

QWidget * VocabularyDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_UNUSED(option); /// as long as it's unused

    if (!index.isValid()) {
        return 0;
    }

    switch (VocabularyModel::columnType(index.column())) {
    case VocabularyModel::WordType: {
        if (!m_doc) return 0;
        KComboBox *wordTypeCombo = new KComboBox(parent);

        BasicContainerModel *basicWordTypeModel = new BasicContainerModel(KEduVocContainer::WordType, parent);
        wordTypeCombo->setModel(basicWordTypeModel);
        QTreeView *view = new QTreeView(parent);

        view->setModel(basicWordTypeModel);
        wordTypeCombo->setView(view);

        view->header()->setVisible(false);
        view->setRootIsDecorated(true);

        basicWordTypeModel->setDocument(m_doc);
        view->expandAll();

        kDebug() << "index data" << index.data().toString();
        //view->setCurrentItem();

        return wordTypeCombo;
    }

    default: {

        KLineEdit *editor = new KLineEdit(parent);
        editor->setFrame(false);
        editor->setFont(index.model()->data(index, Qt::FontRole).value<QFont>());
        editor->setText(index.model()->data(index, Qt::DisplayRole).toString());

        QString locale = index.model()->data(index, VocabularyModel::LocaleRole).toString();
        if(!locale.isEmpty()) {
            LanguageSettings settings(locale);
            settings.readConfig();
            QString layout = settings.keyboardLayout();
            if(!layout.isEmpty()) {
                QDBusInterface kxkb( "org.kde.kxkb", "/kxkb", "org.kde.KXKB" );
                if (kxkb.isValid()) {
                kxkb.call( "setLayout", layout );
                }
            }
        }
        connect(editor, SIGNAL(returnPressed()), this, SLOT(commitAndCloseEditor()));
        return editor;
    }
    }
}

void VocabularyDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
    if (!index.isValid()) {
        return;
    }

    switch (VocabularyModel::columnType(index.column())) {
    default: {
        QString value = index.model()->data(index, Qt::DisplayRole).toString();

        KLineEdit *lineEdit = qobject_cast<KLineEdit*>(editor);
        if (lineEdit) {
            lineEdit->setText(value);
        }
        }
    }
}

void VocabularyDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
    if (!index.isValid()) {
        return;
    }

    switch (VocabularyModel::columnType(index.column())) {
    case (VocabularyModel::WordType): {
        kDebug() << "word type editor";
        KComboBox *combo = qobject_cast<KComboBox*>(editor);
        if (!combo) {
            return;
        }
        kDebug() << "combo" << combo->currentText();
        QModelIndex comboIndex = combo->view()->currentIndex();
        KEduVocWordType* wordType = static_cast<KEduVocWordType*>(comboIndex.internalPointer());

        // the root is the same as no word type
        if (wordType && wordType->parent() == 0) {
            wordType = 0;
        }

        VocabularyFilter *filter = qobject_cast<VocabularyFilter*>(model);
        VocabularyModel *vocModel = qobject_cast<VocabularyModel*>((filter)->sourceModel());
Q_ASSERT(vocModel);
        QVariant data = vocModel->data(filter->mapToSource(index), VocabularyModel::EntryRole);

        KEduVocExpression *expression = data.value<KEduVocExpression*>();
Q_ASSERT(expression);
        int translationId = VocabularyModel::translation(index.column());

        expression->translation(translationId)->setWordType(wordType);

    }
    default: {
        KLineEdit *lineEdit = qobject_cast<KLineEdit*>(editor);
        if (!lineEdit) {
            return;
        }
        QString value = lineEdit->text();
        model->setData(index, value);
    }
    }
}

void VocabularyDelegate::commitAndCloseEditor()
{
    QWidget *editor = qobject_cast<QWidget *>(sender());
    kDebug() << "Committing and closing\n";
    emit commitData(editor);
    emit closeEditor(editor, QAbstractItemDelegate::EditNextItem);
}

void VocabularyDelegate::setCurrentIndex(const QModelIndex & index)
{
    m_currentIndex = index;
}

void VocabularyDelegate::setDocument(KEduVocDocument * doc)
{
    m_doc = doc;
}

/*
QPair< QString, QString > VocabularyDelegate::guessWordType(const QString & entry, int language) const
{
    kDebug() << "guessing word type for: " << entry;

    QString article = entry.section(" ", 0, 0);
    if ( article.length() < entry.length() ) {
        if ( article == ->identifier(language).articles().article(KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Masculine) ) {
            kDebug() << "Noun male";
            return qMakePair(m_doc->wordTypes().specialTypeNoun(), m_doc->wordTypes().specialTypeNounMale());
        }

    }
    return qMakePair(QString(), QString());
}
*/


#include "vocabularydelegate.moc"
