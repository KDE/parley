/***************************************************************************

                   entry dialog for table cell contents

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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
#include "additionaleditpage.h"
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

    /// saves the changes to m_doc
    void commitData(bool force);

public slots:
    void setData(const QList<int>& entries, int currentTranslation);

signals:
    /**
     * To notify the child pages of a new selection.
     * @param
     * @param
     */
    void signalSetData( const QList<int>& entries, int currentTranslation);
    /**
     * Tell the child pages to save their data.
     */
    void signalCommitData();
    void closeEntryDialog();

private slots:
    void slotTypeChanged(const QString& type);
    void slotChildPageModified();
    void slotApply();
    void slotUndo();
    void slotDockHorizontal();
    void slotDockVertical();

private:
    void closeEvent(QCloseEvent*e);

    void setModified(bool isModified);

    /// reloads the expression data into the widgets
    void updatePages ();

    /// The dialog pages
    CommonEntryPage  *commonPage;
    AdditionalEditPage *additionalPage;
    TenseEntryPage   *conjugationPage;
    AdjEntryPage     *comparisonPage;
    MCEntryPage      *mc_page;

    /// Container for KPageDialog
    KPageWidgetItem  *commonPageWidget;
    KPageWidgetItem  *additionalPageWidget;
    KPageWidgetItem  *multipleChoicePageWidget;
    KPageWidgetItem  *comparisonPageWidget;
    KPageWidgetItem  *conjugationPageWidget;

    KXmlGuiWindow      *mainwin;
    QSize             oldMainSize;
    QPoint            oldMainPos;
    bool              docked;

    KEduVocDocument *m_doc;
//    // The row currently selected in the document - this is the entry number
//     int               m_currentRow;
    /// Column in the document - corresponds to the language (-KV_COL_TRANS)
    int               m_currentTranslation;

    /// Selection in the doc - if more than one row is selected behavior is different
    QList<int>        m_entries;

    /// Modified status of child pages. Needs to be reset when committing etc.
    bool m_modified;
};

#endif // EntryDlg_included
