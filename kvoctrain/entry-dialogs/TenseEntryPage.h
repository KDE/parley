/***************************************************************************

    $Id$

                   tenses of irreg. verbs dialog page

    -----------------------------------------------------------------------

    begin                : Sat Nov 27 20:20:34 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.4  2002/01/19 10:33:09  arnold
    made entry dialog modeless

    Revision 1.3  2001/11/09 10:40:05  arnold
    removed ability to display a different font for each column

    Revision 1.2  2001/10/23 18:15:39  arnold
    replaced qtarch dialog files with qtdesigner

    Revision 1.1  2001/10/05 15:40:37  arnold
    import of version 0.7.0pre8 to kde-edu

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

#ifndef TenseEntryPage_included
#define TenseEntryPage_included

#include "TenseEntryPageForm.h"

#include <GrammerManager.h>

class EntryDlg;

class TenseEntryPage : public TenseEntryPageForm
{
    Q_OBJECT

public:

    TenseEntryPage
    (
        EntryDlg          *dlgbook,
        bool               multi_sel,
        const Conjugation &con_prefix,
        const Conjugation &conjugations,
        QWidget           *parent = NULL,
        const char        *name = NULL
    );

    void setData(bool multi_sel, const Conjugation &conjugations);

    Conjugation getConjugation();

    bool isModified();
    void setModified(bool mod = true);
    void setEnabled(int enable_type);

public slots:
    void initFocus() const;

signals:
    void sigModified();

protected:
    void keyPressEvent( QKeyEvent * );
    void updateFields();

protected slots:
    void secondPluralChanged(const QString&);
    void secondSingularChanged(const QString&);
    void thirdNSingularChanged(const QString&);
    void thirdFPluralChanged(const QString&);
    void thirdMSingularChanged(const QString&);
    void thirdFSingularChanged(const QString&);
    void slotTenseSelected(int);
    void thirdMPluralChanged(const QString&);
    void thirdNPluralChanged(const QString&);
    void firstSingularChanged(const QString&);
    void firstPluralChanged(const QString&);
    void slotThirdSCommonToggled(bool);
    void slotThirdPCommonToggled(bool);
    void slotNextConj();

protected:
    Conjugation   prefix,
                  conjugations;
    QString       selection;
    bool          multi_mode;
    bool          modified;
    EntryDlg     *dlgbook;
};
#endif // TenseEntryPage_included
