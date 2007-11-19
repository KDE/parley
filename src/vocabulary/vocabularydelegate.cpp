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
// #define KV_NORM_COLOR      Qt::black

VocabularyDelegate::VocabularyDelegate(QObject *parent) : QItemDelegate(parent)
{}

QWidget * VocabularyDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_UNUSED(option); /// as long as it's unused

    if (!index.isValid())
        return 0;

    int translation = VocabularyModel::translation(index.column());

    switch (index.column()) {
    case 0:
    case 1:
//     {
//         KComboBox *statebox = new KComboBox(parent);
//         statebox->setFrame(false);
//         statebox->setFont(KGlobalSettings::generalFont());
//         connect(statebox, SIGNAL(returnPressed()), this, SLOT(commitAndCloseEditor()));
//         return statebox;
//     }
//     break;

    default: {
        KLineEdit *editor = new KLineEdit(parent);
        editor->setFrame(false);
        editor->setFont(index.model()->data(index, Qt::FontRole).value<QFont>());
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
    case 0:
    case 1:
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
    case 0:
    case 1:

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

void VocabularyDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    if (index.column() == 1) {
        /// @todo a better way to calculate the width of this combobox?
        //QSize sz = editor->sizeHint();
        QRect r = option.rect;
        //sz.setHeight(option.rect.height());
        r.setWidth(150);
        editor->setGeometry(r);
    } else
        editor->setGeometry(option.rect);
}

void VocabularyDelegate::commitAndCloseEditor()
{
    QWidget *editor = qobject_cast<QWidget *>(sender());
    kDebug() << "Committing and closing\n";
    emit commitData(editor);
    emit closeEditor(editor, QAbstractItemDelegate::NoHint);
}

void VocabularyDelegate::drawDisplay(QPainter * painter, const QStyleOptionViewItem & option, const QRect & rect, const QString & text) const
{
    QPen pen = painter->pen();
    QPalette::ColorGroup cg = option.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
    if (option.state & QStyle::State_Selected) {
        painter->fillRect(rect, option.palette.brush(cg, option.state & QStyle::State_HasFocus ?
          QPalette::Base : QPalette::Highlight));
        painter->setPen(option.palette.color(cg, option.state & QStyle::State_HasFocus ?
          QPalette::Text : QPalette::HighlightedText));
    } else {
        painter->setPen(option.palette.color(cg, QPalette::Text));
    }

    if (option.state & QStyle::State_Editing) {
        painter->save();
        painter->setPen(option.palette.color(cg, QPalette::Text));
        painter->drawRect(rect.adjusted(0, 0, -1, -1));
        painter->restore();
    }

    if (text == "@inactive@") {
        painter->drawPixmap((rect.width() / 2) + rect.left() - 8, (rect.height() / 2) + rect.top() - 8, 16, 16, SmallIcon("no"));
    } else if (text == "@active@") {
        painter->drawPixmap((rect.width() / 2) + rect.left() - 8, (rect.height() / 2) + rect.top() - 8, 16, 16, SmallIcon("ok"));
    } else {
        QFont font = painter->font();
        painter->setFont(option.font);
        QRect textRect = rect.adjusted(3, 0, -3, 0); // remove width padding
        QString str = text;

        if (painter->fontMetrics().width(text) > textRect.width() && !text.contains(QLatin1Char('\n')))
            str = elidedText(option.fontMetrics, textRect.width(), option.textElideMode, str);

        painter->drawText(textRect, option.displayAlignment, str);
        painter->setFont(font);
        painter->setPen(pen);
    }
}

void VocabularyDelegate::drawFocus(QPainter * painter, const QStyleOptionViewItem & option, const QRect & rect) const
{
    if (option.state & QStyle::State_HasFocus) {
        painter->save();
        painter->setPen(Qt::black);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect.adjusted(0, 0, -1, -1));
        painter->drawRect(rect.adjusted(1, 1, -2, -2));
        painter->restore();
    }
}

QSize VocabularyDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    return QItemDelegate::sizeHint(option, index);
}

void VocabularyDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
//     if (Prefs::useGradeColors()) {
//         QStyleOptionViewItem opt = option;
//         QColor color = KV_NORM_COLOR;
// 
//         if (index.column() > KV_COL_TRANS) {
//             color = Prefs::gradeColor(0);
//             switch (index.model()->data(index, KVTTableModel::GradeRole).toInt()) {
//             case KV_NORM_GRADE:
//                 color = Prefs::gradeColor(0);
//                 break;
//             case KV_LEV1_GRADE:
//                 color = Prefs::gradeColor(1);
//                 break;
//             case KV_LEV2_GRADE:
//                 color = Prefs::gradeColor(2);
//                 break;
//             case KV_LEV3_GRADE:
//                 color = Prefs::gradeColor(3);
//                 break;
//             case KV_LEV4_GRADE:
//                 color = Prefs::gradeColor(4);
//                 break;
//             case KV_LEV5_GRADE:
//                 color = Prefs::gradeColor(5);
//                 break;
//             case KV_LEV6_GRADE:
//                 color = Prefs::gradeColor(6);
//                 break;
//             case KV_LEV7_GRADE:
//                 color = Prefs::gradeColor(7);
//                 break;
//             default           :
//                 color = Prefs::gradeColor(1);
//             }
//         } else if (index.column() == KV_COL_TRANS) {
//             color = Prefs::gradeColor(0);
//             QList<QVariant> result = index.model()->data(index, KVTTableModel::GradeRole).toList();
// 
//             if (m_currentIndex.column() > 2 && 
//                     (result.size() > (m_currentIndex.column() - 3)) ) {
//                 switch (result[m_currentIndex.column() - 3].toInt()) {
//                 case KV_NORM_GRADE:
//                     color = Prefs::gradeColor(0);
//                     break;
//                 case KV_LEV1_GRADE:
//                     color = Prefs::gradeColor(1);
//                     break;
//                 case KV_LEV2_GRADE:
//                     color = Prefs::gradeColor(2);
//                     break;
//                 case KV_LEV3_GRADE:
//                     color = Prefs::gradeColor(3);
//                     break;
//                 case KV_LEV4_GRADE:
//                     color = Prefs::gradeColor(4);
//                     break;
//                 case KV_LEV5_GRADE:
//                     color = Prefs::gradeColor(5);
//                     break;
//                 case KV_LEV6_GRADE:
//                     color = Prefs::gradeColor(6);
//                     break;
//                 case KV_LEV7_GRADE:
//                     color = Prefs::gradeColor(7);
//                     break;
//                 default           :
//                     color = Prefs::gradeColor(1);
//                 }
//             }
//         }
//         QPalette::ColorGroup cg = QPalette::Normal;
//         opt.palette.setColor(cg, QPalette::Text, color);
//         QItemDelegate::paint(painter, opt, index);
//     } else {
        QItemDelegate::paint(painter, option, index);
//     }
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
