/***************************************************************************

    $Id$

                      clipboard options dialog page

    -----------------------------------------------------------------------

    begin                : Sun Jun 27 11:07:24 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.1  2001/10/05 15:44:04  arnold
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


#ifndef PasteOptPage_included
#define PasteOptPage_included

#include "PasteOptPageForm.h"
#include <qstrlist.h>

class LangSet;
class kvoctrainDoc;

class PasteOptPage : public PasteOptPageForm
{
    Q_OBJECT

public:

    PasteOptPage
    (
        QString       _sep,
        LangSet      &langset,
        QStringList  &list,
        bool          useCurrent,
        kvoctrainDoc *doc,
        QWidget      *parent = NULL,
        const char   *name = NULL
    );

    inline QString  getSeparator() const { return sep; }
    QStringList getPasteOrder();
    bool getUseCurrent() const { return useCurrent; }

protected:
    void keyPressEvent( QKeyEvent * );

public slots:
    void initFocus() const;

protected slots:
    void slotSelectSep(int);
    void slotSelectOrder(int);
    void slotItemDown();
    void slotItemSkip();
    void slotItemUp();
    void slotCurrentDoc(bool);
    void slotSyncLangSet(const QString&);

protected:
    void slotEnableCursor();

    bool          useCurrent;
    QString       sep;
    LangSet      &langset;
    kvoctrainDoc *doc;
};
#endif // PasteOptPage_included
