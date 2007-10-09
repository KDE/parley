/***************************************************************************

                      main part of kvoctrain

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>
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

#include "parley.h"

#include "practice/practicemanager.h"
#include "kvttablemodel.h"
#include "kvtsortfiltermodel.h"
#include "kvttableview.h"
#include "kvtlessonmodel.h"
#include "kvtlessonview.h"

#include "entry-dialogs/EntryDlg.h"
#include "statistics-dialogs/StatisticsDialog.h"
#include "settings/kvoctrainprefs.h"
#include "language-dialogs/languagedialog.h"
#include "language-dialogs/grammardialog.h"
#include "docprop-dialogs/TitlePage.h"
#include "configure-practice/configurepracticedialog.h"
#include "prefs.h"

#include <keduvoclesson.h>

#include <kstatusbar.h>
#include <klineedit.h>
#include <kcombobox.h>
#include <kconfig.h>
#include <kselectaction.h>
#include <kstandarddirs.h>
#include <klocale.h>
#include <kdebug.h>
#include <kiconloader.h>
#include <kprinter.h>
#include <krecentfilesaction.h>
#include <kinputdialog.h>
#include <kapplication.h>
#include <KActionCollection>
#include <KMessageBox>

#include <QFile>
#include <QTimer>
#include <QPixmap>
#include <QKeyEvent>
#include <QApplication>
#include <QClipboard>
#include <QProgressBar>
#include <QSplitter>

// #define MAX_LESSON       25
// #define THRESH_LESSON    KV_MIN_GRADE


void ParleyApp::saveOptions()
{
    fileOpenRecent->saveEntries(KGlobal::config()->group("Recent Files"));

    if (m_tableView) {
        // map to original entry numbers:
        QModelIndex sourceIndex = m_sortFilterModel->mapToSource(m_tableView->currentIndex());
        Prefs::setCurrentRow(sourceIndex.row());
        Prefs::setCurrentCol(sourceIndex.column());
    }

    if (m_lessonSelectionCombo)
        Prefs::setLessonEditingSelection(m_lessonSelectionCombo->currentIndex());

    if (m_mainSplitter)
        Prefs::setMainWindowSplitter(m_mainSplitter->sizes());

    Prefs::self()->writeConfig();
}


void ParleyApp::saveProperties(KConfigGroup &config)
{
    saveOptions();
    if (m_doc) {
        config.writeEntry("Filename", m_doc->url().path());
        config.writeEntry("Title", m_doc->title());
        config.writeEntry("Modified", m_doc->isModified());

        QString filename=m_doc->url().path();
        QString tempname = kapp->tempSaveName(filename);
        m_doc->saveAs(KUrl(tempname), KEduVocDocument::Automatic, "Parley");
    }
}


void ParleyApp::readProperties(const KConfigGroup &config)
{
    QString filename = config.readEntry("Filename");
    QString title = config.readEntry("Title");
    bool modified = config.readEntry("Modified", false);
    if (modified) {
        bool b_canRecover;
        QString tempname = kapp->checkRecoverFile(filename,b_canRecover);

        if (b_canRecover) {
            m_doc = new KEduVocDocument(this);
            m_doc->setUrl(KUrl(tempname));
            removeProgressBar();
            m_doc->setModified();
            m_doc->setTitle(title);
            m_doc->setUrl(KUrl(filename));
            setCaption(m_doc->title(), m_doc->isModified());
            QFile::remove(tempname);
        }
    } else if (!filename.isEmpty()) {
        m_doc = new KEduVocDocument(this);
        m_doc->setUrl(KUrl(filename));
        removeProgressBar();
        setCaption(m_doc->title(), m_doc->isModified());
    }

    show();
}


ParleyApp::~ParleyApp()
{
    if ( m_practiceManager ) {
        m_practiceManager->deleteLater();
    }
    removeEntryDlg();
    delete m_doc;
}


void ParleyApp::slotCancelSelection()
{
    m_tableView->clearSelection();
}


void ParleyApp::slotSelectAll()
{
    m_tableView->selectAll();
}


/**
 * This slotEditEntry() is called by the button in the toolbar.
 * Uses the current selection.
 */
