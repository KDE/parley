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
    setButtons(User1|Ok|Apply|Close);
    setDefaultButton(Apply);
    setFaceType(KPageDialog::List);
    setModal(false);
    setButtonGuiItem(User1, KGuiItem(i18n("&Reset")));

    mainwin = main;
    docked = false;
    m_currentTranslation = -1;
    m_doc = doc;

    commonPage = new CommonEntryPage(m_doc, this);
    commonPageWidget = new KPageWidgetItem( commonPage, i18nc("edit general vocabulary properties that are common to all word types", "Common" ) );
    commonPageWidget->setHeader( i18n( "General properties of the word" ) );
    commonPageWidget->setIcon( KIcon( "edit_table_row" ) );
    addPage(commonPageWidget);

    additionalPage = new AdditionalEditPage(m_doc, this);
    additionalPageWidget = new KPageWidgetItem( additionalPage, i18nc( "edit additional properties like image, synonym, etc.", "Additional" ) );
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

    connect(commonPage, SIGNAL(signalTypeSelected(const QString&)), this,  SLOT(slotTypeChanged(const QString&)));

    connect(this, SIGNAL(user1Clicked()), this, SLOT(slotUndo()));
    connect(this, SIGNAL(applyClicked()), this, SLOT(slotApply()));
    connect(this, SIGNAL(okClicked()), this, SLOT(slotOk()));

    connect(commonPage, SIGNAL(sigModified()), this, SLOT(slotChildPageModified()));
    connect(additionalPage, SIGNAL(sigModified()), this, SLOT(slotChildPageModified()));
    connect(comparisonPage, SIGNAL(sigModified()), this, SLOT(slotChildPageModified()));
    connect(mc_page, SIGNAL(sigModified()), this, SLOT(slotChildPageModified()));
    connect(conjugationPage, SIGNAL(sigModified()), this, SLOT(slotChildPageModified()));

    commonPage->expr_line->setFocus();
    setModified(false);

    for ( int identifier1 = 0; identifier1 < m_doc->identifierCount(); identifier1++ ) {
        for ( int identifier2 = identifier1 + 1; identifier2 < m_doc->identifierCount(); identifier2++ ) {
            FromToEntryPage* gradePage = new FromToEntryPage(m_doc, identifier1, identifier2, this);
            QString title = i18n("Grades from \n%1 to %2", m_doc->identifier(identifier1).name(), m_doc->identifier(identifier2).name());
            KPageWidgetItem* gradePageWidget = new KPageWidgetItem( gradePage, title );
            gradePageWidget->setHeader( title );
//             gradePageWidget->setIcon( KIcon( "statistics" ) );
            addPage(gradePageWidget);
            connect(gradePage, SIGNAL(sigModified()), this, SLOT(slotChildPageModified()));
            connect(this, SIGNAL(signalSetData(const QList<int>&, int)), gradePage, SLOT(setData(const QList<int>&)));
            connect(this, SIGNAL(signalCommitData()), gradePage, SLOT(commitData()));

            gradePage = new FromToEntryPage(m_doc, identifier2, identifier1, this);
            title = i18n("Grades from \n%1 to %2", m_doc->identifier(identifier2).name(), m_doc->identifier(identifier1).name());

            gradePageWidget = new KPageWidgetItem( gradePage, title );
            gradePageWidget->setHeader( title );
//             gradePageWidget->setIcon( KIcon( "statistics" ) );
            addPage(gradePageWidget);
            connect(gradePage, SIGNAL(sigModified()), this, SLOT(slotChildPageModified()));
            connect(this, SIGNAL(signalSetData(const QList<int>&, int)), gradePage, SLOT(setData(const QList<int>&)));
            connect(this, SIGNAL(signalCommitData()), gradePage, SLOT(commitData()));
        }
    }
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

//         if (Prefs::smartAppend()) {
//             int row = m_tableView->currentIndex().row();
//             if (row == m_tableModel->rowCount(QModelIndex()) - 1) {
//                 int col = m_tableView->currentIndex().column();
//                 if (col < m_tableModel->columnCount(QModelIndex()) - 1 && col >= KV_COL_ORG) {
//                     int lesson = m_doc->entry(row)->lesson();
//                     //if (lesson >= m_lessonsComboBox->count())
//                     //lesson = qMax (0, m_lessonsComboBox->count()-1);
//                     m_lessonView->slotSelectLesson(lesson);
//
//                     QString exp;
//                     exp = m_doc->entry(row)->translation(col+1-KV_COL_ORG).translation();
//                     if (exp.isEmpty())
//                         m_tableView->setCurrentIndex(m_tableModel->index(row, col + 1));
//                 } else
//                     slotNewEntry();
//                     //slotAppendRow();
//             }
//         }
}


