/***************************************************************************

    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "wordtypeview.h"

#include "editor/editor.h"
#include "containermodel.h"
#include "keduvocwordtype.h"
#include "prefs.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <KInputDialog>
#include <KAction>
#include <KActionCollection>
#include <KSelectAction>
#include <QMenu>
#include <QContextMenuEvent>

WordTypeView::WordTypeView(Editor* parent) : ContainerView(parent)
{
    setContextMenuPolicy(Qt::DefaultContextMenu);

    m_actionNewWordType = new KAction(this);
    m_actionNewWordType->setText(i18n("New"));
    m_actionNewWordType->setIcon(KIcon("lesson-add"));
    m_actionNewWordType->setWhatsThis(i18n("Add a new word type to your document"));
    m_actionNewWordType->setToolTip(m_actionNewWordType->whatsThis());
    m_actionNewWordType->setStatusTip(m_actionNewWordType->whatsThis());
    m_actionNewWordType->setStatusTip(m_actionNewWordType->whatsThis());

    m_actionRenameWordType = new KAction(this);
    m_actionRenameWordType->setText(i18n("Rename"));
    m_actionRenameWordType->setIcon(KIcon("edit-rename"));
    m_actionRenameWordType->setWhatsThis(i18n("Rename the selected WordType"));
    m_actionRenameWordType->setToolTip(m_actionRenameWordType->whatsThis());
    m_actionRenameWordType->setStatusTip(m_actionRenameWordType->whatsThis());
    m_actionRenameWordType->setStatusTip(m_actionRenameWordType->whatsThis());

    m_actionDeleteWordType = new KAction(this);
    m_actionDeleteWordType->setText(i18n("Delete Word Type"));
    m_actionDeleteWordType->setIcon(KIcon("lesson-remove"));
    m_actionDeleteWordType->setWhatsThis(i18n("Delete the selected WordType."));
    m_actionDeleteWordType->setToolTip(m_actionDeleteWordType->whatsThis());
    m_actionDeleteWordType->setStatusTip(m_actionDeleteWordType->whatsThis());
    m_actionDeleteWordType->setStatusTip(m_actionDeleteWordType->whatsThis());

    m_actionSpecialTypeMenu = new KSelectAction(this);
    m_actionSpecialTypeMenu->setText(i18nc("Let the user select what grammatical meaning is connected to a word type (nouns have gender, verbs conjugations etc)", "Grammar"));
    //actionSplitWordType->setIcon(KIcon(""));  /// @todo better icon
    m_actionSpecialTypeMenu->setWhatsThis(i18n("To let Parley know the grammatical meaning of a word type."));
    m_actionSpecialTypeMenu->setToolTip(m_actionSpecialTypeMenu->whatsThis());
    m_actionSpecialTypeMenu->setStatusTip(m_actionSpecialTypeMenu->whatsThis());
    m_actionSpecialTypeMenu->setStatusTip(m_actionSpecialTypeMenu->whatsThis());


    m_nounAction = new KAction(this);
    m_nounAction->setText(i18n("Noun"));
    m_nounAction->setCheckable(true);
    m_nounAction->setWhatsThis(i18n("This word type folder contains nouns."));
    m_nounAction->setToolTip(m_nounAction->whatsThis());
    m_nounAction->setStatusTip(m_nounAction->whatsThis());
    m_nounAction->setStatusTip(m_nounAction->whatsThis());

    m_nounMaleAction = new KAction(this);
    m_nounMaleAction->setText(i18n("Masculine Noun"));
    m_nounMaleAction->setCheckable(true);
    m_nounMaleAction->setWhatsThis(i18n("This word type folder contains masculine nouns."));
    m_nounMaleAction->setToolTip(m_nounMaleAction->whatsThis());
    m_nounMaleAction->setStatusTip(m_nounMaleAction->whatsThis());
    m_nounMaleAction->setStatusTip(m_nounMaleAction->whatsThis());

    m_nounFemaleAction = new KAction(this);
    m_nounFemaleAction->setText(i18n("Feminine Noun"));
    m_nounFemaleAction->setCheckable(true);
    m_nounFemaleAction->setWhatsThis(i18n("This word type folder contains feminine nouns."));
    m_nounFemaleAction->setToolTip(m_nounFemaleAction->whatsThis());
    m_nounFemaleAction->setStatusTip(m_nounFemaleAction->whatsThis());
    m_nounFemaleAction->setStatusTip(m_nounFemaleAction->whatsThis());

    m_nounNeutralAction = new KAction(this);
    m_nounNeutralAction->setText(i18n("Neuter Noun"));
    m_nounNeutralAction->setCheckable(true);
    m_nounNeutralAction->setWhatsThis(i18n("This word type folder contains neuter nouns."));
    m_nounNeutralAction->setToolTip(m_nounNeutralAction->whatsThis());
    m_nounNeutralAction->setStatusTip(m_nounNeutralAction->whatsThis());
    m_nounNeutralAction->setStatusTip(m_nounNeutralAction->whatsThis());

    m_adjectiveAction = new KAction(this);
    m_adjectiveAction->setText(i18n("Adjective"));
    m_adjectiveAction->setCheckable(true);
    m_adjectiveAction->setWhatsThis(i18n("This word type folder contains adjectives."));
    m_adjectiveAction->setToolTip(m_adjectiveAction->whatsThis());
    m_adjectiveAction->setStatusTip(m_adjectiveAction->whatsThis());
    m_adjectiveAction->setStatusTip(m_adjectiveAction->whatsThis());

    m_adverbAction = new KAction(this);
    m_adverbAction->setText(i18n("Adverb"));
    m_adverbAction->setCheckable(true);
    m_adverbAction->setWhatsThis(i18n("This word type folder contains adverbs."));
    m_adverbAction->setToolTip(m_adverbAction->whatsThis());
    m_adverbAction->setStatusTip(m_adverbAction->whatsThis());
    m_adverbAction->setStatusTip(m_adverbAction->whatsThis());


    m_verbAction = new KAction(this);
    m_verbAction->setText(i18n("Verb"));
    m_verbAction->setCheckable(true);
    m_verbAction->setWhatsThis(i18n("This word type folder contains verbs."));
    m_verbAction->setToolTip(m_verbAction->whatsThis());
    m_verbAction->setStatusTip(m_verbAction->whatsThis());
    m_verbAction->setStatusTip(m_verbAction->whatsThis());

    m_noneAction = new KAction(this);
    m_noneAction->setText(i18n("No Special Type"));
    m_noneAction->setCheckable(true);
    m_noneAction->setChecked(true);
    m_noneAction->setWhatsThis(i18n("This word type folder contains no word type with special meaning."));
    m_noneAction->setToolTip(m_noneAction->whatsThis());
    m_noneAction->setStatusTip(m_noneAction->whatsThis());
    m_noneAction->setStatusTip(m_noneAction->whatsThis());

    QAction* separator = new QAction(this);
    separator->setSeparator(true);

    m_actionSpecialTypeMenu->addAction(m_noneAction);
    m_actionSpecialTypeMenu->addAction(separator);
    m_actionSpecialTypeMenu->addAction(m_nounAction);
    m_actionSpecialTypeMenu->addAction(m_nounMaleAction);
    m_actionSpecialTypeMenu->addAction(m_nounFemaleAction);
    m_actionSpecialTypeMenu->addAction(m_nounNeutralAction);
    m_actionSpecialTypeMenu->addAction(m_adjectiveAction);
    m_actionSpecialTypeMenu->addAction(m_adverbAction);
    m_actionSpecialTypeMenu->addAction(m_verbAction);

    connect(m_actionNewWordType, SIGNAL(triggered()),
            SLOT(slotCreateNewWordType()));
    connect(m_actionRenameWordType, SIGNAL(triggered()),
            SLOT(slotRename()));
    connect(m_actionDeleteWordType, SIGNAL(triggered()),
            SLOT(slotDeleteWordType()));


    // right cick menu for the WordType view:
    addAction(m_actionNewWordType);
    addAction(m_actionRenameWordType);
    addAction(m_actionDeleteWordType);
    separator = new QAction(this);
    separator->setSeparator(true);
    addAction(separator);
    separator = new QAction(this);
    separator->setSeparator(true);
    addAction(separator);
    addAction(m_actionSpecialTypeMenu);

    connect(m_noneAction, SIGNAL(triggered()),
            SLOT(setWordTypeNone()));
    connect(m_nounAction, SIGNAL(triggered()),
            SLOT(setWordTypeNoun()));
    connect(m_nounMaleAction, SIGNAL(triggered()),
            SLOT(setWordTypeNounMale()));
    connect(m_nounFemaleAction, SIGNAL(triggered()),
            SLOT(setWordTypeNounFemale()));
    connect(m_nounNeutralAction, SIGNAL(triggered()),
            SLOT(setWordTypeNounNeutral()));
    connect(m_adjectiveAction, SIGNAL(triggered()),
            SLOT(setWordTypeAdjective()));
    connect(m_adverbAction, SIGNAL(triggered()),
            SLOT(setWordTypeAdverb()));
    connect(m_verbAction, SIGNAL(triggered()),
            SLOT(setWordTypeVerb()));
}

void WordTypeView::currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
    QTreeView::currentChanged(current, previous);

    if (current.isValid()) {
        KEduVocWordType *container = static_cast<KEduVocWordType*>(current.internalPointer());
        if (container) {
            emit selectedWordTypeChanged(container);
            emit signalShowContainer(container);
        }
    }
}

void WordTypeView::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
    QTreeView::selectionChanged(selected, deselected);

    if(selected.count() == 0) {
        return;
    }

    KEduVocWordType *container = static_cast<KEduVocWordType*>(selected.indexes().value(0).internalPointer());
    if (container) {
        emit selectedWordTypeChanged(container);
    }
}

void WordTypeView::slotCreateNewWordType()
{
    QModelIndex selectedIndex = selectionModel()->currentIndex();

    if (!selectedIndex.isValid()) {
        selectedIndex = m_model->index(0, 0, QModelIndex());
    }

    QModelIndex modelIndex = m_model->appendContainer(selectedIndex);

    scrollTo(modelIndex);
    selectionModel()->setCurrentIndex(modelIndex, QItemSelectionModel::ClearAndSelect);
    edit(modelIndex);    // let the user type a new name for the WordType
}

void WordTypeView::slotDeleteWordType()
{
    QModelIndex selectedIndex = selectionModel()->currentIndex();

    if (selectedIndex.parent() == QModelIndex()) {
        KMessageBox::information(this, i18n("The root WordType cannot be deleted."));
        return;
    }

    KEduVocWordType* WordType = static_cast<KEduVocWordType*>(selectedIndex.internalPointer());

    int count = WordType->entryCount();

    if ( count == 0 ||
         KMessageBox::warningYesNo(this, i18np("There is a word left with this word type. It will lose its type. Continue?", "There are %1 words left with this word type. They will lose their word type. Continue?", count)) == KMessageBox::Yes) {
        m_model->deleteContainer(selectedIndex);
    }
}

void WordTypeView::contextMenuEvent(QContextMenuEvent * event)
{
    kDebug() << "Context menu event";

    // check for the root element:
    QModelIndex selectedIndex = selectionModel()->currentIndex();
    m_actionRenameWordType->setEnabled(selectedIndex.parent() != QModelIndex());
    m_actionDeleteWordType->setEnabled(selectedIndex.parent() != QModelIndex());
    m_actionSpecialTypeMenu->setEnabled(selectedIndex.parent() != QModelIndex());

    if (selectedIndex.isValid()) {
        KEduVocWordFlags t = (static_cast<KEduVocWordType*>(selectionModel()->currentIndex().internalPointer())->wordType());

        if (!t)
            m_noneAction->setChecked(true);

        else if (t.testFlag(KEduVocWordFlag::Noun) && t.testFlag(KEduVocWordFlag::Masculine))
            m_nounMaleAction->setChecked(true);

        else if (t.testFlag(KEduVocWordFlag::Noun) && t.testFlag(KEduVocWordFlag::Feminine))
            m_nounFemaleAction->setChecked(true);

        else if (t.testFlag(KEduVocWordFlag::Noun) && t.testFlag(KEduVocWordFlag::Neuter))
            m_nounNeutralAction->setChecked(true);

        else if (t.testFlag(KEduVocWordFlag::Noun))
            m_nounAction->setChecked(true);

        else if (t.testFlag(KEduVocWordFlag::Adjective))
            m_adjectiveAction->setChecked(true);

        else if (t.testFlag(KEduVocWordFlag::Adverb))
            m_adverbAction->setChecked(true);

        else if (t.testFlag(KEduVocWordFlag::Verb))
            m_verbAction->setChecked(true);
    }

    QMenu::exec(actions(), event->globalPos());
}

void WordTypeView::setWordTypeNone()
{
    KEduVocWordType* wordType = static_cast<KEduVocWordType*>(selectionModel()->currentIndex().internalPointer());
    wordType->setWordType(KEduVocWordFlag::NoInformation);
}

void WordTypeView::setWordTypeNoun()
{
    KEduVocWordType* wordType = static_cast<KEduVocWordType*>(selectionModel()->currentIndex().internalPointer());
    wordType->setWordType(KEduVocWordFlag::Noun);
}

void WordTypeView::setWordTypeNounMale()
{
    KEduVocWordType* wordType = static_cast<KEduVocWordType*>(selectionModel()->currentIndex().internalPointer());
    wordType->setWordType(KEduVocWordFlag::Noun | KEduVocWordFlag::Masculine);
}

void WordTypeView::setWordTypeNounFemale()
{
    KEduVocWordType* wordType = static_cast<KEduVocWordType*>(selectionModel()->currentIndex().internalPointer());
    wordType->setWordType(KEduVocWordFlag::Noun | KEduVocWordFlag::Feminine);
}

void WordTypeView::setWordTypeNounNeutral()
{
    KEduVocWordType* wordType = static_cast<KEduVocWordType*>(selectionModel()->currentIndex().internalPointer());
    wordType->setWordType(KEduVocWordFlag::Noun | KEduVocWordFlag::Neuter);
}

void WordTypeView::setWordTypeAdjective()
{
    KEduVocWordType* wordType = static_cast<KEduVocWordType*>(selectionModel()->currentIndex().internalPointer());
    wordType->setWordType(KEduVocWordFlag::Adjective);
}

void WordTypeView::setWordTypeAdverb()
{
    KEduVocWordType* wordType = static_cast<KEduVocWordType*>(selectionModel()->currentIndex().internalPointer());
    wordType->setWordType(KEduVocWordFlag::Adverb);
}

void WordTypeView::setWordTypeVerb()
{
    KEduVocWordType* wordType = static_cast<KEduVocWordType*>(selectionModel()->currentIndex().internalPointer());
    wordType->setWordType(KEduVocWordFlag::Verb);
}


#include "wordtypeview.moc"


