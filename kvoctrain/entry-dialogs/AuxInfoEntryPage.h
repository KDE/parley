/***************************************************************************

    $Id$

                       edit "additional" properties

    -----------------------------------------------------------------------

    begin                : Thu Nov 25 17:29:44 1999
                                           
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



#ifndef AuxInfoEntryPage_included
#define AuxInfoEntryPage_included

#include "AuxInfoEntryPageForm.h"

class EntryDlg;

class AuxInfoEntryPage : public AuxInfoEntryPageForm
{
    Q_OBJECT

public:

    AuxInfoEntryPage
    (
        EntryDlg   *dlgbook,
        bool        multi_sel,
        QString     syno,
        QString     anto,
        QString     example,
        QString     remark,
        QString     para,
	QWidget    *parent = 0,
	const char *name = 0
    );

    void setData(
        bool        multi_sel,
        QString     syno,
        QString     anto,
        QString     example,
        QString     remark,
        QString     para);


    QString getSynonym ();
    QString getAntonym ();
    QString getExample ();
    QString getRemark  ();
    QString getParaphrase();

    bool isModified();
    void setModified(bool mod = true);
    void setEnabled(int enable_type);

public slots:
    void initFocus() const;
    void returnPressed();

signals:
    void sigModified();

protected:
    void keyPressEvent( QKeyEvent * );
    void normalize(QString &str);

protected slots:
    void slotAntonymSelected();
    void slotSynonymSelected();
    void slotExampSelected();
    void slotRemarkSelected();
    void slotParaSelected();

protected:
    QString       synonym;
    QString       antonym;
    QString       example;
    QString       remark;
    QString       paraphrase;
    bool          modified;
    EntryDlg     *dlgbook;
};

#endif // AuxInfoEntryPage_included
