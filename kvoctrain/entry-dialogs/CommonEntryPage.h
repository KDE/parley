/***************************************************************************

    $Id$

                         edit common properties

    -----------------------------------------------------------------------

    begin                : Mon Jun 28 21:02:16 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.7  2002/01/06 15:57:07  arnold
    fixed handling of usage labels

    Revision 1.6  2001/12/30 12:12:57  arnold
    fixed smart appending and editing

    Revision 1.5  2001/12/13 18:39:29  arnold
    added phonetic alphabet stuff

    Revision 1.4  2001/11/17 17:58:55  arnold
    added inline editing of all columns

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

#ifndef CommonEntryPage_included
#define CommonEntryPage_included

#include "CommonEntryPageForm.h"

#include <QueryManager.h>
#include <UsageManager.h>

class PhoneticEntryPage;

class CommonEntryPage : public CommonEntryPageForm
{
    Q_OBJECT

public:

    CommonEntryPage
    (
        QDialog      *dlgbook,
        kvoctrainDoc *doc,
        bool          multi_sel,
        QString       expr,
        int           less,
        QComboBox    *lessBox,
        QString       lang,
        QString       type,
        QString       pronunce,
        QString       usage,
        QString       label,
        QueryManager &querymanager,
        bool          active,
        const QFont&  ipafont,
        QWidget* parent = NULL,
        const char* name = NULL
    );

    void setData(
        bool          multi_sel,
        QString       expr,
        int           less,
        QComboBox    *lessBox,
        QString       lang,
        QString       type,
        QString       pronunce,
        QString       usage,
        QString       label,
        QueryManager &querymanager,
        bool          active);

    ~CommonEntryPage ();


    bool isDirty() const;

    bool    lessonDirty  () const { return lesson_dirty; }
    bool    activeDirty  () const { return active_dirty; }
    bool    typeDirty    () const { return type_dirty; }
    bool    usageDirty   () const { return usage_dirty; }

    int     getLesson  ()   const { return lesson; }
    QString getType    ()   const { return type; }
    QString getExpr    ()   const { return expression; }
    QString getPronunce()   const { return pronunce; }
    QString getUsageLabel() const { return usageCollection; }
    bool    getActive()     const { return entry_active; }

    bool isModified();
    void setModified(bool mod = true);
    void setEnabled(int enable_type);

public slots:
    void initFocus() const;

signals:
    void typeSelected(const QString &);

protected:
    void keyPressEvent( QKeyEvent * );
    void setTypeBox(const QString &act_type);
    void setLessonBox(QComboBox *lessbox, int lesson);
    void setUsageBox( QString act_type);

protected slots:
    void slotLessonSelected(int);
    void slotExprSelected(const QString&);
    void slotTypeSelected(int);
    void slotSubTypeSelected(int);
    void slotPronunceSelected (const QString&);
    void slotUsageChanged();
    void slotActiveChanged(bool state);
    void phoneticSelected(wchar_t);
    void deletePronDlg();

    void invokeLessDlg();
    void invokeTypeDlg();
    void invokePronDlg();
    void invokeUsageDlg();

signals:
    void sigModified();

protected:
    QString      pronunce;
    QString       expression;
    QString       usageCollection;
    int           lesson;
    QString       type;
    QDialog      *dlgbook;
    bool          lesson_dirty;
    bool          type_dirty;
    kvoctrainDoc *doc;
    bool          entry_active;
    bool          active_dirty;
    bool          usage_dirty;

    PhoneticEntryPage  *phoneticDlg;

    vector<TypeRelation>  all_maintypes;
    vector<TypeRelation>  all_types;
    vector<UsageRelation> usages;
    vector<QString>       current_subtypes;
    QueryManager         &querymanager;
    QFont                 ipafont;
    bool                  modified;
};
#endif // CommonEntryPage_included
