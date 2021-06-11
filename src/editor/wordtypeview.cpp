/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "wordtypeview.h"

#include <QContextMenuEvent>
#include <QMenu>

#include <QAction>
#include <KLocalizedString>
#include <KMessageBox>
#include <KSelectAction>

#include <KEduVocExpression>
#include <KEduVocWordtype>

#include "editor/editor.h"
#include "containermodel.h"

using namespace Editor;

WordTypeView::WordTypeView(EditorWindow* parent) : ContainerView(parent)
{
    setContextMenuPolicy(Qt::DefaultContextMenu);

    m_actionNewWordType = new QAction(this);
    m_actionNewWordType->setText(i18n("New"));
    m_actionNewWordType->setIcon(QIcon::fromTheme(QStringLiteral("lesson-add")));
    m_actionNewWordType->setWhatsThis(i18n("Add a new word type to your document"));
    m_actionNewWordType->setToolTip(m_actionNewWordType->whatsThis());
    m_actionNewWordType->setStatusTip(m_actionNewWordType->whatsThis());
    m_actionNewWordType->setStatusTip(m_actionNewWordType->whatsThis());

    m_actionRenameWordType = new QAction(this);
    m_actionRenameWordType->setText(i18n("Rename"));
    m_actionRenameWordType->setIcon(QIcon::fromTheme(QStringLiteral("edit-rename")));
    m_actionRenameWordType->setWhatsThis(i18n("Rename the selected word type"));
    m_actionRenameWordType->setToolTip(m_actionRenameWordType->whatsThis());
    m_actionRenameWordType->setStatusTip(m_actionRenameWordType->whatsThis());
    m_actionRenameWordType->setStatusTip(m_actionRenameWordType->whatsThis());

    m_actionDeleteWordType = new QAction(this);
    m_actionDeleteWordType->setText(i18n("Delete Word Type"));
    m_actionDeleteWordType->setIcon(QIcon::fromTheme(QStringLiteral("lesson-remove")));
    m_actionDeleteWordType->setWhatsThis(i18n("Delete the selected word type."));
    m_actionDeleteWordType->setToolTip(m_actionDeleteWordType->whatsThis());
    m_actionDeleteWordType->setStatusTip(m_actionDeleteWordType->whatsThis());
    m_actionDeleteWordType->setStatusTip(m_actionDeleteWordType->whatsThis());

    m_actionSpecialTypeMenu = new KSelectAction(this);
    m_actionSpecialTypeMenu->setText(i18nc("Let the user select what grammatical meaning is connected to a word type (nouns have gender, verbs conjugations etc)", "Grammar"));
    //actionSplitWordType->setIcon(QIcon::fromTheme(""));  /// @todo better icon
    m_actionSpecialTypeMenu->setWhatsThis(i18n("To let Parley know the grammatical meaning of a word type."));
    m_actionSpecialTypeMenu->setToolTip(m_actionSpecialTypeMenu->whatsThis());
    m_actionSpecialTypeMenu->setStatusTip(m_actionSpecialTypeMenu->whatsThis());
    m_actionSpecialTypeMenu->setStatusTip(m_actionSpecialTypeMenu->whatsThis());


    m_nounAction = new QAction(this);
    m_nounAction->setText(i18n("Noun"));
    m_nounAction->setCheckable(true);
    m_nounAction->setWhatsThis(i18n("This word type folder contains nouns."));
    m_nounAction->setToolTip(m_nounAction->whatsThis());
    m_nounAction->setStatusTip(m_nounAction->whatsThis());
    m_nounAction->setStatusTip(m_nounAction->whatsThis());

    m_nounMaleAction = new QAction(this);
    m_nounMaleAction->setText(i18n("Masculine Noun"));
    m_nounMaleAction->setCheckable(true);
    m_nounMaleAction->setWhatsThis(i18n("This word type folder contains masculine nouns."));
    m_nounMaleAction->setToolTip(m_nounMaleAction->whatsThis());
    m_nounMaleAction->setStatusTip(m_nounMaleAction->whatsThis());
    m_nounMaleAction->setStatusTip(m_nounMaleAction->whatsThis());

    m_nounFemaleAction = new QAction(this);
    m_nounFemaleAction->setText(i18n("Feminine Noun"));
    m_nounFemaleAction->setCheckable(true);
    m_nounFemaleAction->setWhatsThis(i18n("This word type folder contains feminine nouns."));
    m_nounFemaleAction->setToolTip(m_nounFemaleAction->whatsThis());
    m_nounFemaleAction->setStatusTip(m_nounFemaleAction->whatsThis());
    m_nounFemaleAction->setStatusTip(m_nounFemaleAction->whatsThis());

    m_nounNeutralAction = new QAction(this);
    m_nounNeutralAction->setText(i18n("Neuter Noun"));
    m_nounNeutralAction->setCheckable(true);
    m_nounNeutralAction->setWhatsThis(i18n("This word type folder contains neuter nouns."));
    m_nounNeutralAction->setToolTip(m_nounNeutralAction->whatsThis());
    m_nounNeutralAction->setStatusTip(m_nounNeutralAction->whatsThis());
    m_nounNeutralAction->setStatusTip(m_nounNeutralAction->whatsThis());

    m_adjectiveAction = new QAction(this);
    m_adjectiveAction->setText(i18n("Adjective"));
    m_adjectiveAction->setCheckable(true);
    m_adjectiveAction->setWhatsThis(i18n("This word type folder contains adjectives."));
    m_adjectiveAction->setToolTip(m_adjectiveAction->whatsThis());
    m_adjectiveAction->setStatusTip(m_adjectiveAction->whatsThis());
    m_adjectiveAction->setStatusTip(m_adjectiveAction->whatsThis());

    m_adverbAction = new QAction(this);
    m_adverbAction->setText(i18n("Adverb"));
    m_adverbAction->setCheckable(true);
    m_adverbAction->setWhatsThis(i18n("This word type folder contains adverbs."));
    m_adverbAction->setToolTip(m_adverbAction->whatsThis());
    m_adverbAction->setStatusTip(m_adverbAction->whatsThis());
    m_adverbAction->setStatusTip(m_adverbAction->whatsThis());


    m_verbAction = new QAction(this);
    m_verbAction->setText(i18n("Verb"));
    m_verbAction->setCheckable(true);
    m_verbAction->setWhatsThis(i18n("This word type folder contains verbs."));
    m_verbAction->setToolTip(m_verbAction->whatsThis());
    m_verbAction->setStatusTip(m_verbAction->whatsThis());
    m_verbAction->setStatusTip(m_verbAction->whatsThis());

    m_conjunctionAction = new QAction(this);
    m_conjunctionAction->setText(i18n("Conjunction"));
    m_conjunctionAction->setCheckable(true);
    m_conjunctionAction->setWhatsThis(i18n("This word type folder contains conjunctions."));
    m_conjunctionAction->setToolTip(m_conjunctionAction->whatsThis());
    m_conjunctionAction->setStatusTip(m_conjunctionAction->whatsThis());
    m_conjunctionAction->setStatusTip(m_conjunctionAction->whatsThis());

    m_noneAction = new QAction(this);
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
    m_actionSpecialTypeMenu->addAction(m_conjunctionAction);

    connect(m_actionNewWordType, &QAction::triggered, this, &WordTypeView::slotCreateNewWordType);
    connect(m_actionRenameWordType, &QAction::triggered, this, &WordTypeView::slotRename);
    connect(m_actionDeleteWordType, &QAction::triggered, this, &WordTypeView::slotDeleteWordType);

    // right click menu for the WordType view:
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

    connect(m_noneAction, &QAction::triggered, this, &WordTypeView::setWordTypeNone);
    connect(m_nounAction, &QAction::triggered, this, &WordTypeView::setWordTypeNoun);
    connect(m_nounMaleAction, &QAction::triggered, this, &WordTypeView::setWordTypeNounMale);
    connect(m_nounFemaleAction, &QAction::triggered, this, &WordTypeView::setWordTypeNounFemale);
    connect(m_nounNeutralAction, &QAction::triggered, this, &WordTypeView::setWordTypeNounNeutral);
    connect(m_adjectiveAction, &QAction::triggered, this, &WordTypeView::setWordTypeAdjective);
    connect(m_adverbAction, &QAction::triggered, this, &WordTypeView::setWordTypeAdverb);
    connect(m_verbAction, &QAction::triggered, this, &WordTypeView::setWordTypeVerb);
    connect(m_conjunctionAction, &QAction::triggered, this, &WordTypeView::setWordTypeConjunction);
}

