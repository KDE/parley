/***************************************************************************

                   entry dialog for table cell contents

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
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

#include "EntryDlg.h"

#include <QLayout>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFrame>
#include <QCloseEvent>

#include <klocale.h>
#include <kxmlguiwindow.h>
#include <kwindowsystem.h>
#include <KMessageBox>

EntryDlg::EntryDlg(KXmlGuiWindow *main, KEduVocDocument *doc) : KPageDialog()
{
    setButtons(User1|User2|User3|Apply|Close);
    setDefaultButton(Apply);
    setFaceType(KPageDialog::List);
    setModal(false);
    setButtonGuiItem(User1, KGuiItem(i18n("&Reset")));
    setButtonGuiItem(User2, KGuiItem(QString(), "view-left-right"));
    setButtonGuiItem(User3, KGuiItem(QString(), "view-top-bottom"));

    mainwin = main;
    docked = false;
    m_currentRow = -1;
    m_currentTranslation = -1;
    m_doc = doc;

    commonPage = new CommonEntryPage(m_doc, this);
    commonPageWidget = new KPageWidgetItem( commonPage, i18n( "Common" ) );
    commonPageWidget->setHeader( i18n( "General properties of the word" ) );
    commonPageWidget->setIcon( KIcon( "edit_table_row" ) );
    addPage(commonPageWidget);

    additionalPage = new AuxInfoEntryPage(m_doc, this);
    additionalPageWidget = new KPageWidgetItem( additionalPage, i18n( "Additional" ) );
    additionalPageWidget->setHeader( i18n( "Additional properties" ) );
    additionalPageWidget->setIcon( KIcon( "configure" ) );
    addPage(additionalPageWidget);

    mc_page = new MCEntryPage(m_doc, this);
    multipleChoicePageWidget = new KPageWidgetItem( mc_page, i18n( "Multiple Choice" ) );
    multipleChoicePageWidget->setHeader( i18n( "Predetermined multiple choice suggestions" ) );
    multipleChoicePageWidget->setIcon( KIcon( "run_multi" ) );
    addPage(multipleChoicePageWidget);

    conjugationPage = new TenseEntryPage(m_doc, this);
    conjugationPageWidget = new KPageWidgetItem( conjugationPage, i18n( "Conjugation" ) );
    conjugationPageWidget->setHeader( i18n( "Conjugation of the selected verb" ) );
    conjugationPageWidget->setIcon( KIcon( "chronometer" ) );
    addPage(conjugationPageWidget);

    comparisonPage = new AdjEntryPage(m_doc, this);
    comparisonPageWidget = new KPageWidgetItem( comparisonPage, i18n( "Comparison" ) );
    comparisonPageWidget->setHeader( i18n( "Comparison forms of the selected adjective or adverb" ) );
    comparisonPageWidget->setIcon( KIcon( "arrow-up" ) );
    addPage(comparisonPageWidget);

    fromPage = new FromToEntryPage(m_doc, this);
    fromPageWidget = new KPageWidgetItem( fromPage,  i18n("From Original") );
    fromPageWidget->setHeader( i18n( "Grades when asked to type in this language" ) );
    fromPageWidget->setIcon( KIcon( "statistics" ) );
    addPage(fromPageWidget);

    toPage = new FromToEntryPage(m_doc, this);
    toPageWidget = new KPageWidgetItem( toPage, i18n( "To Original" ) );
    toPageWidget->setHeader( i18n( "Grades when asked to enter the original language" ) );
    toPageWidget->setIcon( KIcon( "statistics" ) );
    addPage(toPageWidget);

    connect(commonPage, SIGNAL(typeSelected(const QString&)), SLOT(slotTypeChanged(const QString&)));

    connect(this, SIGNAL(user1Clicked()), this, SLOT(slotUndo()));
    connect(this, SIGNAL(applyClicked()), this, SLOT(slotApply()));
    connect(this, SIGNAL(user2Clicked()), this, SLOT(slotDockVertical()));
    connect(this, SIGNAL(user3Clicked()), this, SLOT(slotDockHorizontal()));

    connect(commonPage, SIGNAL(sigModified()), this, SLOT(slotChildPageModified()));
    connect(additionalPage, SIGNAL(sigModified()), this, SLOT(slotChildPageModified()));
    connect(comparisonPage, SIGNAL(sigModified()), this, SLOT(slotChildPageModified()));
    connect(mc_page, SIGNAL(sigModified()), this, SLOT(slotChildPageModified()));
    connect(conjugationPage, SIGNAL(sigModified()), this, SLOT(slotChildPageModified()));

    connect(fromPage, SIGNAL(sigModified()), this, SLOT(slotChildPageModified()));
    connect(toPage, SIGNAL(sigModified()), this, SLOT(slotChildPageModified()));

    commonPage->expr_line->setFocus();
    setModified(false);
}


EntryDlg::~EntryDlg()
{
    if (docked) {
        docked = false;
        mainwin->resize(oldMainSize);
        mainwin->move(oldMainPos);
    }
}


void EntryDlg::setModified(bool isModified)
{
    enableButtonApply(isModified);
    enableButton(User1, isModified);
    m_modified = isModified;
}


void EntryDlg::slotApply()
{
    commitData(true);
    setModified(false);

/// @todo eventually append a row if we are in the last row???

/*
        commitEntryDlg(true);
        if (Prefs::smartAppend()) {
            int row = m_tableView->currentIndex().row();
            if (row == m_tableModel->rowCount(QModelIndex()) - 1) {
                int col = m_tableView->currentIndex().column();
                if (col < m_tableModel->columnCount(QModelIndex()) - 1 && col >= KV_COL_ORG) {
                    int lesson = m_doc->entry(row)->lesson();
                    //if (lesson >= m_lessonsComboBox->count())
                    //lesson = qMax (0, m_lessonsComboBox->count()-1);
                    m_lessonView->slotSelectLesson(lesson);

                    QString exp;
                    exp = m_doc->entry(row)->translation(col+1-KV_COL_ORG).translation();
                    if (exp.isEmpty())
                        m_tableView->setCurrentIndex(m_tableModel->index(row, col + 1));
                } else
                    slotNewEntry();
                    //slotAppendRow();
            }
        }*/
}


