/***************************************************************************

                    types options dialog page

    -----------------------------------------------------------------------

    begin         : Sun Sep 12 15:38:31 1999

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

#include "TypeOptPage.h"

#include <QPushButton>

#include <kinputdialog.h>
#include <kmessagebox.h>
#include <klocale.h>

#include <kvtquery.h>
#include <keduvocdocument.h>

#define TYPE_TAG ". "

TypeOptPage::TypeOptPage(KEduVocDocument *_doc, QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    titleGroupBox->setTitle(i18n("Type Descriptions"));

    connect(optionsList,   SIGNAL(currentRowChanged(int)), this, SLOT(slotTypeChosen(int)));
    connect(cleanUpButton, SIGNAL(clicked()),              this, SLOT(slotCleanup()));
    connect(deleteButton,  SIGNAL(clicked()),              this, SLOT(slotDeleteType()));
    connect(modifyButton,  SIGNAL(clicked()),              this, SLOT(slotModifyType()));
    connect(newButton,     SIGNAL(clicked()),              this, SLOT(slotNewType()));

    doc = _doc;

    int i = 1;
    foreach(const QString &typeName, doc->typeDescriptions()) {
        optionsList->addItem(QString("%1").arg(i++, 2).append(TYPE_TAG).append(typeName));
        typeIndex.append(i - 1);
    }

    m_currentType = 0;
    if (optionsList->count() > 0)
        optionsList->setCurrentRow(m_currentType);

    modifyButton->setEnabled(optionsList->count() > 0);
    deleteButton->setEnabled(optionsList->count() > 0);

    optionsList->setFocus();
}


void TypeOptPage::slotTypeChosen(int index)
{
    m_currentType = index;
}


void TypeOptPage::slotNewType()
{
    bool ok;
    QString getType = KInputDialog::getText(i18n("Type Description"), i18n("Enter type description:"), QString(), &ok, this);
    if (!ok)
        return;

    QString str;
    int i = optionsList->count() + 1;
    optionsList->addItem(QString("%1").arg(i, 2).append(TYPE_TAG).append(getType.simplified()));
    typeIndex.append(-(i - 1));

    m_currentType = optionsList->count();
    optionsList->setCurrentRow(i - 1);
    modifyButton->setEnabled(true);
    deleteButton->setEnabled(true);
}


void TypeOptPage::slotModifyType()
{
    if (optionsList->count() > 0 && (int) optionsList->count() > m_currentType) {
        QString str = optionsList->item(m_currentType)->text();
        str = str.mid(str.indexOf(TYPE_TAG) + QString(TYPE_TAG).length());

        bool ok;
        QString getType = KInputDialog::getText(i18n("Type Description"), i18n("Enter type description:"), str, &ok, this);
        if (!ok)
            return;

        int i = m_currentType + 1;
        optionsList->item(m_currentType)->setText(QString("%1").arg(i, 2).append(TYPE_TAG).append(getType.simplified()));
    }
}


void TypeOptPage::updateListBox(int start)
{
    QString str;
    for (int i = start; i < (int) optionsList->count(); i++) {
        str = optionsList->item(i)->text();
        str = str.mid(str.indexOf(TYPE_TAG) + QString(TYPE_TAG).length());
        optionsList->item(i)->setText(QString("%1").arg(i + 1, 2).append(TYPE_TAG).append(str));
    }
}


void TypeOptPage::slotDeleteType()
{
    int act = m_currentType;
    if (optionsList->count() > 0 && (int) optionsList->count() > act) {

        QString t;
        t.setNum(typeIndex[m_currentType] - 1).prepend(QM_USER_TYPE);

        for (int ent = 0; ent < doc->entryCount(); ent++) {
            KEduVocExpression *exp = doc->entry(ent);
            for (int lang = 0; lang < (int) doc->identifierCount(); lang++) {
                if (exp->type(lang) == t) {
                    KMessageBox::information(this, i18n("The selected user defined type could not be deleted\nbecause it is in use."), i18n("Deleting Type Description"));
                    return;
                }
            }
        }

        delete optionsList->takeItem(act);
        typeIndex.erase(typeIndex.begin() + act);

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


void TypeOptPage::slotCleanup()
{
    QVector<bool> used_type;
    for (int i = 0; i <= (int) optionsList->count(); i++)
        used_type.append(false);

    for (int col = 0; col < doc->identifierCount(); col++)
        for (int i = 0; i < (int) doc->entryCount(); i++) {
            QString t = doc->entry(i)->type(col);
            if (t.left(QString(QM_USER_TYPE).length()) == QM_USER_TYPE) {
                t.remove(0, QString(QM_USER_TYPE).length());
                int idx = t.toInt();
                if ((int) used_type.count() < idx)
                    used_type.resize(idx);
                if (idx != 0)
                    used_type[idx - 1] = true ;
            }
        }

    for (int i = used_type.count() - 1; i >= 0; i--)
        if (!used_type[i]) {
            for (int u = 0; u < (int) typeIndex.count() ; u++) {
                if (typeIndex[u] == i || typeIndex[u] < 0) {
                    m_currentType = i;
                    slotDeleteType();
                    break;
                }
            }
        }

    m_currentType = 0;
    optionsList->setCurrentRow(m_currentType);
}


void TypeOptPage::getTypeNames(QStringList &ret_type, QList<int> &ret_Index)
{
    ret_type.clear();

    QString str;
    for (int i = 0; i < (int) optionsList->count(); i++) {
        str = optionsList->item(i)->text();
        ret_type.append(str.mid(str.indexOf(TYPE_TAG) + QString(TYPE_TAG).length()));
    }
    ret_Index = typeIndex;
}


void TypeOptPage::cleanUnused(KEduVocDocument *doc, const QList<int> &typeIndex, int old_types)
{
    QList<int> translate_index;
    QList<QString> new_typeStr;

    /////////////////////////////////////////////////////
    // translate_index contains new index number for each
    // old index
    for (int i = 0; i <= qMax(old_types, (int) typeIndex.count()); i++)
        translate_index.append(0);

    // now adjust type descriptions to new index
    for (int i = 0; i < (int) typeIndex.count(); i++) {
        if (typeIndex[i] >= 0)
            translate_index[typeIndex[i] /*+ 1*/] = i + 1;
    }

    // only keep remaining type indices

    // set type index to 0 when not needed any more
    // and translate to new index
    for (int col = 0; col < doc->identifierCount(); col++) {
        for (int i = 0; i < doc->entryCount(); i++) {
            QString old = doc->entry(i)->type(col);
            if (!old.isEmpty() && old.left(QString(QM_USER_TYPE).length()) == QM_USER_TYPE) {
                old.remove(0, 1);
                int o = old.toInt();

                QString newtype;
                if (translate_index[o] > 0) {
                    newtype.setNum(translate_index[o]).prepend(QM_USER_TYPE);
                    doc->entry(i)->setType(col, newtype);
                } else
                    doc->entry(i)->setType(col, "");
            }
        }
    }
}

#include "TypeOptPage.moc"
