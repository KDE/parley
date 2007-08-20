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
    from_page = 0;
    to_page = 0;

    m_doc = doc;

    commonPage = new CommonEntryPage(m_doc, this);
    commonPageWidget = new KPageWidgetItem( commonPage, i18n( "Common" ) );
    commonPageWidget->setHeader( i18n( "General properties of the word" ) );
    commonPageWidget->setIcon( KIcon( "kvoctrain" ) );
    addPage(commonPageWidget);

    additionalPage = new AuxInfoEntryPage(m_doc, this);
    additionalPageWidget = new KPageWidgetItem( additionalPage, i18n( "Additional" ) );
    additionalPageWidget->setHeader( i18n( "Additional properties" ) );
    additionalPageWidget->setIcon( KIcon( "kvoctrain" ) );
    addPage(additionalPageWidget);

    mc_page = new MCEntryPage(m_doc, this);
    multipleChoicePageWidget = new KPageWidgetItem( mc_page, i18n( "Multiple Choice" ) );
    multipleChoicePageWidget->setHeader( i18n( "Predetermined multiple choice suggestions" ) );
    multipleChoicePageWidget->setIcon( KIcon( "kvoctrain" ) );
    addPage(multipleChoicePageWidget);

    conjugationPage = new TenseEntryPage(m_doc, this);
    //addPage(conjugationPage, i18n("Conjugation"));
    conjugationPageWidget = new KPageWidgetItem( conjugationPage, i18n( "Conjugation" ) );
    conjugationPageWidget->setHeader( i18n( "Conjugation of the selected verb" ) );
    conjugationPageWidget->setIcon( KIcon( "kvoctrain" ) );
    addPage(conjugationPageWidget);

    comparisonPage = new AdjEntryPage(m_doc, this);
    //addPage(comparisonPage, i18n("Comparison"));
    comparisonPageWidget = new KPageWidgetItem( comparisonPage, i18n( "Comparison" ) );
    comparisonPageWidget->setHeader( i18n( "Comparison forms of the selected adjective or adverb" ) );
    comparisonPageWidget->setIcon( KIcon( "kvoctrain" ) );
    addPage(comparisonPageWidget);

    from_page = new FromToEntryPage(m_doc, this);
    fromPageWidget = new KPageWidgetItem( from_page,  i18n("From Original") );
    fromPageWidget->setHeader( i18n( "Grades when asked to type in this language" ) );
    fromPageWidget->setIcon( KIcon( "kvoctrain" ) );
    addPage(fromPageWidget);

    to_page = new FromToEntryPage(m_doc, this);
    toPageWidget = new KPageWidgetItem( to_page, i18n( "To Original" ) );
    toPageWidget->setHeader( i18n( "Grades when asked to enter the original language" ) );
    toPageWidget->setIcon( KIcon( "kvoctrain" ) );
    addPage(toPageWidget);

    connect(commonPage, SIGNAL(typeSelected(const QString&)), SLOT(updatePages(const QString&)));

    connect(this, SIGNAL(user1Clicked()), this, SLOT(slotUndo()));
    connect(this, SIGNAL(applyClicked()), this, SLOT(slotApply()));
    connect(this, SIGNAL(user2Clicked()), this, SLOT(slotDockVertical()));
    connect(this, SIGNAL(user3Clicked()), this, SLOT(slotDockHorizontal()));

    connect(commonPage, SIGNAL(sigModified()), this, SLOT(slotDisplayModified()));
    connect(additionalPage, SIGNAL(sigModified()), this, SLOT(slotDisplayModified()));
    connect(comparisonPage, SIGNAL(sigModified()), this, SLOT(slotDisplayModified()));
    connect(mc_page, SIGNAL(sigModified()), this, SLOT(slotDisplayModified()));
    connect(conjugationPage, SIGNAL(sigModified()), this, SLOT(slotDisplayModified()));

    connect(from_page, SIGNAL(sigModified()), this, SLOT(slotDisplayModified()));
    connect(to_page, SIGNAL(sigModified()), this, SLOT(slotDisplayModified()));

    enableButtonApply(false);
    enableButton(User1, false);
    setModified(false);
    commonPage->expr_line->setFocus();
}


