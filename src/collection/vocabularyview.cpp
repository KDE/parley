/***************************************************************************

    Copyright 2006, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
    Copyright 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "vocabularyview.h"
#include "vocabularyheaderview.h"

#include "vocabularymodel.h"
#include "vocabularyfilter.h"
#include "vocabularydelegate.h"
#include "vocabularymimedata.h"

#include "editor/editor.h"
#include "prefs.h"
#include "vocabularycolumnsdialog.h"
#include "documentsettings.h"

#include <keduvoctranslation.h>
#include <keduvocexpression.h>

#include <QHeaderView>
#include <QPainter>
#include <QResizeEvent>
#include <QClipboard>
#include <QTimer>

#include <KApplication>
#include <KNotification>
#include <KComponentData>
#include <KGlobalSettings>
#include <KAction>
#include <KActionCollection>
#include <KToggleAction>
#include <KLocale>
#include <KMessageBox>
#include <KUrl>
#include <sonnet/backgroundchecker.h>
#include <languagesettings.h>

using namespace Editor;

VocabularyView::VocabularyView(EditorWindow * parent)
    : QTableView(parent), m_model(0), m_doc(0),
      m_spellChecker(0), m_spellDialog(0)
{
    installEventFilter(this);

    setHorizontalHeader(new VocabularyHeaderView(Qt::Horizontal, this));

    horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked);

    setSortingEnabled(true);
    setTabKeyNavigation(true);

    m_vocabularyDelegate = new VocabularyDelegate(this);
    setItemDelegate(m_vocabularyDelegate);

    setFrameStyle(QFrame::NoFrame);
    setAlternatingRowColors(true);

    // Enable context menus
    setContextMenuPolicy(Qt::ActionsContextMenu);
    horizontalHeader()->setContextMenuPolicy(Qt::ActionsContextMenu);

    setWordWrap(true);
    setDragEnabled(true);

    // smooth scrolling horizontally, otherwise it tries to jump from item to item.
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    m_appendEntryAction = new KAction(this);
    parent->actionCollection()->addAction("edit_append", m_appendEntryAction);
    m_appendEntryAction->setIcon(KIcon("list-add-card"));
    m_appendEntryAction->setText(i18n("&Add New Entry"));
    connect(m_appendEntryAction, SIGNAL(triggered(bool)), SLOT(appendEntry()));
    m_appendEntryAction->setShortcut(QKeySequence(Qt::Key_Insert));
    m_appendEntryAction->setWhatsThis(i18n("Append a new row to the vocabulary"));
    m_appendEntryAction->setToolTip(m_appendEntryAction->whatsThis());
    m_appendEntryAction->setStatusTip(m_appendEntryAction->whatsThis());
    addAction(m_appendEntryAction);

    m_deleteEntriesAction = new KAction(this);
    parent->actionCollection()->addAction("edit_remove_selected_area", m_deleteEntriesAction);
    m_deleteEntriesAction->setIcon(KIcon("list-remove-card"));
    m_deleteEntriesAction->setText(i18n("&Delete Entry"));
    connect(m_deleteEntriesAction, SIGNAL(triggered(bool)), this, SLOT(deleteSelectedEntries()));
    m_deleteEntriesAction->setShortcut(QKeySequence(Qt::Key_Delete));
    m_deleteEntriesAction->setWhatsThis(i18n("Delete the selected rows"));
    m_deleteEntriesAction->setToolTip(m_deleteEntriesAction->whatsThis());
    m_deleteEntriesAction->setStatusTip(m_deleteEntriesAction->whatsThis());
    addAction(m_deleteEntriesAction);

    QAction* separator = new QAction(this);
    separator->setSeparator(true);
    addAction(separator);

    m_copyAction = KStandardAction::copy(this, SLOT(slotEditCopy()), parent->actionCollection());
    m_copyAction->setWhatsThis(i18n("Copy"));
    m_copyAction->setToolTip(m_copyAction->whatsThis());
    m_copyAction->setStatusTip(m_copyAction->whatsThis());
    addAction(m_copyAction);

    m_cutAction = KStandardAction::cut(this, SLOT(slotCutEntry()), parent->actionCollection());
    m_cutAction->setWhatsThis(i18n("Cut"));
    m_cutAction->setToolTip(m_cutAction->whatsThis());
    m_cutAction->setStatusTip(m_cutAction->whatsThis());
    addAction(m_cutAction);

    m_pasteAction = KStandardAction::paste(this, SLOT(slotEditPaste()), parent->actionCollection());
    m_pasteAction->setWhatsThis(i18n("Paste"));
    m_pasteAction->setToolTip(m_pasteAction->whatsThis());
    m_pasteAction->setStatusTip(m_pasteAction->whatsThis());
    addAction(m_pasteAction);

    m_selectAllAction = KStandardAction::selectAll(this, SLOT(selectAll()), parent->actionCollection());
    m_selectAllAction->setWhatsThis(i18n("Select all rows"));
    m_selectAllAction->setToolTip(m_selectAllAction->whatsThis());
    m_selectAllAction->setStatusTip(m_selectAllAction->whatsThis());

    m_clearSelectionAction = KStandardAction::deselect(this, SLOT(clearSelection()), parent->actionCollection());
    m_clearSelectionAction->setWhatsThis(i18n("Deselect all rows"));
    m_clearSelectionAction->setToolTip(m_clearSelectionAction->whatsThis());
    m_clearSelectionAction->setStatusTip(m_clearSelectionAction->whatsThis());

    // vocabulary columns dialog
    KAction *vocabularyColumnsDialogAction = new KAction(this);
    parent->actionCollection()->addAction("show_vocabulary_columns_dialog", vocabularyColumnsDialogAction);
    vocabularyColumnsDialogAction->setIcon(KIcon("view-file-columns"));
    vocabularyColumnsDialogAction->setText(i18n("Vocabulary Columns..."));
    vocabularyColumnsDialogAction->setWhatsThis(i18n("Toggle display of individual vocabulary columns"));
    vocabularyColumnsDialogAction->setToolTip(vocabularyColumnsDialogAction->whatsThis());
    vocabularyColumnsDialogAction->setStatusTip(vocabularyColumnsDialogAction->whatsThis());
    horizontalHeader()->addAction(vocabularyColumnsDialogAction);
    addAction(vocabularyColumnsDialogAction);
    connect(vocabularyColumnsDialogAction, SIGNAL(triggered(bool)), this, SLOT(slotShowVocabularyColumnsDialog()));
}

void VocabularyView::setModel(VocabularyFilter * model)
{
    QTableView::setModel(model);
    m_model = model;
    connect(selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            SLOT(slotCurrentChanged(const QModelIndex &, const QModelIndex &)));
    connect(selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
            SLOT(slotSelectionChanged(const QItemSelection&, const QItemSelection&)));
    slotSelectionChanged(QItemSelection(), QItemSelection());
}

void VocabularyView::slotCurrentChanged(const QModelIndex & current, const QModelIndex & previous)
{
    Q_UNUSED(previous);
    KEduVocExpression* entry = 0;
    if (current.isValid()) {
        entry =  model()->data(current, VocabularyModel::EntryRole).value<KEduVocExpression*>();
    }
    emit translationChanged(entry, VocabularyModel::translation(current.column()));
}

void VocabularyView::reset()
{
    QTableView::reset();
    emit translationChanged(0, 0);

    QList<int> visibleColumns;
    if (m_doc) {
        DocumentSettings ds(m_doc->url().url());
        ds.readConfig();
        visibleColumns = ds.visibleColumns();

        KConfig parleyConfig("parleyrc");
        KConfigGroup documentGroup(&parleyConfig, "Document " + m_doc->url().url());
        QByteArray state = documentGroup.readEntry("VocabularyColumns", QByteArray());

        if (!horizontalHeader()->restoreState(state)) {
            resizeColumnsToContents();
        }
    }

    for (int i = 0; i < model()->columnCount(QModelIndex()); i++) {
        if (i < visibleColumns.size()) {
            setColumnHidden(i, !visibleColumns.value(i));
        } else {
            if (VocabularyModel::columnType(i) != VocabularyModel::Translation) {
                setColumnHidden(i, true);
            }
        }
    }
}

void VocabularyView::saveColumnVisibility() const
{
    if (!m_doc) {
        return;
    }

    // Generate a QList<int> for saving
    QList<int> visibleList;
    for (int i = 0; i < m_model->columnCount(); ++i) {
        visibleList.append(static_cast<int>(!isColumnHidden(i)));
    }

    DocumentSettings ds(m_doc->url().url());
    ds.setVisibleColumns(visibleList);
    ds.writeConfig();

    QByteArray saveState = horizontalHeader()->saveState();
    KConfig parleyConfig("parleyrc");
    KConfigGroup documentGroup(&parleyConfig, "Document " + m_doc->url().url());
    documentGroup.writeEntry("VocabularyColumns", horizontalHeader()->saveState());
}

void VocabularyView::appendEntry()
{
    QModelIndex newIndex = m_model->appendEntry();
    scrollTo(newIndex);
    selectionModel()->clear(); // otherwise proxy mapping gets screwed for some reason
    selectionModel()->select(newIndex, QItemSelectionModel::ClearAndSelect);
    selectionModel()->setCurrentIndex(newIndex, QItemSelectionModel::ClearAndSelect);
    edit(newIndex);
}

void VocabularyView::appendChar(const QChar &c)
{
    const QModelIndex &index = selectionModel()->currentIndex();
    m_model->setData(index, QString(m_model->data(index).toString() + c));
}

void VocabularyView::deleteSelectedEntries(bool askConfirmation)
{
    QSet<int> rows;
    foreach(const QModelIndex & index, selectionModel()->selectedIndexes()) {
        rows.insert(index.row());
    }

    bool del = true;
    if (askConfirmation) {
        del = KMessageBox::Continue == KMessageBox::warningContinueCancel(this, i18np("Do you really want to delete the selected entry?", "Do you really want to delete the selected %1 entries?", rows.count()), i18n("Delete"), KStandardGuiItem::del());
    }

    if (del) {
        emit translationChanged(0, 0);
        while (!selectionModel()->selectedIndexes().isEmpty()) {
            m_model->removeRows(selectionModel()->selectedIndexes()[0].row(), 1, QModelIndex());
        }
    }
}

void VocabularyView::slotEditCopy()
{
    QModelIndexList sortedIndexes = selectionModel()->selectedIndexes();
    qSort(sortedIndexes);
    QMimeData *mimeData = m_model->mimeData(sortedIndexes);

    QClipboard *clipboard = KApplication::clipboard();
    clipboard->setMimeData(mimeData);
}

void VocabularyView::slotEditPaste()
{
    QClipboard *clipboard = KApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    const VocabularyMimeData *vocMimeData = qobject_cast<const VocabularyMimeData *>(mimeData);
    if (vocMimeData) {
        kDebug() << "Clipboard contains vocabulary mime data.";
        foreach(const VocabularyMimeData::MimeExpression & mimeEntry, vocMimeData->expressionList()) {
            KEduVocExpression *pasteExpression = new KEduVocExpression(mimeEntry.expression);
            m_model->appendEntry(pasteExpression);

            // find word type (create if not found)
            KEduVocWordType *type = m_doc->wordTypeContainer();
            foreach(int translation, mimeEntry.wordTypes.keys()) {
                // append if needed
                foreach(const QString & typeName, mimeEntry.wordTypes.value(translation).wordType) {
                    kDebug() << mimeEntry.wordTypes.value(translation).wordType;
                    KEduVocContainer *childType = type->childContainer(typeName);
                    if (!childType) {
                        // the doc does not contain the right word type - create it
                        childType = new KEduVocWordType(typeName);
                        type->appendChildContainer(childType);
                    }
                    type = static_cast<KEduVocWordType*>(childType);
                }
                pasteExpression->translation(translation)->setWordType(type);
                // check for special type stuff
                if (type->wordType() != mimeEntry.wordTypes.value(translation).grammarType) {
                    if (type->wordType() == KEduVocWordFlag::NoInformation) {
                        type->setWordType(mimeEntry.wordTypes.value(translation).grammarType);
                    }
                }
            }
        }
    } else if (mimeData->hasText()) {
        kDebug() << "Clipboard contains text data.";
        // split at newline
        QStringList lines = clipboard->text().split('\n');
        foreach(QString line, lines) {
            // split at tabs or semicolon:
            m_model->appendEntry(new KEduVocExpression(line.split(QRegExp("[\t;]"), QString::KeepEmptyParts)));
        }
    }
}

void VocabularyView::slotCutEntry()
{
    slotEditCopy();
    deleteSelectedEntries(false);
}

void VocabularyView::slotSelectionChanged(const QItemSelection &, const QItemSelection &)
{
    bool hasSelection = selectionModel()->hasSelection();
    m_deleteEntriesAction->setEnabled(hasSelection);
    m_clearSelectionAction->setEnabled(hasSelection);
    m_copyAction->setEnabled(hasSelection);
    m_cutAction->setEnabled(hasSelection);
}

void VocabularyView::setDocument(KEduVocDocument * doc)
{
    m_doc = doc;
    m_vocabularyDelegate->setDocument(doc);
    QTimer::singleShot(0, this, SLOT(reset()));
}

/**
 * Set the translator to be used by the delegate
 * @param translator
 */
