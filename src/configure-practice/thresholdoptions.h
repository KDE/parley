/***************************************************************************

                      threshold options for Parley

    -----------------------------------------------------------------------

    begin         : Tue Apr 5 2005

    copyright     : (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
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

#ifndef THRESHOLDOPTIONS_H
#define THRESHOLDOPTIONS_H

#include "ui_thresholdoptions.h"
#include "prefs.h"

class KEduVocDocument;


class ThresholdOptions : public QWidget, public Ui::ThresholdOptions
{
    Q_OBJECT
public:
    ThresholdOptions(KEduVocDocument* doc, QWidget* parent);

    void updateWidgets();
    bool hasChanged();
    bool isDefault();
    void updateSettings();

signals:
    void widgetModified();

protected slots:
    void slotComboActivated(int);
    void slotSetGradeComp(int);
    void slotSetBadComp(int);
    void slotSetQueryComp(int);
    void slotSetTypeComp(int);
    void slotSetDateComp(int);

private:
    static QString compStr(Prefs::EnumCompType::type type);

    void fillWidgets();

    KEduVocDocument *m_doc;
};

#endif