void ParleyApp::slotEditEntry()
{
    // prepare the entry dialog
    if (entryDlg == 0) {
        entryDlg = new EntryDlg(this, m_doc);
        connect(entryDlg, SIGNAL(closeEntryDialog()), this, SLOT(removeEntryDlg()));
        connect(entryDlg, SIGNAL(dataChanged(const QModelIndex& , const QModelIndex&)), m_tableModel, SLOT(dataChangedFromOutside(const QModelIndex& , const QModelIndex&)));
    }

    if (entryDlg != 0) {
        entryDlg->commitData(false);
    }
    entryDlg->show();

    if (entryDlg == 0) {
        kError() << "ParleyApp::setDataEntryDlg: entryDlg == 0\n";
        return;
    }

    // set the data in the dialog

    // to pass on the selection we need to translate from m_sortFilterModel to the real model
    QModelIndexList modelIndexList;
    modelIndexList = m_tableView->selectionModel()->selectedRows();

    QList<int> entryList;
    foreach (QModelIndex modelIndex, modelIndexList) {
        entryList.append(m_sortFilterModel->mapToSource(modelIndex).row());
    }

    QModelIndex currentIndex = m_tableView->currentIndex();
    if (currentIndex.isValid()) {
        currentIndex = m_sortFilterModel->mapToSource(currentIndex);
    }

    if (entryList.isEmpty() ) {
        entryList.append( currentIndex.row() );
    }

    entryDlg->setData(entryList, currentIndex.column() - KV_COL_TRANS);
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}


void ParleyApp::removeEntryDlg()
{
    if (entryDlg != 0) {
        entryDlg->deleteLater();
        entryDlg = 0;
    }

    m_tableView->setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked);
}


void ParleyApp::slotLanguageProperties()
{
    GrammarDialog ddlg(m_doc, this);

    if (ddlg.exec() == KDialog::Accepted) {
        m_doc->setModified();
        m_tableModel->reset();
        setCaption(m_doc->title(), m_doc->isModified());
//         slotStatusMsg(IDS_DEFAULT);
    }
}


void ParleyApp::slotModifiedDoc(bool /*mod*/)
{
    setCaption(m_doc->title(), m_doc->isModified());
//     slotStatusMsg(IDS_DEFAULT);
}


