/***************************************************************************

                         i/o part of kvoctrain

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
#include "newdocument-wizard/kvtnewdocumentwizard.h"
#include "entry-dialogs/EntryDlg.h"
#include "prefs.h"

#include "kvttablemodel.h"
#include "kvtsortfiltermodel.h"
#include "kvttableview.h"
#include "lessondockwidget.h"

#include <KFileDialog>
#include <KStatusBar>
#include <KRecentFilesAction>
#include <KStandardDirs>
#include <knewstuff2/engine.h>
#include <KUser>
#include <KMessageBox>

#include <QTimer>
#include <QFrame>
#include <QWizard>
#include <QApplication>
#include <QProgressBar>
#include <QHeaderView>

void ParleyApp::slotTimeOutBackup()
{
    if (Prefs::autoBackup() && m_document->document()->isModified()) {
//         slotStatusMsg(i18n("Autobackup in progress"));
        m_document->save();
    }

    if (Prefs::autoBackup())
        QTimer::singleShot(Prefs::backupTime() * 60 * 1000, this, SLOT(slotTimeOutBackup()));
//     slotStatusMsg(IDS_DEFAULT);
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


void ParleyApp::setDocument(KEduVocDocument * doc)
{
    m_lessonDockWidget->setDocument(m_document->document());

    m_tableModel->setDocument(m_document->document());
    m_tableModel->reset();
    m_tableModel->loadLanguageSettings();


    kDebug() << "setDocument()" << doc->url() << doc->title() << doc->entryCount() << m_tableModel->rowCount(QModelIndex()) << m_tableModel->columnCount(QModelIndex());

    connect(m_document->document(), SIGNAL(docModified(bool)), this, SLOT(slotUpdateWindowCaption()));

    int currentColumn = Prefs::currentCol();
    int currentRow = Prefs::currentRow();
    if (currentColumn <= KV_COL_LESS) {
        currentColumn = KV_COL_TRANS;
    }
    // always operate from m_sortFilterModel
    m_tableView->setCurrentIndex(m_sortFilterModel->mapFromSource(m_tableModel->index(currentRow, currentColumn)));

    setCaption(m_document->document()->url().fileName(), false);

//     m_tableView->adjustContent();
    m_tableView->setColumnHidden(KV_COL_LESS, !Prefs::tableLessonColumnVisible());
    m_tableView->setColumnHidden(KV_COL_MARK, !Prefs::tableActiveColumnVisible());

    slotCurrentChanged(m_tableView->currentIndex(), m_tableView->currentIndex());

    // Filter proxy
    m_tableView->setColumnWidth(0, qvariant_cast<QSize>(m_tableModel->headerData(0, Qt::Horizontal, Qt::SizeHintRole)).width());
    m_tableView->setColumnWidth(1, qvariant_cast<QSize>(m_tableModel->headerData(1, Qt::Horizontal, Qt::SizeHintRole)).width());
    m_tableView->setColumnWidth(2, qvariant_cast<QSize>(m_tableModel->headerData(2, Qt::Horizontal, Qt::SizeHintRole)).width());
    m_tableView->setColumnWidth(3, qvariant_cast<QSize>(m_tableModel->headerData(2, Qt::Horizontal, Qt::SizeHintRole)).width());
    m_tableView->horizontalHeader()->setResizeMode(KV_COL_MARK, QHeaderView::Fixed);
}



