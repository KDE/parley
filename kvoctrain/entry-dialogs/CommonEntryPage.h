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

#include "keduvocwordtype.h"
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

signals:
    void typeSelected(const QString &);

private:
    void updateMainTypeBoxContents();
    void setLessonBox(int lesson);
    void setUsageBox(const QString & act_type);

private slots:
    // to get noted upon modification
    void slotDataChanged();
    void slotDataChanged(int);
    void slotDataChanged(const QString&);

    /// Updates the usage label and emits sigModified()
    void slotUsageChanged();
    /// Updates the subtype box and emits sigModified()
    void slotUpdateSubTypeBoxContents(const QString &mainType);

    /**
     * Append a char from the phonetics dialog
     * @param  the new letter
     */
    void phoneticSelected(wchar_t);
    void invokeTypeDlg();
    void invokePronDlg();
    void invokeUsageDlg();
    void slotSubDialogClosed();

signals:
    void sigModified();

private:
    KEduVocDocument     *m_doc;

    QList<TypeRelation>  all_maintypes;
    QList<TypeRelation>  all_types;
    QList<UsageRelation> usages;
    QList<QString>       current_subtypes;

    KDialog             *subDialog;

    /// The row currently selected in the document - this is the entry number
    int               m_currentRow;
    /// Column in the document - corresponds to the language (-KV_EXTRA_COLS)
    int               m_currentTranslation;
    /// Selection in the doc - if more than one row is selected behavior is different
    QModelIndexList   m_selection;

    KEduVocWordType   m_wordTypes;
};
#endif // CommonEntryPage_included
