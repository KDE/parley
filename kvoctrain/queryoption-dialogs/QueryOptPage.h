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
        QueryManager *_manager,
        bool          swapdir,
        bool          _suggestions,
        bool          _split,
        bool          _periods,
        bool          _colons,
        bool          _semicolons,
        bool          _commas,
        int           _fields,
        bool          _show_more,
        bool          _i_know,
        bool          altlearn,
        QWidget      *parent = NULL,
        const char   *name = NULL
    );

    //kvq_timeout_t  getTypeQueryTimeout()   const { return type_timeout; }
    int  getMQueryTime()   const { return mqtime; }
    bool getSwapDir()      const { return swapdir; }
    bool getAltLearn()     const { return altlearn; }
    bool getShowCounter()  const { return showCounter; }
    bool getSuggestions()  const { return suggestions; }
    bool getSplit()        const { return split; }
    bool getPeriods()      const { return periods; }
    bool getColons()       const { return colons; }
    bool getSemicolons()   const { return semicolons; }
    bool getCommas()       const { return commas; }
    int  getFields()       const { return fields; }
    bool getShowMore()     const { return show_more; }
    bool getIKnow()        const { return i_know; }

    void setStates(int _mqtime, bool _swapdir, bool _altlearn, bool show,
      bool _suggestions, bool _split, bool _periods, bool _colons, bool _semicolons,
      bool _commas, int _fields, bool _show_more, bool _i_know);

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
    void slotCheckSuggestions(bool);
    void slotCheckSplit(bool);
    void slotCheckPeriods(bool);
    void slotCheckColons(bool);
    void slotCheckSemicolons(bool);
    void slotCheckCommas(bool);
    void slotChangeFields(const QString&);
    void slotCheckShowMore(bool);
    void slotCheckIKnow(bool);

protected:

    QValidator   *validator;
    QueryManager  manager;
    int           mqtime;
    bool          swapdir;
    bool          altlearn;
    //kvq_timeout_t type_timeout;
    bool          showCounter;
    bool          suggestions;
    bool          split;
    bool          periods;
    bool          colons;
    bool          semicolons;
    bool          commas;
    int           fields;
    bool          show_more;
    bool          i_know;
};

#endif // QueryOptPage_included