void EntryDlg::slotUndo()
{
    setData(m_currentRow, m_currentTranslation, m_selection);
}


bool EntryDlg::isModified()
{
    return m_modified;
}


void EntryDlg::slotDockVertical()
{
// FIXME: should this be handled in KWinModule?
#ifdef Q_WS_X11
    if (!docked) {
        oldMainPos = mainwin->pos();
        oldMainSize = mainwin->size();
        docked = true;
    }

    QRect rect = KWindowSystem::workArea();

    int diff_x = frameGeometry().width()-width();
    int diff_y = frameGeometry().height()-height();
    resize(minimumWidth(), rect.height()-diff_y);
    mainwin->resize(rect.width()-frameGeometry().width()-diff_x,
                    rect.height()-diff_y);
    move(0, 0);
    mainwin->move(frameGeometry().width(), 0);
#endif
}


void EntryDlg::slotDockHorizontal()
{
#ifdef Q_WS_X11
    if (!docked) {
        oldMainPos = mainwin->pos();
        oldMainSize = mainwin->size();
        docked = true;
    }

    QRect rect = KWindowSystem::workArea();

    int diff_x = frameGeometry().width()-width();
    int diff_y = frameGeometry().height()-height();

    resize(rect.width()-diff_x, minimumHeight());
    mainwin->resize(rect.width()-diff_x,
                    rect.height()-frameGeometry().height()-diff_y);
    move(0, 0);
    mainwin->move(0, frameGeometry().height());
#endif
}


void EntryDlg::closeEvent(QCloseEvent * e)
{
    Q_UNUSED(e);
    commitData(false);
    emit closeEntryDialog();
}


void EntryDlg::setData(int currentRow, int currentTranslation, const QModelIndexList & selection)
{
    // I suppose we could switch over to only use the selection and translation.
    m_selection = selection;
    m_currentTranslation = currentTranslation;

    if ( m_selection.count() > 1 ) {
        // if we receive multiple entries, use the first of them.
        m_currentRow = m_selection.first().row();
    } else {
        m_currentRow = currentRow;
    }


// FOR TESTING ONLY, REMOVE SOON:
    kDebug() << "EntryDlg::setData() row: " << currentRow << " selection - first row: " << m_selection.first().row() << " last: " << m_selection.last().row();

    bool found = false;
    foreach(QModelIndex mi, selection) {
        if ( mi.row() == currentRow ) {
            found = true;
        }
    }

    if(!found) {
        kError() << "currentRow was not in the selection. this is a serious BUG!";
    }


    updatePages();
}