void VocabularyView::setTranslator(Translator* translator)
{
    m_vocabularyDelegate->setTranslator(translator);
}

void VocabularyView::slotShowVocabularyColumnsDialog()
{
    VocabularyColumnsDialog *dialog = new VocabularyColumnsDialog(m_doc, this);

    if (dialog->exec() == KDialog::Accepted) {
        reset();
    }
}

void VocabularyView::checkSpelling(int language)
{
    if (!m_model->rowCount()) {
        KMessageBox::information(this, i18n("Nothing to spell check."));
        return;
    }

    if (!m_spellChecker) {
        m_spellChecker = new Sonnet::BackgroundChecker(this);
        m_spellDialog = new Sonnet::Dialog(m_spellChecker, this);
        connect(m_spellDialog, SIGNAL(done(const QString&)), this, SLOT(continueSpelling()));
        connect(m_spellDialog, SIGNAL(misspelling(const QString&, int)), this, SLOT(misspelling(const QString&, int)));
        connect(m_spellDialog, SIGNAL(replace(const QString&, int, const QString&)), this, SLOT(spellingReplace(const QString&, int, const QString&)));
    }

    m_spellColumn = language * VocabularyModel::EntryColumnsMAX;
    m_spellRow = -1;
    if (m_spellColumn < 0) {
        return;
    }

    QModelIndex index = m_model->index(0, m_spellColumn);
    QString locale = m_doc->identifier(language).locale();
    LanguageSettings settings(locale);
    QString spellCode = settings.spellChecker().isEmpty() ? locale : settings.spellChecker();
    m_spellChecker->changeLanguage(spellCode);
    if (!m_spellChecker->speller().isValid()) {
        kDebug() << "Invalid Language, popup here!";
        KNotification::event(KNotification::Warning, i18nc("@title of a popup", "No Spell Checker Available"), i18nc("@popupmessage", "Either the language set up is incorrect or no spellchecker was installed for this locale: %1.", locale));
    }
    m_spellDialog->show();
    continueSpelling();
}

