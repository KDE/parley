/***************************************************************************

                      blocking options for parley

    -----------------------------------------------------------------------

    begin                : Tue Apr 5 2005

    copyright            :(C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>

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

#ifndef BLOCKOPTIONS_H
#define BLOCKOPTIONS_H

#include <QList>

#include "ui_blockoptions.h"
#include "practiceold/testentrymanager.h"

class KComboBox;

class BlockOptions : public QWidget, public Ui::BlockOptionsBase
{
    Q_OBJECT
public:
    BlockOptions(QWidget* parent = 0);
    void updateWidgets();
    bool hasChanged();
    bool isDefault();
    void updateSettings();

signals:
    void widgetModified();

protected slots:
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

