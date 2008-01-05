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

#include "prefs.h"

#include <keduvocexpression.h>
#include <keduvocgrade.h>

#include <KPassivePopup>
#include <KComboBox>
#include <KDebug>
#include <KGlobalSettings>
#include <KLineEdit>
#include <KLocale>
#include <KIconLoader>
#include <KIcon>
#include <QPainter>

VocabularyDelegate::VocabularyDelegate(QObject *parent) : QItemDelegate(parent)
{}

QWidget * VocabularyDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_UNUSED(option); /// as long as it's unused

    if (!index.isValid())
        return 0;

    switch (index.column()) {


    default: {
        KLineEdit *editor = new KLineEdit(parent);
        editor->setFrame(false);
        editor->setFont(index.model()->data(index, Qt::FontRole).value<QFont>());
        editor->setText(index.model()->data(index, Qt::DisplayRole).toString());
        ///@todo activate the keyboard layout switching code
        /*if (m_doc) {
          QString id = (col == KV_COL_ORG) ? m_doc->originalIdentifier()
            : m_doc->identifier(col - KV_COL_TRANS).name();

          if (langs) {
            QString kbLayout(langs->keyboardLayout(langs->indexShortId(id)));
            if (!kbLayout.isEmpty()) {
                // TODO use generated interface instead
                QDBusInterface kxkb( "org.kde.kxkb", "/kxkb", "org.kde.KXKB" );
                if (kxkb.isValid())
                    kxkb.call( "setLayout", kbLayout );
            }
          }
        }*/
        connect(editor, SIGNAL(returnPressed()), this, SLOT(commitAndCloseEditor()));
        return editor;
    }
    }
}


void VocabularyDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
    if (!index.isValid())
        return;

    switch (index.column()) {
    default: {
        QString value = index.model()->data(index, Qt::DisplayRole).toString();

        KLineEdit *lineEdit = static_cast<KLineEdit*>(editor);
        lineEdit->setText(value);
    }
    }
}


void VocabularyDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
    if (!index.isValid()) {
        return;
    }

    int columnType = VocabularyModel::columnType(index.column());

    switch (columnType) {

    default: {
        KLineEdit *lineEdit = static_cast<KLineEdit*>(editor);
        QString value = lineEdit->text();
        model->setData(index, value);
        /*
        QPair<QString, QString> type;
        type = guessWordType( value, index.column() - KV_COL_TRANS );
        if ( !type.first.isEmpty() ) {
            KPassivePopup* pop = new KPassivePopup(lineEdit);
            pop->setTimeout(2000);
            pop->setView(i18nc("@popupmessage the word has been guessed to be %1 with subtype %2", "Setting word type to %1 (%2).", type.first, type.second), i18nc("@title of a popup", "Noun Detected"));
            pop->show();
        }
        */
    }
    }
}


void VocabularyDelegate::commitAndCloseEditor()
{
    QWidget *editor = qobject_cast<QWidget *>(sender());
    kDebug() << "Committing and closing\n";
    emit commitData(editor);
    emit closeEditor(editor, QAbstractItemDelegate::NoHint);
}


void VocabularyDelegate::setCurrentIndex(const QModelIndex & index)
{
    m_currentIndex = index;
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
