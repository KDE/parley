/***************************************************************************

    $Id$

                     query options dialog class

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

#ifndef QueryOptionsDlg_included
#define QueryOptionsDlg_included

#include <qtabdialog.h>

#include "ThreshOptPage.h"
#include "QueryOptPage.h"
#include "BlockOptPage.h"
#include "GroupOptPage.h"

class kvoctraindoc;

class KComboBox;

class QueryOptionsDlg : public QTabDialog
{
    Q_OBJECT

public:

    QueryOptionsDlg
    (   
        int           _mqtime,
        bool          showcounter,
        kvq_timeout_t type_timeout,
        kvoctrainDoc *_doc,
        KComboBox    *lessons,
        QueryManager *manager,
        bool          swapdir,
        bool          suggestions,
        bool          split,
        bool          periods,
        bool          colons,
        bool          semicolons,
        bool          commas,
        int           fields,
        bool          show_more,
        bool          i_know,
        bool          altlearn,
        bool          block,
        bool          expire,
        vector<PreSetting>  preset,
        QWidget      *parent = NULL,
        const char   *name = NULL
    );

    int  getMQueryTime()  const { return queryOptPage->getMQueryTime(); }
    bool getShowCounter() const { return queryOptPage->getShowCounter(); }
    int  getSwapDir()     const { return queryOptPage->getSwapDir(); }
    int  getAltLearn()    const { return queryOptPage->getAltLearn(); }
    kvq_timeout_t getTypeQueryTimeout()   const
           { return queryOptPage->getTypeQueryTimeout(); }

    bool getSuggestions() const { return queryOptPage->getSuggestions(); }
    bool getSplit()       const { return queryOptPage->getSplit(); }
    bool getPeriods()     const { return queryOptPage->getPeriods(); }
    bool getColons()      const { return queryOptPage->getColons(); }
    bool getSemicolons()  const { return queryOptPage->getSemicolons(); }
    bool getCommas()      const { return queryOptPage->getCommas(); }
    int  getFields()      const { return queryOptPage->getFields(); }
    bool getShowMore()    const { return queryOptPage->getShowMore(); }
    bool getIKnow()       const { return queryOptPage->getIKnow(); }

    int getBlock()       const { return blockOptPage->getBlock(); }
    int getExpire()      const { return blockOptPage->getExpire(); }
    vector<PreSetting> getPreSetting()  const { return settings; }

    QueryManager getQueryManager()  const { return manager; }

    void selectPage(int index);

protected slots:

    void cancelButton();
    void okButton();

    void slotSelectGroup(int grp);
    void slotDeleteGroup(int grp);
    void slotNewGroup(const QString&);
    void slotModifyGroup(int);

protected:
    virtual void showEvent (QShowEvent*);

    QueryManager   manager;
    ThreshOptPage *threshOptPage;
    QueryOptPage  *queryOptPage;
    BlockOptPage  *blockOptPage;
    GroupOptPage  *groupOptPage;
    vector<PreSetting> settings;
};

#endif // QueryOptionsDlg_included
