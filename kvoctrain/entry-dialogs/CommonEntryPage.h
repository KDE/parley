/***************************************************************************

                         edit common properties

    -----------------------------------------------------------------------

    begin         : Mon Jun 28 21:02:16 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

#ifndef CommonEntryPage_included
#define CommonEntryPage_included

#include "ui_CommonEntryPageForm.h"
#include <kvtquery.h>
#include <kvtusage.h>

class PhoneticEntryPage;
class KDialogBase;

class CommonEntryPage : public QWidget, public Ui::CommonEntryPageForm
{
    Q_OBJECT
public:
    CommonEntryPage(KEduVocDocument *doc, KVTQuery &querymanager, QWidget* parent = 0);

    void setData(bool multi_sel, const QString &expr, int less, const QString &type, const QString &pronounce, const QString &usage,  bool active);

    int     getLesson() const
    {
        return lesson;
    }
    QString getType() const
    {
        return m_type;
    }
    QString getExpr() const
    {
        return expression;
    }
    QString getPronounce() const
    {
        return m_pronounce;
    }
    QString getUsageLabel() const
    {
        return usageCollection;
    }
    bool    getActive() const
    {
        return entry_active;
    }

    bool usageIsModified()
    {
        return m_usageIsModified;
    }
    bool typeIsModified()
    {
        return m_typeIsModified;
    }
    bool lessonIsModified()
    {
        return m_lessonIsModified;
    }
    bool activeIsModified()
    {
        return m_activeIsModified;
    }
    bool isModified();
    void setModified(bool mod = true);
    void setEnabled(int enable_type);

signals:
    void typeSelected(const QString &);

protected:
    void setTypeBox(const QString &act_type);
    void setLessonBox(int lesson);
    void setUsageBox(const QString & act_type);

protected slots:
    void slotLessonSelected(int);
    void slotExprSelected(const QString&);
    void slotTypeSelected(int);
    void slotSubTypeSelected(int);
    void slotPronounceSelected(const QString&);
    void slotUsageChanged();
    void slotActiveChanged(bool state);
    void phoneticSelected(wchar_t);
    //void invokeLessDlg();
    void invokeTypeDlg();
    void invokePronDlg();
    void invokeUsageDlg();
    void slotSubDialogClosed();

signals:
    void sigModified();

protected:
    QString              m_pronounce;
    QString              expression;
    QString              usageCollection;
    int                  lesson;
    QString              m_type;
    KEduVocDocument     *doc;
    KVTQuery            &querymanager;
    bool                 entry_active;

    QList<TypeRelation>  all_maintypes;
    QList<TypeRelation>  all_types;
    QList<UsageRelation> usages;
    QList<QString>       current_subtypes;

    bool                 modified;
    KDialog             *subDialog;
    bool                 m_largeSelection;
    bool                 m_usageIsModified;
    bool                 m_typeIsModified;
    bool                 m_lessonIsModified;
    bool                 m_activeIsModified;
};
#endif // CommonEntryPage_included
