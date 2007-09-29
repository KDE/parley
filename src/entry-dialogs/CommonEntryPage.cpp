/***************************************************************************

                       edit common properties

    -----------------------------------------------------------------------

    begin         : Mon Jun 28 21:02:16 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "CommonEntryPage.h"

#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QPixmap>

#include <kdebug.h>
#include <klocale.h>
#include <kicon.h>
#include <kdialog.h>

#include "practice/testentrymanager.h"
#include "docprop-dialogs/UsageOptPage.h"
#include "docprop-dialogs/TypeOptPage.h"

#include "PhoneticEntryPage.h"
#include "EntryDlg.h"

CommonEntryPage::CommonEntryPage(KEduVocDocument *doc, QWidget *parent) : QWidget(parent), m_doc(doc)
{
    setupUi(this);

    m_wordTypes = m_doc->wordTypes();

    // subdialogs
    connect(b_usageDlg, SIGNAL(clicked()), SLOT(invokeUsageDlg()));
    connect(b_pronDlg, SIGNAL(clicked()), SLOT(invokePronDlg()));
    connect(b_TypeDlg, SIGNAL(clicked()), SLOT(invokeTypeDlg()));

    // in general only the modified state interests us.
    connect(usage_box, SIGNAL(itemSelectionChanged()), this, SLOT(slotDataChanged()));
    // to update the usage label
    connect(usage_box, SIGNAL(itemSelectionChanged()), this, SLOT(slotUsageChanged()));
    connect(lesson_box, SIGNAL(activated(int)), SLOT(slotDataChanged(int)));
    connect(subtype_box, SIGNAL(activated(int)), SLOT(slotDataChanged(int)));
    connect(c_active, SIGNAL(stateChanged(int)), SLOT(slotDataChanged(int)));
    connect(pronounce_line, SIGNAL(textChanged(const QString&)), SLOT(slotDataChanged(const QString&)));
    connect(expr_line, SIGNAL(textChanged(const QString&)), SLOT(slotDataChanged(const QString&)));

    connect(type_box, SIGNAL(activated(int)), SLOT(slotDataChanged(int)));
    // type is tricky - need to update subtype
    connect(type_box, SIGNAL(activated(const QString&)), SLOT(slotTypeBoxChanged(const QString&)));


    usage_label->setTitle(i18nc("Usage (area) of an Expression", "&Usage Labels"));
    pronounce_line->setFont(Prefs::iPAFont());

    QIcon list_pm = KIcon("list");

    b_TypeDlg->setIcon(list_pm);
    b_usageDlg->setIcon(list_pm);

    QIcon pron_pm = KIcon("character-set");
    b_pronDlg->setIcon(pron_pm);

    subDialog = 0;

    m_currentTranslation = -1;
}


void CommonEntryPage::setData(const QList<int>& entries, int currentTranslation)
{
    m_currentTranslation = currentTranslation;
    m_entries = entries;

    bool editSingleEntry = (m_entries.count() == 1);
    //disable fields that cannot be changed for multiple, enable them if only one entry is edited
    expr_line->setEnabled(editSingleEntry);
    pronounce_line->setEnabled(editSingleEntry);

    KEduVocExpression *firstEntry = m_doc->entry(m_entries.value(0));
    // set these to the first entry, check if that's ok later
    setLessonBox(firstEntry->lesson());
    setUsageSelection(firstEntry->translation(m_currentTranslation).usages().values());

    c_active->setChecked(firstEntry->isActive());

    updateMainTypeBoxContents();


    QString mainType =
        m_doc->entry(m_entries.value(0))->
            translation(m_currentTranslation).type();
    QString subType =
        m_doc->entry(m_entries.value(0))->
            translation(m_currentTranslation).subType();

    type_box->setCurrentIndex( type_box->findText( mainType ) );
    slotTypeBoxChanged( mainType );
    subtype_box->setCurrentIndex( subtype_box->findText( subType ) );

    if (editSingleEntry) {
        c_active->setTristate(false);
        // these can only be edited in single mode
        expr_line->setText(m_doc->entry(m_entries.value(0))->translation(m_currentTranslation).text());

        pronounce_line->setText(m_doc->entry(m_entries.value(0))->translation(m_currentTranslation).pronunciation());

    } else { // edit more than one entry
        c_active->setTristate(true);
        expr_line->setText("");
        pronounce_line->setText("");

        // fill enabled fields if equal for all edited entries, otherwise empty.
        foreach ( int entry, m_entries) {
            KEduVocExpression *currentEntry = m_doc->entry(entry);
            if ( firstEntry->lesson() != currentEntry->lesson() ) {
                lesson_box->setCurrentIndex(-1);
            }

            /// @todo as soon as we have a .subtype() function use that and check for type differences. Now cheating: only set, if type and subtype are equal.
            if ( firstEntry->translation(m_currentTranslation).type()
                    != currentEntry->translation(m_currentTranslation).type()) {
                type_box->setCurrentIndex(-1);
                subtype_box->setCurrentIndex(-1);
            }
            if ( firstEntry->translation(m_currentTranslation).usages()
                    != currentEntry->translation(m_currentTranslation).usages()) {
                setUsageSelection(QStringList());
                m_usageIsModified = false;
            }
            if ( firstEntry->isActive() != currentEntry->isActive() ) {
                kDebug() << "Setting active box to PartiallyChecked.";
                c_active->setCheckState(Qt::PartiallyChecked);
            }
        } // foreach
    } // edit more than one entry

    m_usageIsModified = false;
}


void CommonEntryPage::setLessonBox(int lesson)
{
    lesson_box->clear();
    /// @todo get rid of lessonNames (?)
    lesson_box->addItems(m_doc->lessonNames());
    lesson_box->setCurrentIndex(lesson);
}


void CommonEntryPage::setUsageSelection(const QStringList& usageList)
{
    usage_box->clear();
    QStringList docUsages = m_doc->usages(); //KVTUsage::getRelation();

    for (int i = 0; i < (int) docUsages.size(); i++) {
        usage_box->addItem(docUsages.value(i));
        if ( usageList.contains(docUsages.value(i))) {
            usage_box->setCurrentRow(i);
        }
    }
    slotUsageChanged();
}


void CommonEntryPage::slotUsageChanged()
{
    QString s;

    for (int i = 0; i < usage_box->count(); i++) {
        if (usage_box->item(i)->isSelected()) {
            if (!s.isEmpty()) {
                s.append('\n');
            }
            s += usage_box->item(i)->text();
        }
    }
    usage_line->setText(s);
    m_usageIsModified = true;
}


void CommonEntryPage::slotTypeBoxChanged(const QString &mainType)
{
    subtype_box->clear();
    subtype_box->addItems( m_wordTypes.subTypeNameList(mainType) );
kDebug() << "fill subType box" << mainType;
    subtype_box->setCurrentIndex(-1);

    emit signalTypeSelected( mainType );
}


void CommonEntryPage::phoneticSelected(wchar_t wc)
{
    pronounce_line->setText( pronounce_line->text() += QChar(wc) );
    emit sigModified();
}


void CommonEntryPage::invokePronDlg()
{
    PhoneticEntryPage * phoneticDlg = new PhoneticEntryPage(Prefs::iPAFont(), this);
    connect(phoneticDlg, SIGNAL(charSelected(wchar_t)), SLOT(phoneticSelected(wchar_t)));
    phoneticDlg->show();
}


void CommonEntryPage::invokeUsageDlg()
{

    QStringList usages;
    for (int i = 0; i < usage_box->count(); i++) {
        if (usage_box->item(i)->isSelected()) {
            usages.append(usage_box->item(i)->text());
        }
    }

    KDialog *subDialog= new KDialog(b_usageDlg);
    subDialog->setCaption(i18nc("usage (area) of an expression", "Edit User-Defined Usage Labels"));
    subDialog->setButtons(KDialog::Ok|KDialog::Cancel);

    subDialog->setDefaultButton(KDialog::Ok);

    connect(subDialog, SIGNAL(finished()), this, SLOT(slotSubDialogClosed()));

    UsageOptPage *usageOptPage = new UsageOptPage(m_doc, this);
    subDialog->setMainWidget(usageOptPage);

    if (subDialog->exec() == QDialog::Accepted) {
        usageOptPage->accept();
        setUsageSelection( usages );
    }
}


void CommonEntryPage::invokeTypeDlg()
{
    KDialog *subDialog = new KDialog(b_TypeDlg);
    subDialog->setCaption(i18n("Edit User Defined Types"));
    subDialog->setButtons(KDialog::Ok|KDialog::Cancel);

    subDialog->setDefaultButton(KDialog::Ok);

    connect(subDialog, SIGNAL(finished()), this, SLOT(slotSubDialogClosed()));

    WordTypeOptionPage *typeOptPage = new WordTypeOptionPage(m_doc, this);
    subDialog->setMainWidget(typeOptPage);

    if (subDialog->exec() == QDialog::Accepted) {
        updateMainTypeBoxContents();
    }
}


void CommonEntryPage::slotSubDialogClosed()
{
    if (subDialog) {
        disconnect(subDialog, SIGNAL(finished()), this, SLOT(slotSubDialogClosed()));
        subDialog->deleteLater();
        subDialog = 0L;
    }
}

void CommonEntryPage::commitData()
{
    if ( m_entries.count() == 1 ) {
    // these things are only changed when editing a single entry
        KEduVocExpression *expr = m_doc->entry(m_entries.value(0));
        if (m_currentTranslation >= 0) {
            expr->translation(m_currentTranslation).setText( expr_line->text() );
            expr->translation(m_currentTranslation).setPronunciation( pronounce_line->text() );
        }
    }

    // things that are changed for multiple or single entries

    foreach(int entry, m_entries) {
        kDebug() << "Changing multiple entries - entry: " << entry;
        KEduVocExpression *expr = m_doc->entry(entry);

        // modified because it can be different for multiple entries and will only be saved if the user changes it. if partially checked no entry will be changed.
        if ( c_active->checkState() != Qt::PartiallyChecked ) {
            expr->setActive( c_active->checkState() == Qt::Checked );
        }
        if ( lesson_box->currentIndex() != -1 ) {
            //m_tableModel->setData(m_tableModel->index(index.m_entries.value(0)(), 0), getLesson(), Qt::EditRole);
            expr->setLesson( lesson_box->currentIndex() );
        }

        if (m_currentTranslation >= 0) {
            if (m_usageIsModified) {
                QSet<QString> usages;
                for (int i = 0; i < usage_box->count(); i++) {
                    if (usage_box->item(i)->isSelected()) {
                        usages.insert(usage_box->item(i)->text());
                    }
                }
                expr->translation(m_currentTranslation).setUsages( usages );
            }
            if ( type_box->currentIndex() != -1 ) {
                QString type = type_box->currentText();
                // set the type
                expr->translation(m_currentTranslation).setType( type_box->currentText() );
                expr->translation(m_currentTranslation).setSubType( subtype_box->currentText() );
                // also set the same type for the other translations
                foreach (int j, expr->translationIndices()) {
                    if (expr->translation(j).type().isEmpty())
                        expr->translation(j).setType( type );
                        ///@todo reset subtype if new type != old type
                }
            } // type
        }
    }
    m_usageIsModified = false;
}

void CommonEntryPage::slotDataChanged()
{
    emit sigModified();
}

void CommonEntryPage::slotDataChanged(int )
{
    emit sigModified();
}

void CommonEntryPage::slotDataChanged(const QString& )
{
    emit sigModified();
}

void CommonEntryPage::updateMainTypeBoxContents()
{
    type_box->clear();
    type_box->addItems( m_wordTypes.typeNameList() );
}


#include "CommonEntryPage.moc"
