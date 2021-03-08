/*
    blocking options for Parley
    SPDX-FileCopyrightText: 2005 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "blockoptions.h"

#include <QCheckBox>

#include <KLocalizedString>
#include <KMessageBox>
#include <KComboBox>

#include "prefs.h"

struct ListRef
{
    const char *text;
    long int num;
};

static ListRef date_itemlist [] = {
    {
        I18N_NOOP("Do not Care"),    0
    },

    {I18N_NOOP("30 Min"),        30 * 60},
    {I18N_NOOP("1 Hour"),    1 * 60 * 60},
    {I18N_NOOP("2 Hours"),   2 * 60 * 60},
    {I18N_NOOP("4 Hours"),   4 * 60 * 60},
    {I18N_NOOP("8 Hours"),   8 * 60 * 60},
    {I18N_NOOP("12 Hours"), 12 * 60 * 60},
    {I18N_NOOP("18 Hours"), 18 * 60 * 60},

    {I18N_NOOP("1 Day"),    1 * 60 * 60 * 24},
    {I18N_NOOP("2 Days"),   2 * 60 * 60 * 24},
    {I18N_NOOP("3 Days"),   3 * 60 * 60 * 24},
    {I18N_NOOP("4 Days"),   4 * 60 * 60 * 24},
    {I18N_NOOP("5 Days"),   5 * 60 * 60 * 24},
    {I18N_NOOP("6 Days"),   6 * 60 * 60 * 24},

    {I18N_NOOP("1 Week"),   1 * 60 * 60 * 24 * 7},
    {I18N_NOOP("2 Weeks"),  2 * 60 * 60 * 24 * 7},
    {I18N_NOOP("3 Weeks"),  3 * 60 * 60 * 24 * 7},
    {I18N_NOOP("4 Weeks"),  4 * 60 * 60 * 24 * 7},

    {I18N_NOOP("1 Month"),  1 * 60 * 60 * 24 * 30},
    {I18N_NOOP("2 Months"), 2 * 60 * 60 * 24 * 30},
    {I18N_NOOP("3 Months"), 3 * 60 * 60 * 24 * 30},
    {I18N_NOOP("4 Months"), 4 * 60 * 60 * 24 * 30},
    {I18N_NOOP("5 Months"), 5 * 60 * 60 * 24 * 30},
    {I18N_NOOP("6 Months"), 6 * 60 * 60 * 24 * 30},
    {I18N_NOOP("10 Months"), 10 * 60 * 60 * 24 * 30},
    {I18N_NOOP("12 Months"), 12 * 60 * 60 * 24 * 30},
    {0 ,  0}
};


BlockOptions::BlockOptions(QWidget* parent): QWidget(parent)
{
    setupUi(this);
    connect(expire1, static_cast<void (KComboBox::*)(int)>(&KComboBox::activated), this, &BlockOptions::slotComboBoxActivated);
    connect(block1, static_cast<void (KComboBox::*)(int)>(&KComboBox::activated), this, &BlockOptions::slotComboBoxActivated);
    connect(expire2, static_cast<void (KComboBox::*)(int)>(&KComboBox::activated), this, &BlockOptions::slotComboBoxActivated);
    connect(block2, static_cast<void (KComboBox::*)(int)>(&KComboBox::activated), this, &BlockOptions::slotComboBoxActivated);
    connect(expire3, static_cast<void (KComboBox::*)(int)>(&KComboBox::activated), this, &BlockOptions::slotComboBoxActivated);
    connect(block3, static_cast<void (KComboBox::*)(int)>(&KComboBox::activated), this, &BlockOptions::slotComboBoxActivated);
    connect(expire4, static_cast<void (KComboBox::*)(int)>(&KComboBox::activated), this, &BlockOptions::slotComboBoxActivated);
    connect(block4, static_cast<void (KComboBox::*)(int)>(&KComboBox::activated), this, &BlockOptions::slotComboBoxActivated);
    connect(expire5, static_cast<void (KComboBox::*)(int)>(&KComboBox::activated), this, &BlockOptions::slotComboBoxActivated);
    connect(block5, static_cast<void (KComboBox::*)(int)>(&KComboBox::activated), this, &BlockOptions::slotComboBoxActivated);
    connect(expire6, static_cast<void (KComboBox::*)(int)>(&KComboBox::activated), this, &BlockOptions::slotComboBoxActivated);
    connect(block6, static_cast<void (KComboBox::*)(int)>(&KComboBox::activated), this, &BlockOptions::slotComboBoxActivated);
    connect(expire7, static_cast<void (KComboBox::*)(int)>(&KComboBox::activated), this, &BlockOptions::slotComboBoxActivated);
    connect(block7, static_cast<void (KComboBox::*)(int)>(&KComboBox::activated), this, &BlockOptions::slotComboBoxActivated);

    connect(kcfg_Block, &QCheckBox::toggled, this, &BlockOptions::slotBlockToggled);
    connect(kcfg_Expire, &QCheckBox::toggled, this, &BlockOptions::slotExpireToggled);

    fillWidgets();
    updateWidgets();
}

void BlockOptions::fillWidgets()
{
    fillComboBox(block1);
    fillComboBox(block2);
    fillComboBox(block3);
    fillComboBox(block4);
    fillComboBox(block5);
    fillComboBox(block6);
    fillComboBox(block7);

    m_blockComboList.append(block1);
    m_blockComboList.append(block2);
    m_blockComboList.append(block3);
    m_blockComboList.append(block4);
    m_blockComboList.append(block5);
    m_blockComboList.append(block6);
    m_blockComboList.append(block7);

    fillComboBox(expire1);
    fillComboBox(expire2);
    fillComboBox(expire3);
    fillComboBox(expire4);
    fillComboBox(expire5);
    fillComboBox(expire6);
    fillComboBox(expire7);

    m_expireComboList.append(expire1);
    m_expireComboList.append(expire2);
    m_expireComboList.append(expire3);
    m_expireComboList.append(expire4);
    m_expireComboList.append(expire5);
    m_expireComboList.append(expire6);
    m_expireComboList.append(expire7);
}

void BlockOptions::updateWidgets()
{
    updateComboBox(Prefs::blockItem(KV_LEV1_GRADE), block1);
    updateComboBox(Prefs::blockItem(KV_LEV2_GRADE), block2);
    updateComboBox(Prefs::blockItem(KV_LEV3_GRADE), block3);
    updateComboBox(Prefs::blockItem(KV_LEV4_GRADE), block4);
    updateComboBox(Prefs::blockItem(KV_LEV5_GRADE), block5);
    updateComboBox(Prefs::blockItem(KV_LEV6_GRADE), block6);
    updateComboBox(Prefs::blockItem(KV_LEV7_GRADE), block7);

    updateComboBox(Prefs::expireItem(KV_LEV1_GRADE), expire1);
    updateComboBox(Prefs::expireItem(KV_LEV2_GRADE), expire2);
    updateComboBox(Prefs::expireItem(KV_LEV3_GRADE), expire3);
    updateComboBox(Prefs::expireItem(KV_LEV4_GRADE), expire4);
    updateComboBox(Prefs::expireItem(KV_LEV5_GRADE), expire5);
    updateComboBox(Prefs::expireItem(KV_LEV6_GRADE), expire6);
    updateComboBox(Prefs::expireItem(KV_LEV7_GRADE), expire7);

    bool block = Prefs::block();
    kcfg_Block->setChecked(block);
    block1->setEnabled(block);
    block2->setEnabled(block);
    block3->setEnabled(block);
    block4->setEnabled(block);
    block5->setEnabled(block);
    block6->setEnabled(block);
    block7->setEnabled(block);

    bool expire = Prefs::expire();
    kcfg_Expire->setChecked(expire);
    expire1->setEnabled(expire);
    expire2->setEnabled(expire);
    expire3->setEnabled(expire);
    expire4->setEnabled(expire);
    expire5->setEnabled(expire);
    expire6->setEnabled(expire);
    expire7->setEnabled(expire);
}

void BlockOptions::fillComboBox(KComboBox * cb)
{
    ListRef *ref = date_itemlist;

    cb->clear();
    while (ref->text != 0) {
        cb->addItem(i18n(ref->text));
        ref++;
    }
}

void BlockOptions::updateComboBox(int value, KComboBox * cb)
{
    ListRef *ref = date_itemlist;
    int index = 0;

    while (ref->text != 0) {
        if (value == ref->num)
            index = ref - date_itemlist;
        ref++;
    }
    cb->setCurrentIndex(index);
}

void BlockOptions::slotBlockToggled(bool state)
{
    block1->setEnabled(state);
    block2->setEnabled(state);
    block3->setEnabled(state);
    block4->setEnabled(state);
    block5->setEnabled(state);
    block6->setEnabled(state);
    block7->setEnabled(state);
    if (state) {
        checkValidity();
    }
}

void BlockOptions::slotExpireToggled(bool state)
{
    expire1->setEnabled(state);
    expire2->setEnabled(state);
    expire3->setEnabled(state);
    expire4->setEnabled(state);
    expire5->setEnabled(state);
    expire6->setEnabled(state);
    expire7->setEnabled(state);
    if (state) {
        checkValidity();
    }
}

void BlockOptions::slotComboBoxActivated(int)
{
    emit widgetModified();
    checkValidity();
}


void BlockOptions::checkValidity()
{
    QString message;
    if (kcfg_Block->isChecked()) {
        bool found = false;
        for (int i = 1; i <= 6; i++) {
            if (date_itemlist[m_blockComboList[i - 1]->currentIndex()].num != 0 &&
                    date_itemlist[m_blockComboList[i]->currentIndex()].num != 0 &&
                    date_itemlist[m_blockComboList[i - 1]->currentIndex()].num >= date_itemlist[m_blockComboList[i]->currentIndex()].num) {
                if (!found)
                    message.append(i18n("Illogical blocking times.\n"));
                found = true;
                message.append(i18n("The time for level %1 should be lower than the time for level %2.\n", i, i + 1));
            }
        }
    }

    if (kcfg_Expire->isChecked()) {
        bool found = false;
        for (int i = 1; i <= 6; i++) {
            if (date_itemlist[m_expireComboList[i - 1]->currentIndex()].num != 0 &&
                    date_itemlist[m_expireComboList[i]->currentIndex()].num != 0 &&
                    date_itemlist[m_expireComboList[i - 1]->currentIndex()].num >=
                    date_itemlist[m_expireComboList[i]->currentIndex()].num) {
                if (!found)
                    message.append(i18n("\nIllogical expiration times.\n"));
                found = true;
                message.append(i18n("The time for level %1 should be lower than the time for level %2.\n", i, i + 1));
            }
        }
    }

    if ((message.length() == 0) && kcfg_Block->isChecked() && kcfg_Expire->isChecked()) {
        bool found = false;
        for (int i = 0; i <= 6; i++) {
            if (m_expireComboList[i]->currentIndex() >= 0 &&
                    m_blockComboList[i]->currentIndex() >= 0 &&
                    date_itemlist[m_expireComboList[i]->currentIndex()].num != 0 &&
                    date_itemlist[m_blockComboList[i]->currentIndex()].num != 0 &&
                    date_itemlist[m_blockComboList[i]->currentIndex()].num >= date_itemlist[m_expireComboList[i]->currentIndex()].num) {
                if (!found)
                    message.append(i18n("\nIllogical blocking vs. expiration times.\n"));
                found = true;
                message.append(i18n("The blocking time at level %1 should be lower than the expiration time.\n", i + 1));
            }
        }
    }

    if (message.length() != 0)
        KMessageBox::sorry(this, message, i18n("Illogical Values"));
}


bool BlockOptions::isDefault()
{
    if (block7->currentIndex() < 0 ||
            block6->currentIndex() < 0 ||
            block5->currentIndex() < 0 ||
            block4->currentIndex() < 0 ||
            block3->currentIndex() < 0 ||
            block2->currentIndex() < 0 ||
            block1->currentIndex() < 0 ||
            expire7->currentIndex() < 0 ||
            expire6->currentIndex() < 0 ||
            expire5->currentIndex() < 0 ||
            expire4->currentIndex() < 0 ||
            expire3->currentIndex() < 0 ||
            expire2->currentIndex() < 0 ||
            expire1->currentIndex() < 0)
        return false;

    return date_itemlist[block7->currentIndex()].num == 5184000 &&
           date_itemlist[block6->currentIndex()].num == 2592000 &&
           date_itemlist[block5->currentIndex()].num == 1209600 &&
           date_itemlist[block4->currentIndex()].num == 604800 &&
           date_itemlist[block3->currentIndex()].num == 345600 &&
           date_itemlist[block2->currentIndex()].num == 172800 &&
           date_itemlist[block1->currentIndex()].num == 86400 &&
           date_itemlist[expire7->currentIndex()].num == 10368000 &&
           date_itemlist[expire6->currentIndex()].num == 5184000 &&
           date_itemlist[expire5->currentIndex()].num == 2592000 &&
           date_itemlist[expire4->currentIndex()].num == 1209600 &&
           date_itemlist[expire3->currentIndex()].num == 604800 &&
           date_itemlist[expire2->currentIndex()].num == 345600 &&
           date_itemlist[expire1->currentIndex()].num == 172800;
}


void BlockOptions::updateSettings()
{
    if (block7->currentIndex() < 0 ||
            block6->currentIndex() < 0 ||
            block5->currentIndex() < 0 ||
            block4->currentIndex() < 0 ||
            block3->currentIndex() < 0 ||
            block2->currentIndex() < 0 ||
            block1->currentIndex() < 0 ||
            expire7->currentIndex() < 0 ||
            expire6->currentIndex() < 0 ||
            expire5->currentIndex() < 0 ||
            expire4->currentIndex() < 0 ||
            expire3->currentIndex() < 0 ||
            expire2->currentIndex() < 0 ||
            expire1->currentIndex() < 0)
        return;

    Prefs::setBlockItem(KV_LEV7_GRADE, date_itemlist[qMax(block7->currentIndex(), 0)].num);
    Prefs::setBlockItem(KV_LEV6_GRADE, date_itemlist[qMax(block6->currentIndex(), 0)].num);
    Prefs::setBlockItem(KV_LEV5_GRADE, date_itemlist[qMax(block5->currentIndex(), 0)].num);
    Prefs::setBlockItem(KV_LEV4_GRADE, date_itemlist[qMax(block4->currentIndex(), 0)].num);
    Prefs::setBlockItem(KV_LEV3_GRADE, date_itemlist[qMax(block3->currentIndex(), 0)].num);
    Prefs::setBlockItem(KV_LEV2_GRADE, date_itemlist[qMax(block2->currentIndex(), 0)].num);
    Prefs::setBlockItem(KV_LEV1_GRADE, date_itemlist[qMax(block1->currentIndex(), 0)].num);

    Prefs::setExpireItem(KV_LEV7_GRADE, date_itemlist[qMax(expire7->currentIndex(), 0)].num);
    Prefs::setExpireItem(KV_LEV6_GRADE, date_itemlist[qMax(expire6->currentIndex(), 0)].num);
    Prefs::setExpireItem(KV_LEV5_GRADE, date_itemlist[qMax(expire5->currentIndex(), 0)].num);
    Prefs::setExpireItem(KV_LEV4_GRADE, date_itemlist[qMax(expire4->currentIndex(), 0)].num);
    Prefs::setExpireItem(KV_LEV3_GRADE, date_itemlist[qMax(expire3->currentIndex(), 0)].num);
    Prefs::setExpireItem(KV_LEV2_GRADE, date_itemlist[qMax(expire2->currentIndex(), 0)].num);
    Prefs::setExpireItem(KV_LEV1_GRADE, date_itemlist[qMax(expire1->currentIndex(), 0)].num);
}

bool BlockOptions::hasChanged()
{
    if (block7->currentIndex() < 0 ||
            block6->currentIndex() < 0 ||
            block5->currentIndex() < 0 ||
            block4->currentIndex() < 0 ||
            block3->currentIndex() < 0 ||
            block2->currentIndex() < 0 ||
            block1->currentIndex() < 0 ||
            expire7->currentIndex() < 0 ||
            expire6->currentIndex() < 0 ||
            expire5->currentIndex() < 0 ||
            expire4->currentIndex() < 0 ||
            expire3->currentIndex() < 0 ||
            expire2->currentIndex() < 0 ||
            expire1->currentIndex() < 0)
        return false;

    return date_itemlist[block7->currentIndex()].num != Prefs::blockItem(KV_LEV7_GRADE) ||
           date_itemlist[block6->currentIndex()].num != Prefs::blockItem(KV_LEV6_GRADE) ||
           date_itemlist[block5->currentIndex()].num != Prefs::blockItem(KV_LEV5_GRADE) ||
           date_itemlist[block4->currentIndex()].num != Prefs::blockItem(KV_LEV4_GRADE) ||
           date_itemlist[block3->currentIndex()].num != Prefs::blockItem(KV_LEV3_GRADE) ||
           date_itemlist[block2->currentIndex()].num != Prefs::blockItem(KV_LEV2_GRADE) ||
           date_itemlist[block1->currentIndex()].num != Prefs::blockItem(KV_LEV1_GRADE) ||
           date_itemlist[expire7->currentIndex()].num != Prefs::expireItem(KV_LEV7_GRADE) ||
           date_itemlist[expire6->currentIndex()].num != Prefs::expireItem(KV_LEV6_GRADE) ||
           date_itemlist[expire5->currentIndex()].num != Prefs::expireItem(KV_LEV5_GRADE) ||
           date_itemlist[expire4->currentIndex()].num != Prefs::expireItem(KV_LEV4_GRADE) ||
           date_itemlist[expire3->currentIndex()].num != Prefs::expireItem(KV_LEV3_GRADE) ||
           date_itemlist[expire2->currentIndex()].num != Prefs::expireItem(KV_LEV2_GRADE) ||
           date_itemlist[expire1->currentIndex()].num != Prefs::expireItem(KV_LEV1_GRADE);
}
