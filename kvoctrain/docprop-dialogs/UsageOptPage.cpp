/***************************************************************************

                   usage labels dialog page

    -----------------------------------------------------------------------

    begin         : Thu Mar 30 20:38:31 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include "UsageOptPage.h"

#include <QGroupBox>
#include <QPushButton>

#include <kinputdialog.h>
#include <kmessagebox.h>
#include <klocale.h>

#include <kvtquery.h>

#define USAGE_TAG ". "

UsageOptPage::UsageOptPage(KEduVocDocument *_doc, QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    titleGroupBox->setTitle(i18n("Usage Labels"));

    connect(optionsList,   SIGNAL(currentRowChanged(int)), this, SLOT(slotUsageChosen(int)));
    connect(cleanUpButton, SIGNAL(clicked()),              this, SLOT(slotCleanup()));
    connect(deleteButton,  SIGNAL(clicked()),              this, SLOT(slotDeleteUsage()));
    connect(modifyButton,  SIGNAL(clicked()),              this, SLOT(slotModifyUsage()));
    connect(newButton,     SIGNAL(clicked()),              this, SLOT(slotNewUsage()));

    doc = _doc;

    int i = 1;
    foreach(const QString &usageName, doc->usageDescriptions()) {
        optionsList->addItem(QString("%1").arg(i++, 2).append(USAGE_TAG).append(usageName));
        usageIndex.append(i);
    }

    m_currentUsage = 0;
    if (optionsList->count() > 0)
        optionsList->setCurrentRow(m_currentUsage);

    modifyButton->setEnabled(optionsList->count() > 0);
    deleteButton->setEnabled(optionsList->count() > 0);

    optionsList->setFocus();
}


void UsageOptPage::slotUsageChosen(int index)
{
    m_currentUsage = index;
}


void UsageOptPage::slotNewUsage()
{
    bool ok;
    QString getUsage = KInputDialog::getText(i18nc("usage (area) of an expression", "Usage Description"), i18n("Enter usage description:"), QString(), &ok, this);
    if (!ok)
        return;

    QString str;
    int i = optionsList->count() + 1;
    optionsList->addItem(QString("%1").arg(i, 2).append(USAGE_TAG).append(getUsage.simplified()));
    usageIndex.append(-(i - 1));

    m_currentUsage = optionsList->count();
    optionsList->setCurrentRow(i - 1);
    modifyButton->setEnabled(true);
    deleteButton->setEnabled(true);
}


void UsageOptPage::slotModifyUsage()
{
    if (optionsList->count() > 0 && (int) optionsList->count() > m_currentUsage) {
        QString str = optionsList->item(m_currentUsage)->text();
        str = str.mid(str.indexOf(USAGE_TAG) + QString(USAGE_TAG).length());

        bool ok;
        QString getUsage = KInputDialog::getText(i18nc("usage (area) of an expression", "Usage Description"), i18n("Enter usage description:"), str, &ok, this);
        if (!ok)
            return;

        int i = m_currentUsage + 1;
        optionsList->item(m_currentUsage)->setText(QString("%1").arg(i, 2).append(USAGE_TAG).append(getUsage.simplified()));
    }
}


void UsageOptPage::updateListBox(int start)
{
    QString str;
    for (int i = start; i < (int) optionsList->count(); i++) {
        str = optionsList->item(i)->text();
        str = str.mid(str.indexOf(USAGE_TAG) + QString(USAGE_TAG).length());
        optionsList->item(i)->setText(QString("%1").arg(i + 1, 2).append(USAGE_TAG).append(str));
    }
}


void UsageOptPage::slotDeleteUsage()
{
    int act = m_currentUsage;
    if (optionsList->count() != 0 && (int) optionsList->count() > act) {
        QString t;
        t.setNum(usageIndex[m_currentUsage] - 1).prepend(UL_USER_USAGE).append(UL_USAGE_DIV);
        for (int ent = 0; ent < doc->entryCount(); ent++) {
            KEduVocExpression *exp = doc->entry(ent);
            for (int lang = 0; lang < doc->identifierCount(); lang++) {
                QString ul = exp->usageLabel(lang).append(UL_USAGE_DIV);
                if (ul.indexOf(t) >= 0) {
                    KMessageBox::information(this, i18nc("usage (area) of an expression", "This user-defined usage label could not be deleted\n"
                                                         "because it is in use."), i18nc("usage (area) of an expression", "Deleting Usage Label"));
                    return;
                }
            }
        }

        delete optionsList->takeItem(act);
        usageIndex.erase(usageIndex.begin() + act);

        if ((int) optionsList->count() <= act)
            act = optionsList->count() - 1;
        else
            updateListBox(act); // update items after current

        if (act >= 0)
            optionsList->setCurrentRow(act);
    }
    modifyButton->setEnabled(optionsList->count() > 0);
    deleteButton->setEnabled(optionsList->count() > 0);
}


void UsageOptPage::slotCleanup()
{
    QVector<bool> used_usage;
    for (int i = 0; i <= (int) optionsList->count(); i++)
        used_usage.append(false);

    for (int col = 0; col < doc->identifierCount(); col++)
        for (int i = 0; i < (int) doc->entryCount(); i++) {
            QString t = doc->entry(i)->usageLabel(col);
            QString n;
            while (t.left(QString(UL_USER_USAGE).length()) == UL_USER_USAGE) {
                t.remove(0, QString(UL_USER_USAGE).length());

                int next;
                if ((next = t.indexOf(UL_USAGE_DIV)) >= 0) {
                    n = t.left(next);
                    t.remove(0, next + 1);
                } else {
                    n = t;
                    t = "";
                }

                int idx = n.toInt();
                if ((int) used_usage.count() < idx)
                    used_usage.resize(idx);
                if (idx > 0)
                    used_usage[idx - 1] = true ;
            }
        }

    for (int i = used_usage.count() - 1; i >= 0; i--) {
        if (!used_usage[i]) {
            for (int u = 0; u < (int) usageIndex.count() ; u++) {
                if (usageIndex[u] == i || usageIndex[u] < 0) {
                    m_currentUsage = i;
                    slotDeleteUsage();
                    break;
                }
            }
        }
    }

    m_currentUsage = 0;
    optionsList->setCurrentRow(m_currentUsage);
}


void UsageOptPage::getUsageLabels(QStringList &ret_usage, QList<int> &ret_Index)
{
    ret_usage.clear();

    QString str;
    for (int i = 0; i < (int) optionsList->count(); i++) {
        str = optionsList->item(i)->text();
        ret_usage.append(str.mid(str.indexOf(USAGE_TAG) + QString(USAGE_TAG).length()));
    }
    ret_Index = usageIndex;
}

void UsageOptPage::cleanUnused(KEduVocDocument *doc, const QList<int> &usageIndex, int old_usages)
{
    QList<int> translate_index;
    QStringList new_usageStr;

    /////////////////////////////////////////////////////
    // translate_index contains new index number for each
    // old index
    for (int i = 0; i <= qMax(old_usages, (int) usageIndex.size()); i++)
        translate_index.append(0);

    // now adjust lesson descriptions to new index
    for (int i = 0; i < (int) usageIndex.count(); i++) {
        if (usageIndex[i] >= 0)
            translate_index[usageIndex[i] + 1] = i + 1;
    }

    // only keep remaining usage indices

    // set usage index to 0 when not needed any more
    // and translate to new index
    for (int col = 0; col < doc->identifierCount(); col++) {
        for (int i = 0; i < doc->entryCount(); i++) {
            QString t = doc->entry(i)->usageLabel(col);
            if (!t.isEmpty() && t.left(QString(UL_USER_USAGE).length()) == UL_USER_USAGE) {
                QString tg;
                while (t.left(QString(UL_USER_USAGE).length()) == UL_USER_USAGE) {
                    QString n;
                    t.remove(0, QString(UL_USER_USAGE).length());
                    int next;
                    if ((next = t.indexOf(UL_USAGE_DIV)) >= 0) {
                        n = t.left(next);
                        t.remove(0, next + 1);
                    } else {
                        n = t;
                        t = "";
                    }

                    QString newusage;
                    int o = n.toInt();
                    if (translate_index[o] > 0) {
                        newusage.setNum(translate_index[o]).prepend(UL_USER_USAGE);
                    }

                    if (tg.length() == 0)
                        tg = newusage;
                    else
                        if (newusage.length())
                            tg.append(UL_USAGE_DIV).append(newusage);
                }

                if (tg.length() == 0)
                    tg = t;
                else if (t.length() > 0)
                    tg.append(UL_USAGE_DIV).append(t);

                doc->entry(i)->setUsageLabel(col, tg);
            }
        }
    }
}

#include "UsageOptPage.moc"