void WordTypeView::setTranslation(KEduVocExpression * entry, int translation)
{
    if (entry == 0) {
        return;
    }

    // attempt to find the container to select
    QModelIndex index(m_model->index(entry->translation(translation)->wordType()));
    scrollTo(index);
    selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
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
        KMessageBox::information(this, i18n("The root word type cannot be deleted."));
        return;
    }

    KEduVocWordType* WordType = static_cast<KEduVocWordType*>(selectedIndex.internalPointer());

    int count = WordType->entryCount();

    if (count == 0 ||
            KMessageBox::warningYesNo(this, i18np("There is a word left with this word type. It will lose its type. Continue?", "There are %1 words left with this word type. They will lose their word type. Continue?", count)) == KMessageBox::Yes) {
        m_model->deleteContainer(selectedIndex);
    }
}

void WordTypeView::contextMenuEvent(QContextMenuEvent * event)
{
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

        else if (t.testFlag(KEduVocWordFlag::Conjunction))
            m_conjunctionAction->setChecked(true);
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


void WordTypeView::setWordTypeConjunction()
{
    KEduVocWordType* wordType = static_cast<KEduVocWordType*>(selectionModel()->currentIndex().internalPointer());
    wordType->setWordType(KEduVocWordFlag::Conjunction);
}