void VocabularyView::continueSpelling()
{
    kDebug() << "Check spelling: " << m_spellRow << m_spellColumn;
    ++m_spellRow;
    while (m_spellRow < m_model->rowCount()) {
        QModelIndex index = m_model->index(m_spellRow, m_spellColumn);
        kDebug() << "  " << m_model->data(index).toString();
        if (!m_model->data(index).toString().isEmpty()) {
            m_spellDialog->setBuffer(m_model->data(index).toString());
            break;
        } else {
            ++m_spellRow;
        }
    }
}

void VocabularyView::selectIndex(const QModelIndex &newIndex)
{
    selectionModel()->select(newIndex, QItemSelectionModel::ClearAndSelect);
    selectionModel()->setCurrentIndex(newIndex, QItemSelectionModel::ClearAndSelect);
    scrollTo(newIndex);
}

bool VocabularyView::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress && Prefs::smartAppend()) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            if (selectionModel()->currentIndex().row() == m_model->rowCount() - 1) {
                appendEntry();
            }
        }
    }
    // standard event processing
    return QObject::eventFilter(obj, event);
}

void VocabularyView::misspelling(const QString & word, int start)
{
    Q_UNUSED(word)
    Q_UNUSED(start)
    QModelIndex index = m_model->index(m_spellRow, m_spellColumn);
    selectIndex(index);
}

void VocabularyView::spellingReplace(const QString & oldWord, int start, const QString & newWord)
{
    kDebug() << oldWord << start << newWord;
    QModelIndex index = m_model->index(m_spellRow, m_spellColumn);
    QString data = index.data().toString();
    QString newData = data.replace(start, oldWord.length(), newWord);
    kDebug() << "Changing " << data << " to " << newData;
    m_model->setData(index, newData);
}

QModelIndexList VocabularyView::getSelectedIndexes() const
{
    return selectionModel()->selectedIndexes();
}

#include "vocabularyview.moc"