void EntryDlg::setModified(bool mod)
{
    commonPage->setModified(mod);
    mc_page->setModified(mod);
    mc_page->setModified(mod);
    if (from_page != 0)
        from_page->setModified(mod);
    if (to_page != 0)
        to_page->setModified(mod);
    enableButtonApply(false);
    enableButton(User1, false);
}

/*
void EntryDlg::setEnabled(int enable)
{

kDebug() << " EntryDlg::setEnabled() " << m_currentTranslation;
    QString type = commonPage->getType();
    QString main;
    int pos;
    if ((pos = type.indexOf(QM_TYPE_DIV)) < 0) {  // only use main type
        main = type;
    } else {
        main = type.left(pos);
    }

    if ( m_selection.count() == 1 && m_currentTranslation >= 0 ) {
        comparisonPage->setEnabled(main == QM_ADJ);
    } else {
        comparisonPage->setEnabled(false);
    }

    if (enable == EnableOnlyOriginal) {
        commonPage->setEnabled(EnableAll);
        additonalPage->setEnabled(EnableAll);
        mc_page->setEnabled(EnableAll);
        conjugationPage->setEnabled(main == QM_VERB ? EnableAll : EnableNone);
        mc_page->setEnabled(EnableAll);
        if (from_page != 0)
            from_page->setEnabled(EnableNone);
        if (to_page != 0)
            to_page->setEnabled(EnableNone);
    } else {
        commonPage->setEnabled(enable);
        additonalPage->setEnabled(enable);
        mc_page->setEnabled(enable);
        conjugationPage->setEnabled(main == QM_VERB ? enable : EnableNone);
        mc_page->setEnabled(enable);
        if (from_page != 0)
            from_page->setEnabled(enable);
        if (to_page != 0)
            to_page->setEnabled(enable);
    }
}
*/

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
    bool modified = false;

    if( commonPage->isModified() ) {
        modified = true;
    }
    if( additionalPage->isModified() ) {
        modified = true;
    }
    if( conjugationPage->isModified() ) {
        modified = true;
    }
    if( mc_page->isModified() ) {
        modified = true;
    }
    if( comparisonPage->isModified() ) {
        modified = true;
    }
    if( from_page->isModified() ) {
        modified = true;
    }
    if( to_page->isModified() ) {
        modified = true;
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

    updateData();
}


void EntryDlg::updateData()
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
    } else {
        comparisonPage->setData( m_currentRow, m_currentTranslation );
        conjugationPage->setData( m_currentRow, m_currentTranslation );
        additionalPage->setData( m_currentRow, m_currentTranslation );
    }

    mc_page->setData(
        m_doc->entry(m_currentRow)->translation(m_currentTranslation).multipleChoice());

// for now use the old grading system only to/from original
// these are only valid if we edit a translation > 0. Otherwise they are disabled

    from_page->setData(editMultipleRows,
            m_doc->entry(m_currentRow)->translation(m_currentTranslation).gradeFrom(0).grade(),
            m_doc->entry(m_currentRow)->translation(m_currentTranslation).gradeFrom(0).queryDate(),
            m_doc->entry(m_currentRow)->translation(m_currentTranslation).gradeFrom(0).queryCount(),
            m_doc->entry(m_currentRow)->translation(m_currentTranslation).gradeFrom(0).badCount(),
            m_doc->entry(m_currentRow)->translation(m_currentTranslation).falseFriend(0),
            i18n("Properties From Original"));

    to_page->setData(editMultipleRows,
            m_doc->entry(m_currentRow)->translation(0).gradeFrom(m_currentTranslation).grade(),
            m_doc->entry(m_currentRow)->translation(0).gradeFrom(m_currentTranslation).queryDate(),
            m_doc->entry(m_currentRow)->translation(0).gradeFrom(m_currentTranslation).queryCount(),
            m_doc->entry(m_currentRow)->translation(0).gradeFrom(m_currentTranslation).badCount(),
            m_doc->entry(m_currentRow)->translation(0).falseFriend(m_currentTranslation),
            i18n("Properties to Original"));

    setModified(false);
    updatePages( m_doc->entry(m_currentRow)->translation(m_currentTranslation).type() );
}