void EntryDlg::slotUndo()
{
    setData(m_entries, m_currentTranslation);
}


bool EntryDlg::isModified()
{
    return m_modified;
}


void EntryDlg::closeEvent(QCloseEvent * e)
{
    Q_UNUSED(e);
    commitData(false);
    emit closeEntryDialog();
}


void EntryDlg::setData(const QList<int>& entries, int currentTranslation)
{
    m_entries = entries;
    m_currentTranslation = currentTranslation;

    if ( entries.first() < 0 ) {
        kDebug() << "EntryDlg::setData() invalid selection: translation: " << currentTranslation << " selection - first row: " << m_entries.first() << " last: " << m_entries.last();
        return;
    }

// FOR TESTING ONLY, REMOVE SOON:
    kDebug() << "EntryDlg::setData() translation: " << currentTranslation << " selection - first row: " << m_entries.first() << " last: " << m_entries.last();

    updatePages();
}


void EntryDlg::updatePages()
{
    bool editMultipleRows = (m_entries.count() > 1);

    QString title;
    if (m_currentTranslation < 0) {
        title = i18np("Edit General Properties", "Edit General Properties for %1 Files", m_entries.count());
    } else {
        if ( !editMultipleRows ) {
            title = i18nc("@title:window Editing one entry, %1 the word, %2 the language", "\"%1\" (%2)",
                m_doc->entry(m_entries.value(0))->translation(m_currentTranslation).text(),
                m_doc->identifier(m_currentTranslation).name() );
        } else {
            title = i18ncp("@title:window Editing %1 entries, %2 the language", "Edit One Word (%2)", "Edit %1 words (%2)",
                m_entries.count(),
                m_doc->identifier(m_currentTranslation).name() );
        }
    }
    setCaption(title);

    commonPage->setData(m_entries, m_currentTranslation);

    if(editMultipleRows) {
        comparisonPage->clear();
        conjugationPage->clear();
        additionalPage->clear();
        mc_page->clear();
    } else {
        comparisonPage->setData( m_entries.value(0), m_currentTranslation );
        conjugationPage->setData( m_entries.value(0), m_currentTranslation );
        additionalPage->setData( m_entries.value(0), m_currentTranslation );
        mc_page->setData( m_entries.value(0), m_currentTranslation );
    }

    // for the grades pages
    emit signalSetData( m_entries, m_currentTranslation);

    // we always have the common page enabled

    // no translation selected (index < 0) leaves only common
    if ( m_currentTranslation < 0 ) {
        kDebug() << "EntryDlg::updatePages() trans index < 0";
        // remove all pages that are currently there
        additionalPageWidget->setEnabled(false);
        multipleChoicePageWidget->setEnabled(false);
        conjugationPageWidget->setEnabled(false);
        comparisonPageWidget->setEnabled(false);
    } else {
        // multiple selection: have only common and grading pages
        if ( m_entries.count() > 1 ) {
            kDebug() << "EntryDlg::updatePages() count > 1";
            additionalPageWidget->setEnabled(false);
            multipleChoicePageWidget->setEnabled(false);
            conjugationPageWidget->setEnabled(false);
            comparisonPageWidget->setEnabled(false);
        } else {
            // only one entry selected - now add entry specific pages
            additionalPageWidget->setEnabled(true);
            multipleChoicePageWidget->setEnabled(true);

            QString type = m_doc->entry(m_entries.value(0))->translation(m_currentTranslation).type();

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

    emit signalCommitData();

//m_tableModel->setData(m_tableModel->index(m_currentRow, 0), getLesson(), Qt::EditRole);            //m_tableModel->setData(m_tableModel->index(m_currentRow, m_currentTranslation), getExpr(), Qt::EditRole);

    /// @todo emit data changed signal for the big table
kDebug() << "Changes should be committed but the table is not updated. FIXME";

    setModified(false);
    m_doc->setModified(true);
}

void EntryDlg::slotTypeChanged(const QString & type)
{

    kDebug() << "EntryDlg::slotTypeChanged: " << type;

    if ( m_entries.count() > 1 ) {
        conjugationPageWidget->setEnabled(false);
        comparisonPageWidget->setEnabled(false);
        return;
    }

    // enable/disable conjugation or comparison pages:
    if (m_doc->wordTypes().specialType(type) == m_doc->wordTypes().specialTypeVerb()) {
        conjugationPageWidget->setEnabled(true);
    } else {
        conjugationPageWidget->setEnabled(false);
    }

    if (m_doc->wordTypes().specialType(type) == m_doc->wordTypes().specialTypeAdjective()
        || m_doc->wordTypes().specialType(type) == m_doc->wordTypes().specialTypeAdverb()) {
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

void EntryDlg::slotOk()
{
    slotApply();
    emit closeEntryDialog();
}


#include "EntryDlg.moc"
