/*
    SPDX-FileCopyrightText: 2006, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

///@file vocabularyview.cpp

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

#include <KEduVocTranslation>
#include <KEduVocExpression>

#include <QHeaderView>
#include <QPainter>
#include <QResizeEvent>
#include <QClipboard>
#include <QTimer>
#include <QApplication>

#include <KNotification>
#include <QAction>
#include <KActionCollection>
#include <KToggleAction>
#include <KLocalizedString>
#include <KMessageBox>
#include <QUrl>
#include <sonnet/backgroundchecker.h>
#include <languagesettings.h>

using namespace Editor;

VocabularyView::VocabularyView(EditorWindow * parent)
    : QTableView(parent), m_model(0),
      m_spellChecker(0), m_spellDialog(0)
{
    installEventFilter(this);

    setHorizontalHeader(new VocabularyHeaderView(Qt::Horizontal, this));

    horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    horizontalHeader()->setSectionsMovable(true);
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

    m_appendEntryAction = new QAction(this);
    parent->actionCollection()->addAction(QStringLiteral("edit_append"), m_appendEntryAction);
    parent->actionCollection()->setDefaultShortcut(
        m_appendEntryAction, QKeySequence(Qt::Key_Insert));
    m_appendEntryAction->setIcon(QIcon::fromTheme(QStringLiteral("list-add-card")));
    m_appendEntryAction->setText(i18n("&Add New Entry"));
    connect(m_appendEntryAction, &QAction::triggered, this, &VocabularyView::appendEntry);
    m_appendEntryAction->setShortcut(QKeySequence(Qt::Key_Insert));
    m_appendEntryAction->setWhatsThis(i18n("Append a new row to the vocabulary"));
    m_appendEntryAction->setToolTip(m_appendEntryAction->whatsThis());
    m_appendEntryAction->setStatusTip(m_appendEntryAction->whatsThis());
    addAction(m_appendEntryAction);

    m_deleteEntriesAction = new QAction(this);
    parent->actionCollection()->addAction(QStringLiteral("edit_remove_selected_area"), m_deleteEntriesAction);
    parent->actionCollection()->setDefaultShortcut(
        m_deleteEntriesAction, QKeySequence::Delete);
    m_deleteEntriesAction->setIcon(QIcon::fromTheme(QStringLiteral("list-remove-card")));
    m_deleteEntriesAction->setText(i18n("&Delete Entry"));
    connect(m_deleteEntriesAction, &QAction::triggered, this, &VocabularyView::deleteSelectedEntries);
    m_deleteEntriesAction->setShortcut(QKeySequence::Delete);
    m_deleteEntriesAction->setWhatsThis(i18n("Delete the selected rows"));
    m_deleteEntriesAction->setToolTip(m_deleteEntriesAction->whatsThis());
    m_deleteEntriesAction->setStatusTip(m_deleteEntriesAction->whatsThis());
    addAction(m_deleteEntriesAction);

    QAction* separator = new QAction(this);
    separator->setSeparator(true);
    addAction(separator);

    m_copyAction = KStandardAction::copy(this, SLOT(slotEditCopy()), parent->actionCollection());
    parent->actionCollection()->setDefaultShortcut(
        m_copyAction, QKeySequence::Copy);
    m_copyAction->setWhatsThis(i18n("Copy"));
    m_copyAction->setToolTip(m_copyAction->whatsThis());
    m_copyAction->setStatusTip(m_copyAction->whatsThis());
    addAction(m_copyAction);

    m_cutAction = KStandardAction::cut(this, SLOT(slotCutEntry()), parent->actionCollection());
    parent->actionCollection()->setDefaultShortcut(
        m_cutAction, QKeySequence::Cut);
    m_cutAction->setWhatsThis(i18n("Cut"));
    m_cutAction->setToolTip(m_cutAction->whatsThis());
    m_cutAction->setStatusTip(m_cutAction->whatsThis());
    addAction(m_cutAction);

    m_pasteAction = KStandardAction::paste(this, SLOT(slotEditPaste()), parent->actionCollection());
    parent->actionCollection()->setDefaultShortcut(
        m_pasteAction, QKeySequence::Paste);
    m_pasteAction->setWhatsThis(i18n("Paste"));
    m_pasteAction->setToolTip(m_pasteAction->whatsThis());
    m_pasteAction->setStatusTip(m_pasteAction->whatsThis());
    addAction(m_pasteAction);

    m_selectAllAction = KStandardAction::selectAll(this, SLOT(selectAll()), parent->actionCollection());
    parent->actionCollection()->setDefaultShortcut(
        m_selectAllAction, QKeySequence::SelectAll);
    m_selectAllAction->setWhatsThis(i18n("Select all rows"));
    m_selectAllAction->setToolTip(m_selectAllAction->whatsThis());
    m_selectAllAction->setStatusTip(m_selectAllAction->whatsThis());

    m_clearSelectionAction = KStandardAction::deselect(this, SLOT(clearSelection()), parent->actionCollection());
    parent->actionCollection()->setDefaultShortcut(
        m_clearSelectionAction, QKeySequence::Deselect);
    m_clearSelectionAction->setWhatsThis(i18n("Deselect all rows"));
    m_clearSelectionAction->setToolTip(m_clearSelectionAction->whatsThis());
    m_clearSelectionAction->setStatusTip(m_clearSelectionAction->whatsThis());

    // vocabulary columns dialog
    QAction *vocabularyColumnsDialogAction = new QAction(this);
    parent->actionCollection()->addAction(QStringLiteral("show_vocabulary_columns_dialog"), vocabularyColumnsDialogAction);
    vocabularyColumnsDialogAction->setIcon(QIcon::fromTheme(QStringLiteral("view-file-columns")));
    vocabularyColumnsDialogAction->setText(i18n("Vocabulary Columns..."));
    vocabularyColumnsDialogAction->setWhatsThis(i18n("Toggle display of individual vocabulary columns"));
    vocabularyColumnsDialogAction->setToolTip(vocabularyColumnsDialogAction->whatsThis());
    vocabularyColumnsDialogAction->setStatusTip(vocabularyColumnsDialogAction->whatsThis());
    horizontalHeader()->addAction(vocabularyColumnsDialogAction);
    addAction(vocabularyColumnsDialogAction);
    connect(vocabularyColumnsDialogAction, &QAction::triggered, this, &VocabularyView::slotShowVocabularyColumnsDialog);
}

void VocabularyView::setFilter(VocabularyFilter * model)
{
    QTableView::setModel(model);

    m_model = model;
    connect(selectionModel(), &QItemSelectionModel::currentChanged,
            this, &VocabularyView::slotCurrentChanged);
    connect(selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &VocabularyView::slotSelectionChanged);
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
        ds.load();
        visibleColumns = ds.visibleColumns();

        KConfig parleyConfig(QStringLiteral("parleyrc"));
        KConfigGroup documentGroup(&parleyConfig, "Document " + m_doc->url().url());
        QByteArray state = documentGroup.readEntry("VocabularyColumns", QByteArray());

        if (!horizontalHeader()->restoreState(state)) {
            resizeColumnsToContents();
        }
    }

    horizontalHeader()->setSectionsMovable(true);

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
    ds.save();

    QByteArray saveState = horizontalHeader()->saveState();
    KConfig parleyConfig(QStringLiteral("parleyrc"));
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
    std::sort(sortedIndexes.begin(), sortedIndexes.end());
    QMimeData *mimeData = m_model->mimeData(sortedIndexes);

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setMimeData(mimeData);
}

void VocabularyView::slotEditPaste()
{
    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    const VocabularyMimeData *vocMimeData = qobject_cast<const VocabularyMimeData *>(mimeData);
    if (vocMimeData) {
        qDebug() << "Clipboard contains vocabulary mime data.";
        foreach(const VocabularyMimeData::MimeExpression & mimeEntry, vocMimeData->expressionList()) {
            KEduVocExpression *pasteExpression = new KEduVocExpression(mimeEntry.expression);
            m_model->appendEntry(pasteExpression);

            // find word type (create if not found)
            KEduVocWordType *type = m_doc->wordTypeContainer();
            foreach(int translation, mimeEntry.wordTypes.keys()) {
                // append if needed
                foreach(const QString & typeName, mimeEntry.wordTypes.value(translation).wordType) {
                    qDebug() << mimeEntry.wordTypes.value(translation).wordType;
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
        qDebug() << "Clipboard contains text data.";
        // split at newline
        QStringList lines = clipboard->text().split('\n');
        foreach(QString line, lines) {
            // split at tabs or semicolon:
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
            m_model->appendEntry(new KEduVocExpression(line.split(QRegExp(QStringLiteral("[\t;]")), QString::KeepEmptyParts)));
#else
            m_model->appendEntry(new KEduVocExpression(line.split(QRegExp(QStringLiteral("[\t;]")), Qt::KeepEmptyParts)));
#endif
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

void VocabularyView::setDocument(const std::shared_ptr<KEduVocDocument> &doc)
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
    VocabularyColumnsDialog *dialog = new VocabularyColumnsDialog(m_doc.get(), this);

    if (dialog->exec() == QDialog::Accepted) {
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
        connect(m_spellDialog, SIGNAL(done(QString)), this, SLOT(continueSpelling()));
        connect(m_spellDialog, &Sonnet::Dialog::misspelling, this, &VocabularyView::misspelling);
        connect(m_spellDialog, &Sonnet::Dialog::replace, this, &VocabularyView::spellingReplace);
    }

    m_spellColumn = language * VocabularyModel::EntryColumnsMAX;
    m_spellRow = -1;
    if (m_spellColumn < 0) {
        return;
    }

    QString locale = m_doc->identifier(language).locale();
    LanguageSettings settings(locale);
    QString spellCode = settings.spellChecker().isEmpty() ? locale : settings.spellChecker();
    m_spellChecker->changeLanguage(spellCode);
    if (!m_spellChecker->speller().isValid()) {
        qDebug() << "Invalid Language, popup here!";
        KNotification::event(KNotification::Warning, i18nc("@title of a popup", "No Spell Checker Available"), i18nc("@popupmessage", "Either the language set up is incorrect or no spellchecker was installed for this locale: %1.", locale));
    }
    m_spellDialog->show();
    continueSpelling();
}

void VocabularyView::continueSpelling()
{
    qDebug() << "Check spelling: " << m_spellRow << m_spellColumn;
    ++m_spellRow;
    while (m_spellRow < m_model->rowCount()) {
        QModelIndex index = m_model->index(m_spellRow, m_spellColumn);
        qDebug() << "  " << m_model->data(index).toString();
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
    qDebug() << oldWord << start << newWord;
    QModelIndex index = m_model->index(m_spellRow, m_spellColumn);
    QString data = index.data().toString();
    QString newData = data.replace(start, oldWord.length(), newWord);
    qDebug() << "Changing " << data << " to " << newData;
    m_model->setData(index, newData);
}

QModelIndexList VocabularyView::getSelectedIndexes() const
{
    return selectionModel()->selectedIndexes();
}
