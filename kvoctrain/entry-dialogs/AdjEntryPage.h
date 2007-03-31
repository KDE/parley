/***************************************************************************

                   dialog page for adjectives (comparison)

    -----------------------------------------------------------------------

    begin         : Sat Dec 4 18:09:29 1999

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

#ifndef AdjEntryPage_included
#define AdjEntryPage_included

#include "ui_AdjEntryPageForm.h"
#include <keduvocgrammar.h>

class AdjEntryPage : public QWidget, public Ui::AdjEntryPageForm
{
    Q_OBJECT

public:
    AdjEntryPage(QWidget *parent = 0);

    void setData(bool multi_sel, const KEduVocComparison  &comp);

    KEduVocComparison getComparison() const
    {
        return comparisons;
    }

    bool isModified();
    void setModified(bool mod = true);
    void setEnabled(int enable_type);

signals:
    void sigModified();

protected slots:
    void lev1Changed(const QString&);
    void lev2Changed(const QString&);
    void lev3Changed(const QString&);

protected:
    KEduVocComparison comparisons;
    bool              modified;
    bool              m_largeSelection;
};

#endif // AdjEntryPage_included
