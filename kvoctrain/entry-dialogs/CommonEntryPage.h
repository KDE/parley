/***************************************************************************

                         edit common properties

    -----------------------------------------------------------------------

    begin         : Mon Jun 28 21:02:16 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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
#include "query-dialogs/kvtquery.h"
#include <kvtusage.h>
#include <keduvocdocument.h>


class CommonEntryPage : public QWidget, public Ui::CommonEntryPageForm
{
    Q_OBJECT
public:
    CommonEntryPage(KEduVocDocument *doc, QWidget* parent = 0);

    void setData(int row, int col, const QModelIndexList & selection);
    void commitData();



    bool isModified();
    void setModified(bool mod = true);
    void setEnabled(bool enable);

signals:
    void typeSelected(const QString &);

private:
    void setTypeBox(const QString &act_type);
    void setLessonBox(int lesson);
    void setUsageBox(const QString & act_type);

private slots:
    void slotLessonSelected(int);
    void slotExprSelected(const QString&);
    void slotTypeSelected(int);
    void slotSubTypeSelected(int);
    void slotPronounceSelected(const QString&);
    void slotUsageChanged();
    void slotActiveChanged(bool state);
    void phoneticSelected(wchar_t);
    void invokeTypeDlg();
    void invokePronDlg();
    void invokeUsageDlg();
    void slotSubDialogClosed();

signals:
    void sigModified();

private:
    QString              m_pronounce;
    QString              m_expression;
    QString              m_usageCollection;
    int                  m_lesson;
    QString              m_type;
    KEduVocDocument     *m_doc;
    bool                 m_entry_active;

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

    /// The row currently selected in the document - this is the entry number
    int               m_currentRow;
    /// Column in the document - corresponds to the language (-KV_EXTRA_COLS)
    int               m_currentTranslation;
    /// Selection in the doc - if more than one row is selected behavior is different
    QModelIndexList   m_selection;
};
#endif // CommonEntryPage_included
