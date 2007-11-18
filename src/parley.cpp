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

#include "practice/testentrymanager.h"
//#include "kvttablemodel.h"
//#include "kvtsortfiltermodel.h"
//#include "kvttableview.h"
#include "lessondockwidget.h"
#include "vocabulary/vocabularymodel.h"
#include "vocabulary/vocabularymodel.h"

#include "entry-dialogs/EntryDlg.h"
#include "entry-dialogs/wordtypewidget.h"
#include "statistics-dialogs/StatisticsDialog.h"
#include "settings/parleyprefs.h"
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
#include <krecentfilesaction.h>
#include <kinputdialog.h>
#include <kapplication.h>
#include <KActionCollection>
#include <KMessageBox>

#include <QFile>
#include <QPixmap>
#include <QKeyEvent>
#include <QApplication>
#include <QClipboard>
#include <QProgressBar>
#include <QSplitter>
#include <QHeaderView>
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>



ParleyApp::ParleyApp(const KUrl & filename) : KXmlGuiWindow(0)
{
    m_document = new ParleyDocument(this);

//     m_tableView = 0;
//     m_tableModel = 0;
//     m_sortFilterModel = 0;
    m_searchLine = 0;
    m_searchWidget = 0;
    m_pronunciationStatusBarLabel = 0;
    m_remarkStatusBarLabel = 0;
    m_typeStatusBarLabel = 0;

    m_entryDlg = 0;

    initStatusBar();
    initActions();

    m_recentFilesAction->loadEntries(KGlobal::config()->group("Recent Files"));

    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);


    initView();
    initModel();

    initDockWidgets();

    // these connects need the model to exist
    // selection changes (the entry dialog needs these)
//     connect(m_tableView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
//             this, SLOT(slotCurrentChanged(const QModelIndex &, const QModelIndex &)));

//     connect(m_tableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
//             this, SLOT(slotSelectionChanged(const QItemSelection &, const QItemSelection &)));

//     QAction * actionRestoreNativeOrder = actionCollection()->action("restore_native_order");
//     m_tableView->horizontalHeader()->addAction(actionRestoreNativeOrder);
//     connect(actionRestoreNativeOrder, SIGNAL(triggered()), m_sortFilterModel, SLOT(restoreNativeOrder()));

//     QDockWidget *editMultipleChoiceDock = new QDockWidget(i18n("Multiple Choice"), this);
//     editMultipleChoiceDock->setObjectName("editMultipleChoiceDock");
//     addDockWidget(Qt::BottomDockWidgetArea, editMultipleChoiceDock);
//     editMultipleChoiceDock->setWidget(new MCEntryPage(m_document->document(), editMultipleChoiceDock));


//     QDockWidget *editEntryDock = new QDockWidget(i18n("General Properties"), this);
//     editEntryDock->setObjectName("EditEntryDock");
//     addDockWidget(Qt::RightDockWidgetArea, editEntryDock);
//     editEntryDock->setWidget(new CommonEntryPage(m_document->document(), editEntryDock));

//     m_deleteEntriesAction->setEnabled(m_tableModel->rowCount(QModelIndex()) > 0);

kDebug() << "Parley - will open doc";

    if ( !filename.url().isEmpty() ) {
        kDebug() << "open doc" << filename.url();
        m_document->open(filename);
        kDebug() << "open done";
    } else {
        if (m_recentFilesAction->actions().count() > 0
            && m_recentFilesAction->action(
                                        m_recentFilesAction->actions().count()-1)->isEnabled() )
        {
            m_recentFilesAction->action(m_recentFilesAction->actions().count()-1)->trigger();
        } else {
        kDebug() << "new doc";
            // this is probably the first time we start.
            m_document->newDocument();
            updateDocument();
        }
    }

    // save position of dock windows etc
    setAutoSaveSettings();
}


ParleyApp::~ParleyApp()
{
    removeEntryDlg();
}


void ParleyApp::saveOptions()
{
    m_recentFilesAction->saveEntries(KGlobal::config()->group("Recent Files"));

///@todo save selection per document
//     if (m_tableView) {
//         // map to original entry numbers:
//         QModelIndex sourceIndex = m_sortFilterModel->mapToSource(m_tableView->currentIndex());
//         Prefs::setCurrentRow(sourceIndex.row());
//         Prefs::setCurrentCol(sourceIndex.column());
//     }

    m_lessonDockWidget->saveOptions();

    Prefs::self()->writeConfig();
}