void ParleyApp::slotCutEntry()
{
    // there's no need to reinvent the wheel ;)
    slotEditCopy();

    // but we won't ask the user whether to delete or not.. we'll just cut
    if (m_tableView->selectionModel()->selectedRows().count() == 1) {
        int currentRow = m_tableView->currentIndex().row();
        int currentColumn = m_tableView->currentIndex().column();
        m_sortFilterModel->removeRows(m_tableView->currentIndex().row(), 1, QModelIndex());
        m_tableView->selectionModel()->setCurrentIndex(m_sortFilterModel->index(currentRow, currentColumn), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    } else {
        int currentRow = m_tableView->currentIndex().row();
        int currentColumn = m_tableView->currentIndex().column();
        int rowCount = m_sortFilterModel->rowCount(QModelIndex());
        // Must count backwards otherwise entry-numbering goes wrong when
        // deleting.
        for (int i = rowCount - 1; i >= 0; i--)
            if (m_tableView->selectionModel()->isRowSelected(i, QModelIndex()))
                m_sortFilterModel->removeRows(i, 1, QModelIndex());
        m_tableView->selectionModel()->setCurrentIndex(m_sortFilterModel->index(currentRow, currentColumn), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    }
    editDelete->setEnabled(m_sortFilterModel->rowCount(QModelIndex()) > 0);
}


void ParleyApp::slotDeleteEntry()
{
    if (m_tableView->selectionModel()->selectedRows().count() == 1) {
        if (KMessageBox::Continue == KMessageBox::warningContinueCancel(this, i18n("Do you really want to delete the selected entry?"), "", KStandardGuiItem::del())) {
            int currentRow = m_tableView->currentIndex().row();
            int currentColumn = m_tableView->currentIndex().column();
            m_sortFilterModel->removeRows(m_tableView->currentIndex().row(), 1, QModelIndex());
            m_tableView->selectionModel()->setCurrentIndex(m_sortFilterModel->index(currentRow, currentColumn), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        }
    } else {
        if (KMessageBox::Continue == KMessageBox::warningContinueCancel(this, i18n("Do you really want to delete the selected entries?"), "", KStandardGuiItem::del())) {
            int currentRow = m_tableView->currentIndex().row();
            int currentColumn = m_tableView->currentIndex().column();
            int rowCount = m_sortFilterModel->rowCount(QModelIndex());
            // Must count backwards otherwise entry-numbering goes wrong when
            // deleting.
            for (int i = rowCount - 1; i >= 0; i--)
                if (m_tableView->selectionModel()->isRowSelected(i, QModelIndex()))
                    m_sortFilterModel->removeRows(i, 1, QModelIndex());
            m_tableView->selectionModel()->setCurrentIndex(m_sortFilterModel->index(currentRow, currentColumn), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        }
    }
    editDelete->setEnabled(m_sortFilterModel->rowCount(QModelIndex()) > 0);
}


void ParleyApp::slotNewEntry()
{
    m_tableModel->appendEntry();

    // show the new entry
    makeLessonVisibleInTable(m_doc->currentLesson());

    // the delete action should be enabled if we have >0 entries in the big table (should be the case now)
    editDelete->setEnabled(m_sortFilterModel->rowCount(QModelIndex()) > 0);

    m_tableView->selectionModel()->setCurrentIndex(
        m_sortFilterModel->mapFromSource(m_tableModel->index( m_sortFilterModel->rowCount( QModelIndex() ) - 1, KV_COL_TRANS)),
       QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}


void ParleyApp::makeLessonVisibleInTable(int lessonIndex)
{
    switch (m_lessonSelectionCombo->currentIndex()) {
    case Prefs::EnumLessonEditingSelection::CurrentLesson:
        m_doc->setCurrentLesson(lessonIndex);
        m_sortFilterModel->clear();
        break;
    case Prefs::EnumLessonEditingSelection::LessonsInQuery:
        m_doc->setCurrentLesson(lessonIndex);
        if ( !m_doc->lesson(lessonIndex).inQuery() ) {
            m_lessonSelectionCombo->setCurrentIndex(Prefs::EnumLessonEditingSelection::CurrentLesson);
        }
        m_sortFilterModel->clear();
        break;
    case Prefs::EnumLessonEditingSelection::AllLessons:
        break;
    }
}

void ParleyApp::keyPressEvent(QKeyEvent *e)
{
    bool controlActive = (e->modifiers() & Qt::ControlModifier) !=0;

    switch (e->key()) {
        /*
        case Qt::Key_Plus:
          if (controlActive) {
            int less = m_lessonsComboBox->currentIndex();
            if (less == m_lessonsComboBox->count() - 1)
              m_lessonsComboBox->setCurrentIndex(0);
            else
              m_lessonsComboBox->setCurrentIndex(less+1);
            slotChooseLesson(m_lessonsComboBox->currentIndex());
          }
        break;

        case Qt::Key_Minus:
          if (controlActive) {
            int less = m_lessonsComboBox->currentIndex();
            if (less == 0)
              m_lessonsComboBox->setCurrentIndex(m_lessonsComboBox->count() - 1);
            else
              m_lessonsComboBox->setCurrentIndex(less - 1);
            slotChooseLesson(m_lessonsComboBox->currentIndex());
          }
        break;
        */
    case Qt::Key_Control:
        controlActive = true;
        break;

    case Qt::Key_Tab:
        if (m_tableView->hasFocus())  {
            m_searchLine->setFocus();
            m_searchLine->selectAll();
        } else
            m_tableView->setFocus();
        break;

    case Qt::Key_Backtab:
        if (m_searchLine->hasFocus())
            m_tableView->setFocus();
        else {
            m_searchLine->setFocus();
            m_searchLine->selectAll();
        }
        break;

    default:
        bool found = false;
        if (!found)
            e->ignore();
    }
//     slotStatusMsg(IDS_DEFAULT);
}


void ParleyApp::slotShowStatistics()
{
    KVTStatisticsDialog sdlg(m_tableModel, this);
    sdlg.exec();
}


void ParleyApp::slotCleanVocabulary()
{
    prepareProgressBar();
    QApplication::setOverrideCursor(Qt::WaitCursor);
    int num = m_doc->cleanUp();
    QApplication::restoreOverrideCursor();
    removeProgressBar();

//     slotStatusMsg(IDS_DEFAULT);

    if (num != 0) {
        m_tableModel->reset();
        QString s = i18np("1 entry with the same content was found and removed.",
                          "%1 entries with the same content were found and removed.", num);

        KMessageBox::information(this, s, i18n("Remove Duplicates"));
    }
}


void ParleyApp::slotGeneralOptions()
{
    KVocTrainPrefs* dialog = new KVocTrainPrefs(m_doc, this, "settings",  Prefs::self());
    connect(dialog, SIGNAL(settingsChanged(const QString &)), this, SLOT(slotApplyPreferences()));
    dialog->show();
}


void ParleyApp::slotApplyPreferences()
{
    if (Prefs::autoBackup()) {
        QTimer::singleShot(Prefs::backupTime() * 60 * 1000, this, SLOT(slotTimeOutBackup()));
    }

    if (m_pronunciationStatusBarLabel) {
        m_pronunciationStatusBarLabel->setFont(Prefs::iPAFont());
    }

    m_tableView->setFont(Prefs::tableFont());
    m_tableView->reset();

    m_tableModel->reset();
}


// void ParleyApp::slotStatusHelpMsg(const QString &text)
// {
//     ///////////////////////////////////////////////////////////////////
//     // change status message of whole statusbar temporary (text, msec)
//     if (pbar == 0 || !pbar->isVisible()) {
//         statusBar()->showMessage(text, 3000);
//     }
// }

void ParleyApp::slotFilePrint()
{
    KPrinter printer;
    printer.setFullPage(true);
    if (printer.setup(this)) {
        m_tableView->print(&printer);
    }
}


void ParleyApp::slotEditCopy()
{
//     slotStatusMsg(i18n("Copying selection to clipboard..."));

    QApplication::setOverrideCursor(Qt::WaitCursor);

    QString textToCopy;
    QModelIndexList selectedRows = m_tableView->selectionModel()->selectedRows(0);

    foreach(const QModelIndex &idx, selectedRows) {
        bool sep = false;
        for (int i = KV_COL_TRANS; i < m_tableModel->columnCount(QModelIndex()); i++) {
            if (!sep)
                sep = true;
            else
                textToCopy += '\t';

            QModelIndex mappedIndex = m_sortFilterModel->mapToSource(m_sortFilterModel->index(idx.row(), i));
            textToCopy += m_tableModel->data(mappedIndex, Qt::DisplayRole).toString();
        }
        if (!textToCopy.isEmpty())
            textToCopy += '\n';
    }

    if (!textToCopy.isEmpty())
        QApplication::clipboard()->setText(textToCopy);

    QApplication::restoreOverrideCursor();
//     slotStatusMsg(IDS_DEFAULT);
}


void ParleyApp::slotEditPaste()
{
    /// @todo make the pasted stuff visible by making the corresponding lesson visible, if it is not (?)
//     slotStatusMsg(i18n("Inserting clipboard contents..."));

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QString s;
    QString textToPaste = QApplication::clipboard()->text();

    QTextStream ts;
    ts.setString(&textToPaste, QIODevice::Text);

    QString num;

    QModelIndexList selectedRows = m_tableView->selectionModel()->selectedRows();
    int lastSelectedRow;
    if(!selectedRows.isEmpty())
        lastSelectedRow = m_sortFilterModel->mapToSource(selectedRows.back()).row() + 1;
    else
        lastSelectedRow = m_tableModel->rowCount(QModelIndex());

    int count = 0;
    while (!ts.atEnd()) {
        s = ts.readLine();
        if (!s.isEmpty()) {
            m_tableModel->insertRows(lastSelectedRow + count, 1);
            QStringList sl = s.split('\t', QString::KeepEmptyParts);

            for (int i = 0; i < sl.count(); ++i) {
                m_tableModel->setData(m_tableModel->index(lastSelectedRow + count, i + KV_COL_TRANS), sl[i], Qt::EditRole);
                m_tableModel->setData(m_tableModel->index(lastSelectedRow + count, i + KV_COL_TRANS), m_doc->currentLesson(), KVTTableModel::LessonRole);
            }
        }
        count++;
    }

    QApplication::restoreOverrideCursor();
//     slotStatusMsg(IDS_DEFAULT);

    editDelete->setEnabled(m_sortFilterModel->rowCount(QModelIndex()) > 0);
}


void ParleyApp::slotSelectionChanged(const QItemSelection &, const QItemSelection &)
{
    // update the entry dialog if it is there
    if (entryDlg != 0) {
        slotEditEntry();
    }
}


void ParleyApp::slotCurrentChanged(const QModelIndex & current, const QModelIndex & previous)
{
    // update the status bar
    Q_UNUSED(previous);
    if (!current.isValid()) {
        return;
    }

    int translationId = current.column() - KV_COL_TRANS;
    if ( translationId < 0 ) {
        translationId = 0;
    }

    QModelIndex index = current;
    index = m_sortFilterModel->mapToSource(current);

    //KEduVocExpression * currentExpression = current.data(KVTTableModel::ExpressionRole).value<KEduVocExpression*>();
    KEduVocExpression * currentExpression = m_doc->entry(index.row());

    if (m_remarkStatusBarLabel != 0) {
        m_remarkStatusBarLabel->setText(i18n("Comment: %1", currentExpression->translation(translationId).comment()));
    }
    if (m_pronunciationStatusBarLabel != 0) {
        m_pronunciationStatusBarLabel->setText(i18n("Pronunciation: %1", currentExpression->translation(translationId).pronunciation()));
    }
    QString typeText = currentExpression->translation(translationId).type();
    if ( !currentExpression->translation(translationId).subType().isEmpty() ){
        typeText.append(i18n(" (%1)", currentExpression->translation(translationId).subType()));
    }
    if (m_typeStatusBarLabel != 0) {
        m_typeStatusBarLabel->setText(i18n("Type: %1", typeText));
    }
    // update the entry dialog if it is there
    if (entryDlg != 0) {
        slotEditEntry();
    }
}


void ParleyApp::slotCurrentLessonChanged()
{
    editDelete->setEnabled(m_sortFilterModel->rowCount(QModelIndex()) > 0);
}

void ParleyApp::slotConfigShowSearch()
{
    if (m_searchWidget) {
        m_searchWidget->setVisible(m_searchWidget->isHidden());
        Prefs::setShowSearch(m_searchWidget->isVisible());
    }
}

void ParleyApp::slotEditLanguages()
{
    LanguageDialog* languageDialog = new LanguageDialog(m_doc, this);
    if ( languageDialog->exec() == KDialog::Accepted ) {
        m_tableModel->reset();
    }
}

void ParleyApp::slotDocumentProperties()
{
    TitlePage* titleAuthorWidget = new TitlePage(m_doc, this);
    KDialog* titleAuthorDialog;
    titleAuthorDialog = new KDialog(this);
    titleAuthorDialog->setMainWidget( titleAuthorWidget );
    titleAuthorDialog->setCaption(i18nc("@title:window document properties", "Properties for %1", m_doc->url().url()));
    if ( titleAuthorDialog->exec() == KDialog::Accepted ) {
        titleAuthorWidget->commitData();
    }
    delete titleAuthorDialog;
}

void ParleyApp::configurePractice()
{
    ConfigurePracticeDialog* configurePracticeDialog;
    configurePracticeDialog = new ConfigurePracticeDialog(m_doc, this, "practice settings",  Prefs::self());

    configurePracticeDialog->show();
}

void ParleyApp::startPractice()
{
    if ( m_practiceManager ) {
        m_practiceManager->deleteLater();
    }
    m_practiceManager = new PracticeManager(this, m_doc);
    m_practiceManager->startPractice();
}


#include "parley.moc"
