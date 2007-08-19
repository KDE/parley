/***************************************************************************

                   entry dialog for table cell contents

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

#ifndef EntryDlg_included
#define EntryDlg_included

#include <QItemSelection>
#include <QCloseEvent>

#include <kpagedialog.h>

#include "FromToEntryPage.h"
#include "CommonEntryPage.h"
#include "AuxInfoEntryPage.h"
#include "TenseEntryPage.h"
#include "AdjEntryPage.h"
#include "MCEntryPage.h"

class QTabWidget;
class KXmlGuiWindow;
class KEduVocDocument;

class EntryDlg : public KPageDialog
{
    Q_OBJECT

public:

    enum EditResult {EditCancel, EditApply, EditUndo, EditPageUp, EditPageDown, EditUp, EditDown, EditLeft, EditRight};

    enum EnableType {EnableAll, EnableOnlyCommon, EnableNone, EnableOnlyOriginal };

    EntryDlg(KXmlGuiWindow *main, KEduVocDocument *doc);

    ~EntryDlg();

    bool isModified();
    void setModified(bool mod);
    void setEnabled(int);

    void setData(int row, int col, const QModelIndexList & selection);

signals:
    void sigEditChoice(int);

public slots:
    void slotDisplayModified();
    void slotApply();
    void slotUndo();
    void slotDockHorizontal();
    void slotDockVertical();

protected slots:
    void updatePages(const QString &type);

protected:
    virtual void closeEvent(QCloseEvent*e);

    FromToEntryPage  *from_page;
    FromToEntryPage  *to_page;
    CommonEntryPage  *comm_page;
    AuxInfoEntryPage *aux_page;
    TenseEntryPage   *tense_page;
    AdjEntryPage     *adj_page;
    MCEntryPage      *mc_page;

    KXmlGuiWindow      *mainwin;
    QSize             oldMainSize;
    QPoint            oldMainPos;
    bool              docked;

public:
    /// saves the changes to m_doc
    void commitData(bool force);
private:
    /// reloads the expression data into the widgets
    void updateData ();


    KEduVocDocument *m_doc;
    /// The row currently selected in the document - this is the entry number
    int               m_currentRow;
    /// Column in the document - corresponds to the language (-KV_EXTRA_COLS)
    int               m_currentTranslation;
    /// Selection in the doc - if more than one row is selected behavior is different
    QModelIndexList   m_selection;
};

#endif // EntryDlg_included