void ParleyApp::slotCancelSelection()
{
//     m_tableView->clearSelection();
}


void ParleyApp::slotSelectAll()
{
//     m_tableView->selectAll();
}


/**
 * This slotEditEntry() is called by the button in the toolbar.
 * Uses the current selection.
 */
void ParleyApp::slotEditEntry()
{
//     // prepare the entry dialog
//     if (m_entryDlg == 0) {
//         m_entryDlg = new EntryDlg(this, m_document->document());
//         connect(m_entryDlg, SIGNAL(closeEntryDialog()), this, SLOT(removeEntryDlg()));
// 
//         connect(this, SIGNAL(signalSetData(const QList<int>&, int)), m_entryDlg, SLOT(setData(const QList<int>&, int)));
//     }
// 
//     if (m_entryDlg != 0) {
//         m_entryDlg->commitData(false);
//     }
//     m_entryDlg->show();
// 
//     if (m_entryDlg == 0) {
//         kError() << "ParleyApp::setDataEntryDlg: m_entryDlg == 0\n";
//         return;
//     }
// 
//     // set the data in the dialog
//     updateEditWidgets();

//     m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}


void ParleyApp::removeEntryDlg()
{
    if (m_entryDlg != 0) {
        disconnect(m_entryDlg);
        m_entryDlg->deleteLater();
        m_entryDlg = 0;
    }

//     m_tableView->setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked);
}


void ParleyApp::slotLanguageProperties()
{
    GrammarDialog ddlg(m_document->document(), this);

    if (ddlg.exec() == KDialog::Accepted) {
        m_document->document()->setModified();
//         m_tableModel->reset();
        slotUpdateWindowCaption();
//         slotStatusMsg(IDS_DEFAULT);
    }
}


void ParleyApp::slotUpdateWindowCaption()
{
    setCaption(m_document->document()->title(), m_document->document()->isModified());
//     slotStatusMsg(IDS_DEFAULT);
}


void ParleyApp::slotCutEntry()
{
    // there's no need to reinvent the wheel ;)
    slotEditCopy();

    // but we won't ask the user whether to delete or not.. we'll just cut
//     if (m_tableView->selectionModel()->selectedRows().count() == 1) {
//         int currentRow = m_tableView->currentIndex().row();
//         int currentColumn = m_tableView->currentIndex().column();
//         m_sortFilterModel->removeRows(m_tableView->currentIndex().row(), 1, QModelIndex());
//         m_tableView->selectionModel()->setCurrentIndex(m_sortFilterModel->index(currentRow, currentColumn), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
//     } else {
//         int currentRow = m_tableView->currentIndex().row();
//         int currentColumn = m_tableView->currentIndex().column();
//         int rowCount = m_sortFilterModel->rowCount(QModelIndex());
//         // Must count backwards otherwise entry-numbering goes wrong when
//         // deleting.
//         for (int i = rowCount - 1; i >= 0; i--)
//             if (m_tableView->selectionModel()->isRowSelected(i, QModelIndex()))
//                 m_sortFilterModel->removeRows(i, 1, QModelIndex());
//         m_tableView->selectionModel()->setCurrentIndex(m_sortFilterModel->index(currentRow, currentColumn), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
//     }
//     m_deleteEntriesAction->setEnabled(m_sortFilterModel->rowCount(QModelIndex()) > 0);
}


void ParleyApp::slotDeleteEntry()
{
//     if (m_tableView->selectionModel()->selectedRows().count() == 1) {
//         if (KMessageBox::Continue == KMessageBox::warningContinueCancel(this, i18n("Do you really want to delete the selected entry?"), "", KStandardGuiItem::del())) {
//             int currentRow = m_tableView->currentIndex().row();
//             int currentColumn = m_tableView->currentIndex().column();
//             m_sortFilterModel->removeRows(m_tableView->currentIndex().row(), 1, QModelIndex());
//             m_tableView->selectionModel()->setCurrentIndex(m_sortFilterModel->index(currentRow, currentColumn), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
//         }
//     } else {
//         if (KMessageBox::Continue == KMessageBox::warningContinueCancel(this, i18n("Do you really want to delete the selected entries?"), "", KStandardGuiItem::del())) {
//             int currentRow = m_tableView->currentIndex().row();
//             int currentColumn = m_tableView->currentIndex().column();
//             int rowCount = m_sortFilterModel->rowCount(QModelIndex());
//             // Must count backwards otherwise entry-numbering goes wrong when
//             // deleting.
//             for (int i = rowCount - 1; i >= 0; i--)
//                 if (m_tableView->selectionModel()->isRowSelected(i, QModelIndex()))
//                     m_sortFilterModel->removeRows(i, 1, QModelIndex());
//             m_tableView->selectionModel()->setCurrentIndex(m_sortFilterModel->index(currentRow, currentColumn), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
//         }
//     }
//     m_deleteEntriesAction->setEnabled(m_sortFilterModel->rowCount(QModelIndex()) > 0);
}


void ParleyApp::slotNewEntry()
{
//     m_tableModel->appendEntry();

///@todo
//     m_vocabularyModel->appendEntry();


    // the delete action should be enabled if we have >0 entries in the big table (should be the case now)
//     m_deleteEntriesAction->setEnabled(m_sortFilterModel->rowCount(QModelIndex()) > 0);

//     QModelIndex currentIndex = m_sortFilterModel->index(
//             m_sortFilterModel->rowCount(QModelIndex()) - 1, KV_COL_TRANS);

//     m_tableView->selectionModel()->setCurrentIndex(
//         currentIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
// 
//     m_tableView->edit(currentIndex);
}


/*void ParleyApp::keyPressEvent(QKeyEvent *e)
{
    bool controlActive = (e->modifiers() & Qt::ControlModifier) !=0;

    switch (e->key()) {*/
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
//     case Qt::Key_Control:
//         controlActive = true;
//         break;
// 
//     case Qt::Key_Tab:
//         if (m_tableView->hasFocus())  {
//             m_searchLine->setFocus();
//             m_searchLine->selectAll();
//         } else
//             m_tableView->setFocus();
//         break;
// 
//     case Qt::Key_Backtab:
//         if (m_searchLine->hasFocus())
//             m_tableView->setFocus();
//         else {
//             m_searchLine->setFocus();
//             m_searchLine->selectAll();
//         }
//         break;
// 
//     default:
//         bool found = false;
//         if (!found)
//             e->ignore();
//     }
// //     slotStatusMsg(IDS_DEFAULT);
// }


void ParleyApp::slotShowStatistics()
{
//     KVTStatisticsDialog sdlg(m_tableModel, this);
//     sdlg.exec();
}


// void ParleyApp::slotCleanVocabulary()
// {
//     QApplication::setOverrideCursor(Qt::WaitCursor);
//     int num = m_document->document()->cleanUp();
//     QApplication::restoreOverrideCursor();
// 
//     if (num != 0) {
//         m_tableModel->reset();
//         QString s = i18np("1 entry with the same content was found and removed.",
//                           "%1 entries with the same content were found and removed.", num);
// 
//         KMessageBox::information(this, s, i18n("Remove Duplicates"));
//     }
// }


void ParleyApp::slotGeneralOptions()
{
    ParleyPrefs* dialog = new ParleyPrefs(m_document->document(), this, "settings",  Prefs::self());
    connect(dialog, SIGNAL(settingsChanged(const QString &)), this, SLOT(slotApplyPreferences()));
    dialog->show();
}


void ParleyApp::slotApplyPreferences()
{
    m_document->enableAutoBackup(Prefs::autoBackup());

    if (m_pronunciationStatusBarLabel) {
        m_pronunciationStatusBarLabel->setFont(Prefs::iPAFont());
    }

//     m_tableView->setFont(Prefs::tableFont());
//     m_tableView->reset();
// 
//     m_tableModel->reset();
}


// void ParleyApp::slotStatusHelpMsg(const QString &text)
// {
//     ///////////////////////////////////////////////////////////////////
//     // change status message of whole statusbar temporary (text, msec)
//     if (pbar == 0 || !pbar->isVisible()) {
//         statusBar()->showMessage(text, 3000);
//     }
// }



void ParleyApp::slotEditCopy()
{
//     slotStatusMsg(i18n("Copying selection to clipboard..."));

/*    QApplication::setOverrideCursor(Qt::WaitCursor);

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

    QApplication::restoreOverrideCursor();*/
//     slotStatusMsg(IDS_DEFAULT);
}


void ParleyApp::slotEditPaste()
{
    /// @todo make the pasted stuff visible by making the corresponding lesson visible, if it is not (?)
//     slotStatusMsg(i18n("Inserting clipboard contents..."));

/*    QApplication::setOverrideCursor(Qt::WaitCursor);
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
//                 m_tableModel->setData(m_tableModel->index(lastSelectedRow + count, i + KV_COL_TRANS), m_document->document()->currentLesson(), KVTTableModel::LessonRole);
                 m_tableModel->setData(m_tableModel->index(lastSelectedRow + count, i + KV_COL_TRANS), m_lessonDockWidget->selectedLesson(), KVTTableModel::LessonRole);
            }
        }
        count++;
    }

    QApplication::restoreOverrideCursor();
//     slotStatusMsg(IDS_DEFAULT);

    m_deleteEntriesAction->setEnabled(m_sortFilterModel->rowCount(QModelIndex()) > 0);*/
}


void ParleyApp::slotSelectionChanged(const QItemSelection &, const QItemSelection &)
{
    updateEditWidgets();
}


void ParleyApp::slotCurrentChanged(const QModelIndex & current, const QModelIndex & previous)
{
    // update the status bar
/*    Q_UNUSED(previous);
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
    KEduVocExpression * currentExpression = m_document->document()->entry(index.row());

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

    updateEditWidgets();*/
}


void ParleyApp::slotCurrentLessonChanged(int newLesson)
{
//     m_sortFilterModel->slotCurrentLessonChanged(newLesson);
//     m_deleteEntriesAction->setEnabled(m_sortFilterModel->rowCount(QModelIndex()) > 0);
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
    LanguageDialog* languageDialog = new LanguageDialog(m_document->document(), this);
    ///@todo
    // if this becomes a KConfigDialog: connect(languageDialog, SIGNAL(settingsChanged(const QString&)), m_tableModel, SLOT(loadLanguageSettings()));
    if ( languageDialog->exec() == KDialog::Accepted ) {
//         m_tableModel->loadLanguageSettings();
    }
}

void ParleyApp::slotDocumentProperties()
{
    TitlePage* titleAuthorWidget = new TitlePage(m_document->document(), this);
    KDialog* titleAuthorDialog;
    titleAuthorDialog = new KDialog(this);
    titleAuthorDialog->setMainWidget( titleAuthorWidget );
    titleAuthorDialog->setCaption(i18nc("@title:window document properties", "Properties for %1", m_document->document()->url().url()));
    if ( titleAuthorDialog->exec() == KDialog::Accepted ) {
        titleAuthorWidget->commitData();
    }
    slotUpdateWindowCaption();
    delete titleAuthorDialog;
}

void ParleyApp::configurePractice()
{
    ConfigurePracticeDialog* configurePracticeDialog;
    configurePracticeDialog = new ConfigurePracticeDialog(m_document->document(), this, "practice settings",  Prefs::self());

    configurePracticeDialog->show();
}

void ParleyApp::startPractice()
{
    removeEntryDlg();
    hide();

    TestEntryManager testManager(m_document->document(), this);
    testManager.startPractice();

    show();
}


bool ParleyApp::queryClose()
{
    bool erg = queryExit();
    if (erg)
        m_document->document()->setModified(false);  // avoid double query on exit via system menu
    return erg;
}


bool ParleyApp::queryExit()
{
    saveOptions();
    if (!m_document->document()->isModified()) {
        return true;
    }

    bool save = Prefs::autoSave(); //save without asking

    if (!save) {
        int exit = KMessageBox::warningYesNoCancel(this, i18n("Vocabulary is modified.\n\nSave file before exit?\n"),
                "", KStandardGuiItem::save(), KStandardGuiItem::discard());
        if (exit == KMessageBox::Yes) {
            save = true;   // save and exit
        } else if (exit == KMessageBox::No) {
            save = false;  // don't save but exit
        } else {
            return false;  // continue work
        }
    }

    if (save) {
        m_document->save();       // save and exit
    }
    return true;
}


void ParleyApp::slotFileQuit()
{
    close();
}


void ParleyApp::updateDocument()
{
    m_lessonDockWidget->setDocument(m_document->document());
//     m_wordTypeWidget->setDocument(m_document->document());

//     m_tableModel->setDocument(m_document->document());
//     m_tableModel->reset();
//     m_tableModel->loadLanguageSettings();

    connect(m_document->document(), SIGNAL(docModified(bool)), this, SLOT(slotUpdateWindowCaption()));

//     int currentColumn = Prefs::currentCol();
//     int currentRow = Prefs::currentRow();
//     if (currentColumn <= KV_COL_LESS) {
//         currentColumn = KV_COL_TRANS;
//     }
    // always operate from m_sortFilterModel
//     m_tableView->setCurrentIndex(m_sortFilterModel->mapFromSource(m_tableModel->index(currentRow, currentColumn)));

    setCaption(m_document->document()->url().fileName(), false);

//     m_tableView->adjustContent();
//     m_tableView->setColumnHidden(KV_COL_LESS, !Prefs::tableLessonColumnVisible());
//     m_tableView->setColumnHidden(KV_COL_MARK, !Prefs::tableActiveColumnVisible());

//     slotCurrentChanged(m_tableView->currentIndex(), m_tableView->currentIndex());


//     m_tableView->setColumnWidth(0, qvariant_cast<QSize>(m_tableModel->headerData(0, Qt::Horizontal, Qt::SizeHintRole)).width());
//     m_tableView->setColumnWidth(1, qvariant_cast<QSize>(m_tableModel->headerData(1, Qt::Horizontal, Qt::SizeHintRole)).width());
//     m_tableView->setColumnWidth(2, qvariant_cast<QSize>(m_tableModel->headerData(2, Qt::Horizontal, Qt::SizeHintRole)).width());
//     m_tableView->setColumnWidth(3, qvariant_cast<QSize>(m_tableModel->headerData(2, Qt::Horizontal, Qt::SizeHintRole)).width());
//     m_tableView->horizontalHeader()->setResizeMode(KV_COL_MARK, QHeaderView::Fixed);


//     for ( int i = 0; i<m_document->document()->identifierCount(); i++) {
// kDebug() << "hide " << (i*VocabularyModel::EntryColumnsMAX)+VocabularyModel::Pronunciation;
//         m_vocabularyView->setColumnHidden((i*VocabularyModel::EntryColumnsMAX)+VocabularyModel::Pronunciation, true);
//         m_vocabularyView->setColumnHidden((i*VocabularyModel::EntryColumnsMAX)+VocabularyModel::SubWordType, true);
//         m_vocabularyView->setColumnHidden((i*VocabularyModel::EntryColumnsMAX)+VocabularyModel::Antonym, true);
//         m_vocabularyView->setColumnHidden((i*VocabularyModel::EntryColumnsMAX)+VocabularyModel::Paraphrase, true);
//     }
}


void ParleyApp::updateEditWidgets()
{
    // to pass on the selection we need to translate from m_sortFilterModel to the real model
/*    QModelIndexList modelIndexList;
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

    emit signalSetData(entryList, currentIndex.column() - KV_COL_TRANS);*/
}


void ParleyApp::initDockWidgets()
{
    // word types dock
//     QDockWidget *wordTypeDockWidget = new QDockWidget(i18n("Word Type"), this);
//     wordTypeDockWidget->setObjectName("WordTypeDock");
//     m_wordTypeWidget = new WordTypeWidget(this);
//     wordTypeDockWidget->setWidget(m_wordTypeWidget);
//     addDockWidget(Qt::BottomDockWidgetArea, wordTypeDockWidget);
//     connect(this, SIGNAL(signalSetData(const QList<int>&, int)), m_wordTypeWidget, SLOT(setEntries(const QList<int>&, int)));
}


#include "parley.moc"