void EntryDlg::updatePages(const QString &type)
{
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
        return;
    }

    // multiple selection: have only common and grading pages
    if ( m_selection.count() > 1 ) {
        kDebug() << "EntryDlg::updatePages() count > 1";
        additionalPageWidget->setEnabled(false);
        multipleChoicePageWidget->setEnabled(false);
        conjugationPageWidget->setEnabled(false);
        comparisonPageWidget->setEnabled(false);
        fromPageWidget->setEnabled(true);
        toPageWidget->setEnabled(true);
        return;
    }

    fromPageWidget->setEnabled(true);
    toPageWidget->setEnabled(true);

    // only one entry selected - now add entry specific pages
    additionalPageWidget->setEnabled(true);
    multipleChoicePageWidget->setEnabled(true);

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

    commonPage->commitData();

    int hasSel = m_selection.count() > 1;

    if (!hasSel) {
        KEduVocExpression *expr = m_doc->entry(m_currentRow);
        if (m_currentTranslation >= 0) {
            /// @todo emit data changed signal for the big table
kDebug() << "Changes should be committed but the table is not updated. FIXME";
            //m_tableModel->setData(m_tableModel->index(m_currentRow, 0), getLesson(), Qt::EditRole);            //m_tableModel->setData(m_tableModel->index(m_currentRow, m_currentTranslation), getExpr(), Qt::EditRole);


            additionalPage->commitData();
            conjugationPage->commitData();
            comparisonPage->commitData();
//             mc_page->commitData();


            expr->translation(m_currentTranslation).setMultipleChoice(mc_page->getMultipleChoice());

            expr->translation(m_currentTranslation).setFalseFriend(0, from_page ? from_page->getFauxAmi() : QString(""));
            expr->translation(0).setFalseFriend(m_currentTranslation, to_page->getFauxAmi());

            expr->translation(0).gradeFrom(m_currentTranslation).setGrade( from_page ? from_page->getGrade() : KV_NORM_GRADE );
            expr->translation(m_currentTranslation).gradeFrom(0).setGrade( to_page ? to_page->getGrade() : KV_NORM_GRADE );

            expr->translation(0).gradeFrom(m_currentTranslation).setQueryCount(from_page ? from_page->getQCount() : 0);
            expr->translation(m_currentTranslation).gradeFrom(0).setQueryCount(to_page ? to_page->getQCount() : 0);

            expr->translation(0).gradeFrom(m_currentTranslation).setBadCount(from_page ? from_page->getBCount() : 0);
            expr->translation(m_currentTranslation).gradeFrom(0).setBadCount(to_page ? to_page->getBCount() : 0);

            expr->translation(m_currentTranslation).gradeFrom(0).setQueryDate( from_page ? from_page->getDate() : QDateTime() );
            expr->translation(0).gradeFrom(m_currentTranslation).setQueryDate( to_page ? to_page->getDate() : QDateTime() );
        }

    } else {
        foreach(QModelIndex selIndex, m_selection) {
            //QModelIndex index = m_sortFilterModel->mapToSource(selIndex);
            KEduVocExpression *expr = m_doc->entry(m_currentRow);

            if (m_currentTranslation >= 0) {
                // only updated "common" props in multimode
                // is the modified necessary? maybe because it can be different and will only be saved if the user changes it. otherwise it should stay different probably. so maybe leave the modified stuff in here.
                if (from_page->gradeIsModified())
                    expr->translation(m_currentTranslation).gradeFrom(0).setGrade(from_page->getGrade());
                if (to_page->gradeIsModified())
                    expr->translation(m_currentTranslation).gradeFrom(0).setGrade(to_page->getGrade());
                if (from_page->queryCountIsModified())
                    expr->translation(m_currentTranslation).gradeFrom(0).setQueryCount(from_page->getQCount());
                if (to_page->queryCountIsModified())
                    expr->translation(m_currentTranslation).gradeFrom(0).setQueryCount(to_page->getQCount());
                if (from_page->badCountIsModified())
                    expr->translation(m_currentTranslation).gradeFrom(0).setBadCount(from_page->getBCount());
                if (to_page->badCountIsModified())
                    expr->translation(m_currentTranslation).gradeFrom(0).setBadCount(to_page->getBCount());
                if (from_page->dateIsModified())
                    expr->translation(m_currentTranslation).gradeFrom(0).setQueryDate(from_page->getDate());
                if (to_page->dateIsModified())
                    expr->translation(0).gradeFrom(m_currentTranslation).setQueryDate( to_page->getDate());
            }
        }
    }

    setModified(false);
    m_doc->setModified(true);
}


#include "EntryDlg.moc"
