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

#include "practice/kvtquery.h"
#include <keduvocdocument.h>

#define TENSE_TAG ". "

TenseOptPage::TenseOptPage(KEduVocDocument *_doc, QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    titleGroupBox->setTitle(i18n("Tense Descriptions"));

    connect(optionsList,   SIGNAL(currentRowChanged(int)), this, SLOT(slotTenseChosen(int)));
    connect(cleanUpButton, SIGNAL(clicked()),              this, SLOT(slotCleanup()));
    connect(deleteButton,  SIGNAL(clicked()),              this, SLOT(slotDeleteTense()));
    connect(modifyButton,  SIGNAL(clicked()),              this, SLOT(slotModifyTense()));
    connect(newButton,     SIGNAL(clicked()),              this, SLOT(slotNewTense()));

    doc = _doc;

    int i = 1;
    foreach(const QString &tenseName, doc->tenseDescriptions()) {
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

        for (int ent = 0; ent < doc->entryCount(); ent++) {
            KEduVocExpression *exp = doc->entry(ent);
            for (int lang = 0; lang < doc->identifierCount(); lang++) {
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


void TenseOptPage::slotCleanup()
{
///@todo reimplement me, if I am still needed
/*    QVector<bool> used_tense;
    for (int i = 0; i <= (int) optionsList->count(); i++)
        used_tense.append(false);

    for (int col = 0; col < doc->identifierCount(); col++)
        for (int i = 0; i < (int) doc->entryCount(); i++) {
            KEduVocConjugation conj = doc->entry(i)->translation(col).conjugation();
            for (int ci = 0; ci < conj.entryCount(); ci++) {
                QString t = conj.tenses().value(ci);
                ///@todo the following lines make no sense
                // since the conjugations will change more, do it later
                if (t.left(QString('#').length()) == "#") {
                    t.remove(0, QString('#').length());
                    int idx = t.toInt();
                    if ((int) used_tense.count() < idx)
                        used_tense.resize(idx);
                    if (idx > 0)
                        used_tense[idx - 1] = true;
                }
            }
        }

    for (int i = used_tense.count() - 1; i >= 0; i--)
        if (!used_tense[i]) {
            for (int u = 0; u < (int) tenseIndex.count() ; u++) {
                if (tenseIndex[u] == i || tenseIndex[u] < 0) {
                    m_currentTense = i;
                    slotDeleteTense();
                    break;
                }
            }
        }

    m_currentTense = 0;
    optionsList->setCurrentRow(m_currentTense);*/
}


void TenseOptPage::getTenseNames(QStringList &ret_tense, QList<int> &ret_Index)
{
    ret_tense.clear();

    QString str;
    for (int i = 0; i < (int) optionsList->count(); i++) {
        str = optionsList->item(i)->text();
        ret_tense.append(str.mid(str.indexOf(TENSE_TAG) + QString(TENSE_TAG).length()));
    }
    ret_Index = tenseIndex;
}


void TenseOptPage::cleanUnused(KEduVocDocument *doc, const QList<int> &tenseIndex, int old_tenses)
{
///@todo tenses
/*
    QList<int> translate_index;
    QStringList new_tenseStr;

    /////////////////////////////////////////////////////
    // translate_index contains new index number for each
    // old index
    for (int i = 0; i <= qMax(old_tenses, (int) tenseIndex.count()); i++)
        translate_index.append(0);

    // now adjust tense descriptions to new index
    for (int i = 0; i < (int) tenseIndex.count(); i++) {
        if (tenseIndex[i] >= 0)
            translate_index[tenseIndex[i] + 1] = i + 1;
    }

    // only keep remaining tense indices

    // set tense index to 0 when not needed any more
    // and translate to new index
    for (int col = 0; col < doc->identifierCount(); col++) {
        for (int i = 0; i < doc->entryCount(); i++) {
            KEduVocConjugation conj = doc->entry(i)->translation(col).conjugation();
            bool dirty = false;
            for (int ci = 0; ci < conj.entryCount(); ci++) {
                QString old = conj.getType(ci);
                if (!old.isEmpty() && old.left(QString(QM_USER_TYPE).length()) == QM_USER_TYPE) {
                    old.remove(0, QString(QM_USER_TYPE).length());
                    int o = old.toInt();

                    dirty = true;
                    QString newtense;
                    if (translate_index[o] > 0) {
                        newtense.setNum(translate_index[o]).prepend(QM_USER_TYPE);
                        conj.setType(ci, newtense);
                    } else
                        conj.setType(ci, "");
                }
            }
            if (dirty)
                doc->entry(i)->translation(col).setConjugation(conj);
        }
    }
*/
}

#include "TenseOptPage.moc"
