/***************************************************************************

                   user tense options dialog page

    -----------------------------------------------------------------------

    begin         : Sun May 28 12:14:31 2000

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

#include "TenseOptPage.h"

#include <QPushButton>

#include <kinputdialog.h>
#include <kmessagebox.h>
#include <klocale.h>

#include "practice/testentrymanager.h"
#include <keduvocdocument.h>

#define TENSE_TAG ". "

TenseOptPage::TenseOptPage(KEduVocDocument *doc, QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    titleGroupBox->setTitle(i18n("Tense Descriptions"));

    connect(optionsList,   SIGNAL(currentRowChanged(int)), this, SLOT(slotTenseChosen(int)));
    connect(cleanUpButton, SIGNAL(clicked()),              this, SLOT(slotCleanup()));
    connect(deleteButton,  SIGNAL(clicked()),              this, SLOT(slotDeleteTense()));
    connect(modifyButton,  SIGNAL(clicked()),              this, SLOT(slotModifyTense()));
    connect(newButton,     SIGNAL(clicked()),              this, SLOT(slotNewTense()));

    m_doc = doc;

    int i = 1;
    foreach(const QString &tenseName, m_doc->tenseDescriptions()) {
        optionsList->addItem(QString("%1").arg(i++, 2).append(TENSE_TAG).append(tenseName));
        tenseIndex.append(i);
    }

    m_currentTense = 0;
    if (optionsList->count() > 0)
        optionsList->setCurrentRow(m_currentTense);

    modifyButton->setEnabled(optionsList->count() > 0);
    deleteButton->setEnabled(optionsList->count() > 0);

    optionsList->setFocus();
}


void TenseOptPage::slotTenseChosen(int index)
{
    m_currentTense = index;
}


void TenseOptPage::slotNewTense()
{
    bool ok;
    QString getTense = KInputDialog::getText(i18n("Tense Description"), i18n("Enter tense description:"), QString(), &ok, this);
    if (!ok)
        return;

    QString str;
    int i = optionsList->count() + 1;
    optionsList->addItem(QString("%1").arg(i, 2).append(TENSE_TAG).append(getTense.simplified()));
    tenseIndex.append(-(i - 1));

    m_currentTense = optionsList->count();
    optionsList->setCurrentRow(i - 1);
    modifyButton->setEnabled(true);
    deleteButton->setEnabled(true);
}


void TenseOptPage::slotModifyTense()
{
    if (optionsList->count() != 0 && (int) optionsList->count() > m_currentTense) {
        QString str = optionsList->item(m_currentTense)->text();
        str = str.mid(str.indexOf(TENSE_TAG) + QString(TENSE_TAG).length());

        bool ok;
        QString getTense = KInputDialog::getText(i18n("Tense Description"), i18n("Enter tense description:"), str, &ok, this);
        if (!ok)
            return;

        int i = m_currentTense + 1;
        optionsList->item(m_currentTense)->setText(QString("%1").arg(i, 2).append(TENSE_TAG).append(getTense.simplified()));
    }
}


void TenseOptPage::updateListBox(int start)
{
    QString str;
    for (int i = start; i < (int) optionsList->count(); i++) {
        str = optionsList->item(i)->text();
        str = str.mid(str.indexOf(TENSE_TAG) + QString(TENSE_TAG).length());
        optionsList->item(i)->setText(QString("%1").arg(i + 1, 2).append(TENSE_TAG).append(str));
    }
}


void TenseOptPage::slotDeleteTense()
{
    int act = m_currentTense;
    if (optionsList->count() > 0 && (int) optionsList->count() > act) {

        QString t = optionsList->item(act)->text();

        for (int ent = 0; ent < m_doc->entryCount(); ent++) {
            KEduVocExpression *exp = m_doc->entry(ent);
            for (int lang = 0; lang < m_doc->identifierCount(); lang++) {
                if ( exp->translation(lang).conjugationTenses().contains(t)) {
                        KMessageBox::information(this, i18n("The selected user defined tense could not be deleted\nbecause it is in use."),    i18n("Deleting Tense Description"));
                        return;
                }
            }
        }

        delete optionsList->takeItem(act);
        tenseIndex.erase(tenseIndex.begin() + act);

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


// void TenseOptPage::getTenseNames(QStringList &ret_tense, QList<int> &ret_Index)
// {
//     ret_tense.clear();
//
//     QString str;
//     for (int i = 0; i < (int) optionsList->count(); i++) {
//         str = optionsList->item(i)->text();
//         ret_tense.append(str.mid(str.indexOf(TENSE_TAG) + QString(TENSE_TAG).length()));
//     }
//     ret_Index = tenseIndex;
// }

void TenseOptPage::accept()
{
    QList<int> tenseIndex;
    QStringList new_tenseStr;

    QString str;
    for (int i = 0; i < (int) optionsList->count(); i++) {
        str = optionsList->item(i)->text();
        new_tenseStr.append(str.mid(str.indexOf(TENSE_TAG) + QString(TENSE_TAG).length()));
    }

    m_doc->setTenseDescriptions(new_tenseStr);
}


#include "TenseOptPage.moc"

