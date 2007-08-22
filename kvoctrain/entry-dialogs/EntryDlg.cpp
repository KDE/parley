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
    fromPage = 0;
    toPage = 0;

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
    //addPage(conjugationPage, i18n("Conjugation"));
    conjugationPageWidget = new KPageWidgetItem( conjugationPage, i18n( "Conjugation" ) );
    conjugationPageWidget->setHeader( i18n( "Conjugation of the selected verb" ) );
    conjugationPageWidget->setIcon( KIcon( "chronometer" ) );
    addPage(conjugationPageWidget);

    comparisonPage = new AdjEntryPage(m_doc, this);
    //addPage(comparisonPage, i18n("Comparison"));
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

    connect(commonPage, SIGNAL(sigModified()), this, SLOT(slotDisplayModified()));
    connect(additionalPage, SIGNAL(sigModified()), this, SLOT(slotDisplayModified()));
    connect(comparisonPage, SIGNAL(sigModified()), this, SLOT(slotDisplayModified()));
    connect(mc_page, SIGNAL(sigModified()), this, SLOT(slotDisplayModified()));
    connect(conjugationPage, SIGNAL(sigModified()), this, SLOT(slotDisplayModified()));

    connect(fromPage, SIGNAL(sigModified()), this, SLOT(slotDisplayModified()));
    connect(toPage, SIGNAL(sigModified()), this, SLOT(slotDisplayModified()));

    disableApplyButton();
    commonPage->expr_line->setFocus();
}


void EntryDlg::disableApplyButton()
{
    enableButtonApply(false);
    enableButton(User1, false);
}


void EntryDlg::slotApply()
{
    emit sigEditChoice(EditApply);
}


void EntryDlg::slotUndo()
{
    setData(m_currentRow, m_currentTranslation, m_selection);
}


bool EntryDlg::isModified()
{
    kDebug() << "Checking modified status of pages - row:" << m_currentRow << " trans: " << m_currentTranslation ;
    bool modified = false;

    if( commonPage->isEnabled() ) {
        if( commonPage->isModified() ) {
        kDebug() << "isModified(): commonPage";
            modified = true;
        }
    }
    if( additionalPage->isEnabled() ) {
        if( additionalPage->isModified() ) {
        kDebug() << "isModified(): additionalPage";
            modified = true;
        }
    }
    if( conjugationPage->isEnabled() ) {
        if( conjugationPage->isModified() ) {
        kDebug() << "isModified(): conjugationPage";
            modified = true;
        }
    }
    if( mc_page->isEnabled() ) {
        if( mc_page->isModified() ) {
        kDebug() << "isModified(): mc_page";
            modified = true;
        }
    }
    if( comparisonPage->isEnabled() ) {
        if( comparisonPage->isModified() ) {
        kDebug() << "isModified(): comparisonPage";
            modified = true;
        }
    }
    if( fromPage->isEnabled() ) {
        if( fromPage->isModified() ) {
        kDebug() << "isModified(): fromPage";
            modified = true;
        }
    }
    if( toPage->isEnabled() ) {
        if( toPage->isModified() ) {
        kDebug() << "isModified(): toPage";
            modified = true;
        }
    }
    return modified;
}


void EntryDlg::slotDisplayModified()
{
    enableButtonApply(true);
    enableButton(User1, true);
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


EntryDlg::~EntryDlg()
{
    if (docked) {
        docked = false;
        mainwin->resize(oldMainSize);
        mainwin->move(oldMainPos);
    }
}


void EntryDlg::closeEvent(QCloseEvent * e)
{
    Q_UNUSED(e);
    emit sigEditChoice(EditCancel);
}


void EntryDlg::setData(int currentRow, int currentTranslation, const QModelIndexList & selection)
{
    m_currentRow = currentRow;
    m_currentTranslation = currentTranslation;
    m_selection = selection;

    kDebug() << "EntryDlg::setData() selection - first row: " << m_selection.first().row() << " last: " << m_selection.last().row();

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
    disableApplyButton();
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

    disableApplyButton();
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


#include "EntryDlg.moc"
