/***************************************************************************

    $Id$

                      query options dialogpage

    -----------------------------------------------------------------------

    begin                : Sat Jul 10 18:03:22 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.1  2001/10/05 15:46:01  arnold
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

#ifndef QueryOptPage_included
#define QueryOptPage_included

#include "QueryOptPageForm.h"

#include <QueryManager.h>
#include <compat_2x.h>

class QValidator;

class QueryOptPage : public QueryOptPageForm
{
    Q_OBJECT

public:

    QueryOptPage
    (
        int           _mqtime,
        bool          show,
        kvq_timeout_t type_timeout,
        QueryManager *_manager,
        bool          swapdir,
        QWidget      *parent = NULL,
        const char   *name = NULL
    );

    kvq_timeout_t  getTypeQueryTimeout()   const { return type_timeout; }
    int  getMQueryTime()   const { return mqtime; }
    bool getSwapDir()      const { return swapdir; }
    bool getShowCounter()  const { return showCounter; }

    void setStates(int _mqtime, bool _swapdir, bool show, kvq_timeout_t type_to);

protected:
    void keyPressEvent( QKeyEvent * );

signals:
    void modifySetting(int = -1);

public slots:
    void initFocus() const;

protected slots:
    void slotChangeMQTime(const QString&);
    void slotCheckSwap(bool);
    void slotContTimeOut();
    void slotNoTimeout();
    void slotShowTimeout();
    void slotShowRemTime(bool);

protected:

    QValidator   *validator;
    QueryManager  manager;
    int           mqtime;
    bool          swapdir;
    kvq_timeout_t type_timeout;
    bool          showCounter;
};

#endif // QueryOptPage_included
