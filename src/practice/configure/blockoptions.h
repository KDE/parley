/*
    blocking options for parley
    SPDX-FileCopyrightText: 2005 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef BLOCKOPTIONS_H
#define BLOCKOPTIONS_H

#include <QList>

#include "practice/sessionmanagerbase.h"
#include "ui_blockoptions.h"

class KComboBox;

class BlockOptions : public QWidget, public Ui::BlockOptionsBase
{
    Q_OBJECT
public:
    explicit BlockOptions(QWidget *parent = nullptr);
    void updateWidgets();
    bool hasChanged();
    bool isDefault();
    void updateSettings();

Q_SIGNALS:
    void widgetModified();

protected Q_SLOTS:
    void slotBlockToggled(bool);
    void slotExpireToggled(bool);
    void slotComboBoxActivated(int);

private:
    void fillWidgets();
    void fillComboBox(KComboBox *);
    void updateComboBox(int, KComboBox *);
    void checkValidity();

    QList<KComboBox *> m_blockComboList;
    QList<KComboBox *> m_expireComboList;
};

#endif
