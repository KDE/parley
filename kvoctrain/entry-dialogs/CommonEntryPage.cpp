/***************************************************************************

                       edit common properties

    -----------------------------------------------------------------------

    begin         : Mon Jun 28 21:02:16 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
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

#include "query-dialogs/kvtquery.h"
#include <kvtlanguages.h>
#include "DocPropDlg.h"
#include "PhoneticEntryPage.h"
#include "EntryDlg.h"

CommonEntryPage::CommonEntryPage(KEduVocDocument *doc, QWidget *parent) : QWidget(parent), m_doc(doc)
{
    setupUi(this);

    connect(b_usageDlg, SIGNAL(clicked()), SLOT(invokeUsageDlg()));
    connect(b_pronDlg, SIGNAL(clicked()), SLOT(invokePronDlg()));
    connect(b_TypeDlg, SIGNAL(clicked()), SLOT(invokeTypeDlg()));
    connect(usage_box, SIGNAL(itemSelectionChanged()), this, SLOT(slotUsageChanged()));
    connect(lesson_box, SIGNAL(activated(int)), SLOT(slotLessonSelected(int)));
    connect(subtype_box, SIGNAL(activated(int)), SLOT(slotSubTypeSelected(int)));
    connect(type_box, SIGNAL(activated(int)), SLOT(slotTypeSelected(int)));
    connect(c_active, SIGNAL(toggled(bool)), SLOT(slotActiveChanged(bool)));
    connect(pronounce_line, SIGNAL(textChanged(const QString&)), SLOT(slotPronounceSelected(const QString&)));
    connect(expr_line, SIGNAL(textChanged(const QString&)), SLOT(slotExprSelected(const QString&)));

    usage_label->setTitle(i18nc("Usage (area) of an Expression", "&Usage Labels"));
    pronounce_line->setFont(Prefs::iPAFont());

    QIcon list_pm = KIcon("list");
    b_LessDlg->setIcon(list_pm);
    b_LessDlg->setEnabled(false);
    b_TypeDlg->setIcon(list_pm);
    b_usageDlg->setIcon(list_pm);

    QIcon pron_pm = KIcon("character-set");
    b_pronDlg->setIcon(pron_pm);

    subDialog = 0;

    m_currentRow = -1;
    m_currentTranslation = -1;
}


void CommonEntryPage::setData(int row, int col, const QModelIndexList & selection)
{
    m_currentRow = row;
    m_currentTranslation = col;
    m_selection = selection;

    expr_line->setText(m_doc->entry(m_currentRow)->translation(m_currentTranslation).translation());

    m_lesson = m_doc->entry(m_currentRow)->lesson(),
    setLessonBox(m_lesson);

    m_usageCollection = m_doc->entry(m_currentRow)->translation(m_currentTranslation).usageLabel();
    setUsageBox(m_usageCollection);

    m_type = m_doc->entry(m_currentRow)->translation(m_currentTranslation).type();
    setTypeBox(m_type);

    m_pronounce = m_doc->entry(m_currentRow)->translation(m_currentTranslation).pronunciation();
    pronounce_line->setText(m_pronounce);

    m_entry_active = m_doc->entry(m_currentRow)->isActive();
    c_active->setChecked(m_entry_active);

    int start = -1;
    int i = 0;
    while (start < 0 && i < (int) all_types.size()) {
        if (all_types [i].shortStr() == KVTQuery::getMainType(m_type))
            start = i;
        i++;
    }
    int offset = -1;
    while (offset < 0 && i < (int) all_types.size()) {
        if (all_types [i].shortStr() == m_type)
            offset = i - start;
        i++;
    }
    if (offset >= 0) {
        slotSubTypeSelected(offset);
        subtype_box->setCurrentIndex(offset);
    }

    m_largeSelection = (m_selection.count() > 1);
    if (m_largeSelection) {
        expr_line->setText("");
        pronounce_line->setText("");
        lesson_box->setCurrentIndex(-1);
        type_box->setCurrentIndex(-1);
        subtype_box->setCurrentIndex(-1);
    }

    m_usageIsModified = false;
    m_typeIsModified = false;
    m_lessonIsModified = false;
    m_activeIsModified = false;
    setModified(false);
}


void CommonEntryPage::setEnabled(bool enable)
{
    lesson_box->setEnabled(enable);
    c_active->setEnabled(enable);
    if ( m_currentTranslation >= 0 ) {
        usage_box->setEnabled(enable);
        subtype_box->setEnabled(enable);
        type_box->setEnabled(enable);
        b_pronDlg->setEnabled(pronounce_line->isEnabled());
        pronounce_line->setEnabled(enable && !m_largeSelection);
        expr_line->setEnabled(enable && !m_largeSelection);
    } else {
        usage_box->setEnabled(false);
        subtype_box->setEnabled(false);
        type_box->setEnabled(false);
        b_pronDlg->setEnabled(false);
        pronounce_line->setEnabled(false);
        expr_line->setEnabled(false);
    }
}


void CommonEntryPage::setTypeBox(const QString &act_type)
{
    all_types = KVTQuery::getRelation(false);
    all_maintypes = KVTQuery::getRelation(true);

    QString s = KVTQuery::getMainType(act_type)+QM_TYPE_DIV;
    int curr_type = 0;
    type_box->clear();
    type_box->addItem(i18n("<none>"));
    for (int i = 0; i < (int) all_maintypes.size(); i++) {
        type_box->addItem(all_maintypes[i].longStr());
        if (s == all_maintypes[i].shortStr()+QM_TYPE_DIV)
            curr_type = i+1;
    }
    type_box->setCurrentIndex(curr_type);
    slotTypeSelected(curr_type);
}


void CommonEntryPage::setLessonBox(int lesson)
{
    lesson_box->clear();
    //lesson_box->addItem(m_doc->lessonDescription(0));
    lesson_box->addItems(m_doc->lessonDescriptions());

    //if (lesson >= lesson_box->count())
    //  lesson = 0;
    lesson_box->setCurrentIndex(lesson-1);
}


void CommonEntryPage::setUsageBox(const QString & act_usage)
{
    disconnect(usage_box, SIGNAL(itemSelectionChanged()), this, SLOT(slotUsageChanged()));
    usages = KVTUsage::getRelation();
    usage_box->clear();
    for (int i = 0; i < (int) usages.size(); i++) {
        usage_box->addItem(usages[i].longStr());
        if (KVTUsage::contains(QString(usages[i].identStr()), act_usage)) {
            usage_box->setCurrentRow(i);
        }
    }
    connect(usage_box, SIGNAL(itemSelectionChanged()), this, SLOT(slotUsageChanged()));
    slotUsageChanged();
}

void CommonEntryPage::slotUsageChanged()
{
    setModified(true);
    m_usageIsModified = true;
    m_usageCollection = "";
    QString s;

    for (int i = 0; i < usage_box->count(); i++) {
        if (usage_box->item(i)->isSelected()) {
            if (!m_usageCollection.isEmpty())
                m_usageCollection += UL_USAGE_DIV;
            m_usageCollection += usages[i].identStr();

            if (!s.isEmpty())
                s += ", ";
            s += usages[i].shortStr();
        }
    }
    usage_line->setText(s);
}


void CommonEntryPage::slotLessonSelected(int l)
{
    setModified(true);
    m_lessonIsModified = true;
    m_lesson = l+1;
}


void CommonEntryPage::slotActiveChanged(bool state)
{
    setModified(true);
    m_activeIsModified = true;
    m_entry_active = state;
}


void CommonEntryPage::slotExprSelected(const QString& s)
{
    setModified(true);
    m_expression = s;
}


void CommonEntryPage::slotPronounceSelected(const QString& s)
{
    setModified(true);
    m_pronounce = s;
}


void CommonEntryPage::slotSubTypeSelected(int i)
{
    setModified(true);
    m_typeIsModified = true;
    if (i < (int) current_subtypes.size()) {
        m_type = current_subtypes[i];
        emit typeSelected(m_type);
    }
}


void CommonEntryPage::slotTypeSelected(int idx)
{
    setModified(true);
    m_typeIsModified = true;
    subtype_box->clear();
    current_subtypes.clear();
    bool first = true;

    if (idx == 0) { // 0 == none !
        m_type = "";
        emit typeSelected(m_type);
    } else {
        m_type = all_maintypes[idx-1].shortStr();
        emit typeSelected(m_type);
        QString main_patt = all_maintypes[idx-1].shortStr()+QM_TYPE_DIV;
        int sub_idx;
        if (idx-1 < (int) all_types.size()) {
            for (sub_idx = 0; sub_idx < (int) all_types.size(); sub_idx++) {
                if (all_types[sub_idx].shortStr().left(main_patt.length()) == main_patt) {
                    if (first) {
                        subtype_box->addItem(i18n("<none>"));
                        current_subtypes.append(all_maintypes[idx-1].shortStr());
                        first = false;
                    }
                    QString s = all_types[sub_idx].longStr();
                    subtype_box->addItem(s.simplified());
                    current_subtypes.append(all_types[sub_idx].shortStr());
                }
            }
        }
    }

    subtype_box->setEnabled(!first);
    subtype_label->setEnabled(!first);
}


void CommonEntryPage::phoneticSelected(wchar_t wc)
{
    setModified(true);
    m_pronounce += QChar(wc);
    pronounce_line->setText(m_pronounce);
}


void CommonEntryPage::invokePronDlg()
{
    PhoneticEntryPage * phoneticDlg = new PhoneticEntryPage(Prefs::iPAFont(), this);
    connect(phoneticDlg, SIGNAL(charSelected(wchar_t)), SLOT(phoneticSelected(wchar_t)));
    phoneticDlg->show();
}


void CommonEntryPage::invokeUsageDlg()
{
    QList<int> usageIndex;
    QStringList new_usageStr;

    int old_usages = (int) m_doc->usageDescriptions().size();

    KDialog *subDialog= new KDialog(b_usageDlg);
    subDialog->setCaption(i18nc("usage (area) of an expression", "Edit User-Defined Usage Labels"));
    subDialog->setButtons(KDialog::Ok|KDialog::Cancel);

    subDialog->setDefaultButton(KDialog::Ok);

    connect(subDialog, SIGNAL(finished()), this, SLOT(slotSubDialogClosed()));

    UsageOptPage *usageOptPage = new UsageOptPage(m_doc, this);
    subDialog->setMainWidget(usageOptPage);

    if (subDialog->exec() == QDialog::Accepted) {
        usageOptPage->getUsageLabels(new_usageStr, usageIndex);
        UsageOptPage::cleanUnused(m_doc, usageIndex, old_usages);
        KVTUsage::setUsageNames(new_usageStr);
        setUsageBox(m_usageCollection);
        m_doc->setUsageDescriptions(new_usageStr);
        m_doc->setModified();
    }
}


void CommonEntryPage::invokeTypeDlg()
{
    QList<int> typeIndex;
    QStringList new_typeStr;

    int old_types = (int) m_doc->typeDescriptions().size();
    KDialog *subDialog = new KDialog(b_TypeDlg);
    subDialog->setCaption(i18n("Edit User Defined Types"));
    subDialog->setButtons(KDialog::Ok|KDialog::Cancel);

    subDialog->setDefaultButton(KDialog::Ok);

    connect(subDialog, SIGNAL(finished()), this, SLOT(slotSubDialogClosed()));

    TypeOptPage *typeOptPage = new TypeOptPage(m_doc, this);
    subDialog->setMainWidget(typeOptPage);

    if (subDialog->exec() == QDialog::Accepted) {
        typeOptPage->getTypeNames(new_typeStr, typeIndex);
        TypeOptPage::cleanUnused(m_doc, typeIndex, old_types);
        KVTQuery::setTypeNames(new_typeStr);
        setTypeBox(m_type);
        m_doc->setTypeDescriptions(new_typeStr);
        m_doc->setModified();
    }
}


bool CommonEntryPage::isModified()
{
    return modified;
}


void CommonEntryPage::setModified(bool mod)
{
    modified = mod;
    if (mod)
        emit sigModified();
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
    if ( !m_largeSelection ) { // single entry
        KEduVocExpression *expr = m_doc->entry(m_currentRow);
        expr->setActive(m_entry_active);
        expr->setLesson(m_lesson);

        if (m_currentTranslation >= 0) {
            expr->translation(m_currentTranslation).setTranslation(m_expression);
            expr->translation(m_currentTranslation).setPronunciation(m_pronounce);
            expr->translation(m_currentTranslation).setUsageLabel(m_usageCollection);
            expr->translation(m_currentTranslation).setType(m_type);

            for (int j = 0; j < expr->translationCount(); j++) {
                if (expr->translation(j).type().isEmpty())
                    expr->translation(j).setType( m_type );
            }

            for (int j = 0; j < expr->translationCount(); j++) {
                if (KVTQuery::getMainType(expr->translation(j).type())
                        !=
                        KVTQuery::getMainType(m_type))
                    expr->translation(j).setType(m_type);
            }
        }
    } else { // multiple entries (don't change the word itself for example)
        foreach(QModelIndex selIndex, m_selection) {
            //QModelIndex index = m_sortFilterModel->mapToSource(selIndex);

            ///@todo need to use the selection!!!
            KEduVocExpression *expr = m_doc->entry(m_currentRow);

            // modified because it can be different for multiple entries and will only be saved if the user changes it. otherwise it should stay different.
            if (m_activeIsModified) {
                expr->setActive(m_entry_active);
            }
            if (m_lessonIsModified) {
                //m_tableModel->setData(m_tableModel->index(index.m_currentRow(), 0), getLesson(), Qt::EditRole);
                expr->setLesson(m_lesson);
            }
            // only update "common" properties in multiple entries selection mode
            if (m_currentTranslation >= 0) {
                if (m_usageIsModified)
                    for (int j = 0; j < expr->translationCount(); j++)
                        expr->translation(j).setUsageLabel(m_usageCollection);
                if (m_typeIsModified)
                    for (int j = 0; j < expr->translationCount(); j++)
                        expr->translation(j).setType(m_type);
            }
        }
    }
}

#include "CommonEntryPage.moc"
