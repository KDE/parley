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

    EntryDlg(KXmlGuiWindow *main, KEduVocDocument *doc);

    ~EntryDlg();

    bool isModified();
    void disableApplyButton();
    void setData(int row, int col, const QModelIndexList & selection);

    /// saves the changes to m_doc
    void commitData(bool force);

signals:
    void sigEditChoice(int);

public slots:
    void slotDisplayModified();
    void slotApply();
    void slotUndo();
    void slotDockHorizontal();
    void slotDockVertical();

private slots:
    void updatePages(const QString &type);

private:
    void closeEvent(QCloseEvent*e);

    /// reloads the expression data into the widgets
    void updateData ();

    /// The dialog pages
    CommonEntryPage  *commonPage;
    FromToEntryPage  *fromPage;
    FromToEntryPage  *toPage;
    AuxInfoEntryPage *additionalPage;
    TenseEntryPage   *conjugationPage;
    AdjEntryPage     *comparisonPage;
    MCEntryPage      *mc_page;

    /// Container for KPageDialog
    KPageWidgetItem  *commonPageWidget;
    KPageWidgetItem  *additionalPageWidget;
    KPageWidgetItem  *multipleChoicePageWidget;
    KPageWidgetItem  *comparisonPageWidget;
    KPageWidgetItem  *conjugationPageWidget;
    KPageWidgetItem  *fromPageWidget;
    KPageWidgetItem  *toPageWidget;

    KXmlGuiWindow      *mainwin;
    QSize             oldMainSize;
    QPoint            oldMainPos;
    bool              docked;




    KEduVocDocument *m_doc;
    /// The row currently selected in the document - this is the entry number
    int               m_currentRow;
    /// Column in the document - corresponds to the language (-KV_EXTRA_COLS)
    int               m_currentTranslation;
    /// Selection in the doc - if more than one row is selected behavior is different
    QModelIndexList   m_selection;
};

#endif // EntryDlg_included
