/***************************************************************************

    $Id$

                      query options dialogpage

    -----------------------------------------------------------------------

    begin                : Sat Jul 10 18:03:22 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

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

#ifndef QueryOptPage_included
#define QueryOptPage_included

#include "QueryOptPageForm.h"

#include <QueryManager.h>

#include <kstandarddirs.h>
#include <klocale.h>

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
        bool          altlearn,
        QWidget      *parent = NULL,
        const char   *name = NULL
    );

    kvq_timeout_t  getTypeQueryTimeout()   const { return type_timeout; }
    int  getMQueryTime()   const { return mqtime; }
    bool getSwapDir()      const { return swapdir; }
    bool getAltLearn()     const { return altlearn; }
    bool getShowCounter()  const { return showCounter; }

    void setStates(int _mqtime, bool _swapdir, bool _altlearn, bool show, kvq_timeout_t type_to);

protected:
    void keyPressEvent( QKeyEvent * );

signals:
    void modifySetting(int = -1);

public slots:
    void initFocus() const;

protected slots:
    void slotChangeMQTime(const QString&);
    void slotCheckSwap(bool);
    void slotAltLearn(bool);
    void slotContTimeOut();
    void slotNoTimeout();
    void slotShowTimeout();
    void slotShowRemTime(bool);

protected:

    QValidator   *validator;
    QueryManager  manager;
    int           mqtime;
    bool          swapdir;
    bool          altlearn;
    kvq_timeout_t type_timeout;
    bool          showCounter;
};

#endif // QueryOptPage_included
