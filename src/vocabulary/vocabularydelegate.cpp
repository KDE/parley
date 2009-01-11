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


VocabularyDelegate::VocabularyDelegate ( QObject *parent ) : QItemDelegate ( parent )
{
    m_doc = 0;
    m_translator = 0;
}

QSet<QString> VocabularyDelegate::getTranslations ( const QModelIndex & index ) const
{
    if ( Prefs::automaticTranslation() == false ) return QSet<QString>();

    QSet<QString> translations; //translations of this column from all the other languages

    int language = index.column() / VocabularyModel::EntryColumnsMAX;
    QString toLanguage = m_doc->identifier ( language ).locale();

    //iterate through all the Translation columns
    for ( int i = 0; i < index.model()->columnCount ( index.parent() ); i ++ )
    {
        if ( VocabularyModel::columnType ( i ) == VocabularyModel::Translation ) //translation column
        {
            QString fromLanguage = m_doc->identifier ( VocabularyModel::translation ( i ) ).locale();
            QString word = index.model()->index ( index.row(),i,QModelIndex() ).data().toString();

            if ( fromLanguage != toLanguage )
            {
//                 kDebug() << fromLanguage << toLanguage << word;
                //get the word translations and add them to the translations set
                QSet<QString> * tr = m_translator->getTranslation ( word,fromLanguage,toLanguage );
                if ( tr )
                    translations.unite ( * ( tr ) );
            }
        }
    }

    return translations;
}

QWidget * VocabularyDelegate::createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    Q_UNUSED ( option ); /// as long as it's unused

    if ( !index.isValid() )
    {
        return 0;
    }

    switch ( VocabularyModel::columnType ( index.column() ) )
    {
        case VocabularyModel::WordType:
        {
            if ( !m_doc ) return 0;
            KComboBox *wordTypeCombo = new KComboBox ( parent );

            WordTypeBasicModel *basicWordTypeModel = new WordTypeBasicModel ( parent );
            wordTypeCombo->setModel ( basicWordTypeModel );
            QTreeView *view = new QTreeView ( parent );

            view->setModel ( basicWordTypeModel );
            wordTypeCombo->setView ( view );

            view->header()->setVisible ( false );
            view->setRootIsDecorated ( true );

            basicWordTypeModel->setDocument ( m_doc );
            view->expandAll();

            kDebug() << "index data" << index.data().toString();
            //view->setCurrentItem();

            return wordTypeCombo;
        }

        case VocabularyModel::Translation:
        {
            if ( !m_doc || !m_translator ) return 0;

            if ( VocabularyModel::columnType ( index.column() ) == VocabularyModel::Translation )
            {
                //get the translations of this word (fetch only with the help of scripts, if enabled)
                QSet<QString> translations = getTranslations ( index );

                //create combo box
                //if there is only one word and that is the suggestion word (in translations) then don't create the combobox
                if ( !translations.isEmpty() && ! ( translations.size() == 1 && ( *translations.begin() ) == index.model()->data ( index, Qt::DisplayRole ).toString() ) )
                {
                    KComboBox *translationCombo = new KComboBox ( parent );
                    translationCombo->setFrame ( false );
                    translationCombo->addItems ( translations.toList() );
                    translationCombo->setEditable ( true );
                    translationCombo->setFont ( index.model()->data ( index, Qt::FontRole ).value<QFont>() );
                    translationCombo->setEditText ( index.model()->data ( index, Qt::DisplayRole ).toString() );
                    translationCombo->completionObject()->setItems ( translations.toList() );
                    connect ( translationCombo->lineEdit(), SIGNAL ( editingFinished() ), this, SLOT ( commitAndCloseEditor() ) );
                    return translationCombo;
                }
            }
            // no break - we fall back to a line edit if there are not multiple translations fetched online
        }
        default:
        {
            KLineEdit *editor = new KLineEdit ( parent );
            editor->setFrame ( false );
            editor->setFont ( index.model()->data ( index, Qt::FontRole ).value<QFont>() );
            editor->setText ( index.model()->data ( index, Qt::DisplayRole ).toString() );

            QString locale = index.model()->data ( index, VocabularyModel::LocaleRole ).toString();
            if ( !locale.isEmpty() )
            {
                LanguageSettings settings ( locale );
                settings.readConfig();
                QString layout = settings.keyboardLayout();
                if ( !layout.isEmpty() )
                {
                    QDBusInterface kxkb ( "org.kde.kxkb", "/kxkb", "org.kde.KXKB" );
                    if ( kxkb.isValid() )
                    {
                        kxkb.call ( "setLayout", layout );
                    }
                }
            }
            connect ( editor, SIGNAL ( editingFinished() ), this, SLOT ( commitAndCloseEditor() ) );
            return editor;
        }
    }
}

void VocabularyDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const
{
    if ( !index.isValid() ) {
        return;
    }

    switch ( VocabularyModel::columnType ( index.column() ) )
    {
        case ( VocabularyModel::Translation ) :
        {
            QString value = index.model()->data ( index, Qt::DisplayRole ).toString();
            KComboBox * translationCombo = qobject_cast<KComboBox*> ( editor );
            if ( translationCombo )
            {
                translationCombo->setEditText ( value );
                if (value.isEmpty()) {
                    // show the translations that were fetched as popup
                    translationCombo->showPopup();
                }
                break;
            }
        }
        default:
        {
            QString value = index.model()->data ( index, Qt::DisplayRole ).toString();

            KLineEdit *lineEdit = qobject_cast<KLineEdit*> ( editor );
            if ( lineEdit ) {
                lineEdit->setText ( value );
            }
        }
    }
}

void VocabularyDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
{
    if ( !index.isValid() ) {
        return;
    }

    switch ( VocabularyModel::columnType ( index.column() ) )
    {
        case ( VocabularyModel::WordType ) :
        {
            kDebug() << "word type editor";
            KComboBox *combo = qobject_cast<KComboBox*> ( editor );
            if ( !combo ) {
                return;
            }
            kDebug() << "combo" << combo->currentText();
            QModelIndex comboIndex = combo->view()->currentIndex();
            KEduVocWordType* wordType = static_cast<KEduVocWordType*> ( comboIndex.internalPointer() );

            // the root is the same as no word type
            if ( wordType && wordType->parent() == 0 )
            {
                wordType = 0;
            }

            VocabularyFilter *filter = qobject_cast<VocabularyFilter*> ( model );
            VocabularyModel *vocModel = qobject_cast<VocabularyModel*> ( ( filter )->sourceModel() );
            Q_ASSERT ( vocModel );
            QVariant data = vocModel->data ( filter->mapToSource ( index ), VocabularyModel::EntryRole );

            KEduVocExpression *expression = data.value<KEduVocExpression*>();
            Q_ASSERT ( expression );
            int translationId = VocabularyModel::translation ( index.column() );

            expression->translation ( translationId )->setWordType ( wordType );

        }
        case ( VocabularyModel::Translation ) :
        {
            KComboBox * translationCombo = qobject_cast<KComboBox*> ( editor );
            if ( translationCombo )
            {
                model->setData ( index,translationCombo->currentText() );
                break;
            }
        }
        default:
        {
            KLineEdit *lineEdit = qobject_cast<KLineEdit*> ( editor );
            if ( lineEdit )
            {
                model->setData ( index, lineEdit->text() );
            }            
        }
    }
}

void VocabularyDelegate::commitAndCloseEditor()
{
    kDebug() << "Committing and closing delegate";
    QWidget *editor = qobject_cast<QWidget *> ( sender() );
    if (editor) {
        editor->setFocus();
    }
    emit commitData ( editor );
    emit closeEditor ( editor, QAbstractItemDelegate::EditNextItem );
}

void VocabularyDelegate::setCurrentIndex ( const QModelIndex & index )
{
    m_currentIndex = index;
}

void VocabularyDelegate::setDocument ( KEduVocDocument * doc )
{
    m_doc = doc;
}

/*
QPair< QString, QString > VocabularyDelegate::guessWordType(const QString & entry, int language) const
{
    kDebug() << "guessing word type for: " << entry;

    QString article = entry.section(" ", 0, 0);
    if ( article.length() < entry.length() ) {
        if ( article == ->identifier(language).articles().article(KEduVocWordFlag::Singular| KEduVocWordFlag::Definite| KEduVocWordFlag::Masculine) ) {
            kDebug() << "Noun masculine";
            return qMakePair(m_doc->wordTypes().specialTypeNoun(), m_doc->wordTypes().specialTypeNounMale());
        }

    }
    return qMakePair(QString(), QString());
}
*/


VocabularyDelegate::WordTypeBasicModel::WordTypeBasicModel ( QObject * parent )
        :BasicContainerModel ( KEduVocContainer::WordType, parent )
{
}

KEduVocContainer * VocabularyDelegate::WordTypeBasicModel::rootContainer() const
{
    if ( !m_doc ) {
        return 0;
    }
    return m_doc->wordTypeContainer();
}

/**
 * Sets the member variable m_translator to a Translator object
 * @param translator Translator Object to be used for retreiving word translations
 */
void VocabularyDelegate::setTranslator ( Translator* translator )
{
    m_translator = translator;
}

#include "vocabularydelegate.moc"
