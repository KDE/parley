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

class CommonEntryPage : public CommonEntryPageForm
{
    Q_OBJECT

public:

    CommonEntryPage
    (
        QDialog      *dlgbook,
        kvoctrainDoc *doc,
        bool        multi_sel,
        QString     expr,
        int         less,
        QComboBox  *lessBox,
        QString     lang,
        QString     type,
        QString     pronunce,
        QString     usage,
        QString     label,
        QueryManager &querymanager,
        QWidget* parent = NULL,
        const char* name = NULL
    );

    bool    lessonDirty  () const { return lesson_dirty; }
    bool    typeDirty    () const { return type_dirty; }

    int     getLesson  () const { return lesson; }
    QString getType    () const { return type; }
    QString getExpr    () const { return expression; }
    QString getPronunce() const { return pronunce; }
    QString getUsageLabel() const { return usageCollection; }

signals:
    void typeSelected(const QString &);

protected:
    void keyPressEvent( QKeyEvent * );
    void setTypeBox(const QString &act_type);
    void setLessonBox(QComboBox *lessbox, int lesson);
    void setUsageBox(const QString &act_type);

protected slots:
    void initFocus() const;
    void slotLessonSelected(int);
    void slotExprSelected(const QString&);
    void slotTypeSelected(int);
    void slotSubTypeSelected(int);
    void slotPronunceSelected (const QString&);
    void slotUsageChanged();

    void invokeLessDlg();
    void invokeTypeDlg();
    void invokePronDlg();
    void invokeUsageDlg();

protected:
    QString   pronunce;
    QString   expression;
    QString   usageCollection;
    int       lesson;
    QString   type;
    QDialog  *dlgbook;
    bool      lesson_dirty;
    bool      type_dirty;
    kvoctrainDoc *doc;

    vector<TypeRelation>  all_maintypes;
    vector<TypeRelation>  all_types;
    vector<UsageRelation> usages;
    vector<QString>       current_subtypes;
    QueryManager         &querymanager;
};
#endif // CommonEntryPage_included
