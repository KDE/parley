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

#include "AuxInfoEntryPageData.h"

struct SpecFont_t;

class AuxInfoEntryPage : public AuxInfoEntryPageData
{
    Q_OBJECT

public:

    AuxInfoEntryPage
    (
        SpecFont_t *font,
        QDialog    *dlgbook,
        bool        multi_sel,
        QString     syno,
        QString     anto,
        QString     example,
        QString     remark,
        QString     para,
	QWidget    *parent = 0,
	const char *name = 0
    );

    QString getSynonym ();
    QString getAntonym ();
    QString getExample ();
    QString getRemark  ();
    QString getParaphrase();

protected:
    void keyPressEvent( QKeyEvent * );
    void normalize(QString &str);

protected slots:
    void initFocus() const;
    void slotAntonymSelected();
    void slotSynonymSelected();
    void slotExampSelected();
    void slotRemarkSelected();
    void slotParaSelected();

protected:
    QString   synonym;
    QString   antonym;
    QString   example;
    QString   remark;
    QString   paraphrase;
};

#endif // AuxInfoEntryPage_included