void EntryDlg::updatePages()
{
    QString title;
    if (m_currentTranslation < 0) {
        title = i18n("Edit General Properties");
    } else if (m_currentTranslation == 0) {
        title = i18n("Edit Properties for Original");
    } else {
        title = i18n("Edit Properties of a Translation");
    }
    setCaption(title);

    bool editMultipleRows = (m_selection.count() > 1);

    commonPage->setData(m_currentRow, m_currentTranslation, m_selection);

    if(editMultipleRows) {
        comparisonPage->clear();
        conjugationPage->clear();
        additionalPage->clear();
        mc_page->clear();
    } else {
        comparisonPage->setData( m_currentRow, m_currentTranslation );
        conjugationPage->setData( m_currentRow, m_currentTranslation );
        additionalPage->setData( m_currentRow, m_currentTranslation );
        mc_page->setData( m_currentRow, m_currentTranslation );
    }

// for now use the old grading system only to/from original
// these are only valid if we edit a translation > 0. Otherwise they are disabled
    if ( m_currentTranslation > 0 ) {
        fromPage->setData( m_currentRow, m_currentTranslation, 0, m_selection);
        toPage->setData( m_currentRow, 0, m_currentTranslation, m_selection);
    }

    // we always have the common page enabled

    // no translation selected (index < 0) leaves only common
    if ( m_currentTranslation < 0 ) {
        kDebug() << "EntryDlg::updatePages() trans index < 0";
        // remove all pages that are currently there
        additionalPageWidget->setEnabled(false);
        multipleChoicePageWidget->setEnabled(false);
        conjugationPageWidget->setEnabled(false);
        comparisonPageWidget->setEnabled(false);
        fromPageWidget->setEnabled(false);
        toPageWidget->setEnabled(false);
    } else {

        if ( m_currentTranslation > 0 ) {
            fromPageWidget->setEnabled(true);
            toPageWidget->setEnabled(true);
        } else {
            fromPageWidget->setEnabled(false);
            toPageWidget->setEnabled(false);
        }

        // multiple selection: have only common and grading pages
        if ( m_selection.count() > 1 ) {
            kDebug() << "EntryDlg::updatePages() count > 1";
            additionalPageWidget->setEnabled(false);
            multipleChoicePageWidget->setEnabled(false);
            conjugationPageWidget->setEnabled(false);
            comparisonPageWidget->setEnabled(false);
        } else {
            // only one entry selected - now add entry specific pages
            additionalPageWidget->setEnabled(true);
            multipleChoicePageWidget->setEnabled(true);

            QString type = m_doc->entry(m_currentRow)->translation(m_currentTranslation).type();

            slotTypeChanged( type );
        } // single entry selected
    }

    if ( !currentPage()->isEnabled() ) {
        setCurrentPage(commonPageWidget);
    }
    setModified(false);
}

void EntryDlg::commitData(bool force)
{
    if ( !isModified() ) {
        return;
    }

    if ( m_currentRow < 0 ) {
        return;
    }

    if (!force && !Prefs::autoEntryApply()) {
        if (KMessageBox::No == KMessageBox::warningYesNo(this,
                i18n("The entry dialog contains unsaved changes.\n"
                     "Do you want to apply or discard your changes?"),
                i18n("Unsaved Changes"),
                KStandardGuiItem::apply(), KStandardGuiItem::discard())) {
            return;
        }
    }

    if( commonPage->isEnabled() ) {
        commonPage->commitData();
    }
    if( fromPage->isEnabled() ) {
        fromPage->commitData();
    }
    if( toPage->isEnabled() ) {
        toPage->commitData();
    }
    if( additionalPage->isEnabled() ) {
        additionalPage->commitData();
    }
    if( conjugationPage->isEnabled() ) {
        conjugationPage->commitData();
    }
    if( comparisonPage->isEnabled() ) {
        comparisonPage->commitData();
    }
    if( mc_page->isEnabled() ) {
        mc_page->commitData();
    }


//m_tableModel->setData(m_tableModel->index(m_currentRow, 0), getLesson(), Qt::EditRole);            //m_tableModel->setData(m_tableModel->index(m_currentRow, m_currentTranslation), getExpr(), Qt::EditRole);

    /// @todo emit data changed signal for the big table
kDebug() << "Changes should be committed but the table is not updated. FIXME";

    emit dataChanged(m_selection.first(), m_selection.last());

    setModified(false);
    m_doc->setModified(true);
}

void EntryDlg::slotTypeChanged(const QString & type)
{
    if ( m_selection.count() > 1 ) {
        conjugationPageWidget->setEnabled(false);
        comparisonPageWidget->setEnabled(false);
        return;
    }

    // enable/disable conjugation or comparison pages:
    QString main;
    int pos;
    if ((pos = type.indexOf(QM_TYPE_DIV)) < 0) {  // only use main type
        main = type;
    } else {
        main = type.left(pos);
    }

    if (main == QM_VERB) {
        conjugationPageWidget->setEnabled(true);
    } else {
        conjugationPageWidget->setEnabled(false);
    }

    if (main == QM_ADJ) {
        comparisonPageWidget->setEnabled(true);
    } else {
        comparisonPageWidget->setEnabled(false);
    }

}

void EntryDlg::slotChildPageModified()
{
    enableButtonApply(true);
    enableButton(User1, true);
    m_modified = true;
}


#include "EntryDlg.moc"
