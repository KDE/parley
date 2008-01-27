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

#include "parley.h"
#include "containermodel.h"
#include "keduvocwordtype.h"
#include "prefs.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <KInputDialog>
#include <KAction>
#include <KActionCollection>
#include <KSelectAction>

WordTypeView::WordTypeView(ParleyApp * parent) :ContainerView(parent)
{
//     setContextMenuPolicy(Qt::CustomContextMenu);

    KAction *actionNewWordType = new KAction(this);
    parent->actionCollection()->addAction("new_wordtype", actionNewWordType);
    actionNewWordType->setText(i18n("New"));
    actionNewWordType->setIcon(KIcon("WordType-add"));
    actionNewWordType->setWhatsThis(i18n("Add a new word type to your document"));
    actionNewWordType->setToolTip(actionNewWordType->whatsThis());
    actionNewWordType->setStatusTip(actionNewWordType->whatsThis());
    actionNewWordType->setStatusTip(actionNewWordType->whatsThis());

    KAction *actionRenameWordType = new KAction(this);
    parent->actionCollection()->addAction("rename_wordtype", actionRenameWordType);
    actionRenameWordType->setText(i18n("Rename"));
    actionRenameWordType->setIcon(KIcon("edit-rename"));
    actionRenameWordType->setWhatsThis(i18n("Rename the selected WordType"));
    actionRenameWordType->setToolTip(actionRenameWordType->whatsThis());
    actionRenameWordType->setStatusTip(actionRenameWordType->whatsThis());
    actionRenameWordType->setStatusTip(actionRenameWordType->whatsThis());

    KAction *actionDeleteWordType = new KAction(this);
    parent->actionCollection()->addAction("delete_wordtype", actionDeleteWordType);
    actionDeleteWordType->setText(i18n("Delete"));
    actionDeleteWordType->setIcon(KIcon("WordType-remove"));
    actionDeleteWordType->setWhatsThis(i18n("Delete the selected WordType."));
    actionDeleteWordType->setToolTip(actionDeleteWordType->whatsThis());
    actionDeleteWordType->setStatusTip(actionDeleteWordType->whatsThis());
    actionDeleteWordType->setStatusTip(actionDeleteWordType->whatsThis());

    KSelectAction *actionSpecialTypeMenu = new KSelectAction(this);
    parent->actionCollection()->addAction("special_wordtype_menu", actionSpecialTypeMenu);
    actionSpecialTypeMenu->setText(i18nc("Let the user select what grammatical meaning is connected to a word type (nouns have gender, verbs conjugations etc)", "Grammar"));
    //actionSplitWordType->setIcon(KIcon(""));  /// @todo better icon
    actionSpecialTypeMenu->setWhatsThis(i18n("To let Parley know the grammatical meaning of a word type."));
    actionSpecialTypeMenu->setToolTip(actionSpecialTypeMenu->whatsThis());
    actionSpecialTypeMenu->setStatusTip(actionSpecialTypeMenu->whatsThis());
    actionSpecialTypeMenu->setStatusTip(actionSpecialTypeMenu->whatsThis());


    KAction *nounAction = new KAction(this);
    parent->actionCollection()->addAction("wordtype_noun", nounAction);
    nounAction->setText(i18n("Noun"));
    nounAction->setCheckable(true);
    nounAction->setWhatsThis(i18n("This word type folder contains nouns."));
    nounAction->setToolTip(nounAction->whatsThis());
    nounAction->setStatusTip(nounAction->whatsThis());
    nounAction->setStatusTip(nounAction->whatsThis());

    KAction *nounMaleAction = new KAction(this);
    parent->actionCollection()->addAction("wordtype_nounMale", nounMaleAction);
    nounMaleAction->setText(i18n("Male Noun"));
    nounMaleAction->setCheckable(true);
    nounMaleAction->setWhatsThis(i18n("This word type folder contains nounMales."));
    nounMaleAction->setToolTip(nounMaleAction->whatsThis());
    nounMaleAction->setStatusTip(nounMaleAction->whatsThis());
    nounMaleAction->setStatusTip(nounMaleAction->whatsThis());

    KAction *nounFemaleAction = new KAction(this);
    parent->actionCollection()->addAction("wordtype_nounFemale", nounFemaleAction);
    nounFemaleAction->setText(i18n("Female Noun"));
    nounFemaleAction->setCheckable(true);
    nounFemaleAction->setWhatsThis(i18n("This word type folder contains nounFemales."));
    nounFemaleAction->setToolTip(nounFemaleAction->whatsThis());
    nounFemaleAction->setStatusTip(nounFemaleAction->whatsThis());
    nounFemaleAction->setStatusTip(nounFemaleAction->whatsThis());

    KAction *nounNeutralAction = new KAction(this);
    parent->actionCollection()->addAction("wordtype_nounNeutral", nounNeutralAction);
    nounNeutralAction->setText(i18n("Neutral Noun"));
    nounNeutralAction->setCheckable(true);
    nounNeutralAction->setWhatsThis(i18n("This word type folder contains nounNeutrals."));
    nounNeutralAction->setToolTip(nounNeutralAction->whatsThis());
    nounNeutralAction->setStatusTip(nounNeutralAction->whatsThis());
    nounNeutralAction->setStatusTip(nounNeutralAction->whatsThis());

    KAction *adjectiveAction = new KAction(this);
    parent->actionCollection()->addAction("wordtype_adjective", adjectiveAction);
    adjectiveAction->setText(i18n("Adjective"));
    adjectiveAction->setCheckable(true);
    adjectiveAction->setWhatsThis(i18n("This word type folder contains adjectives."));
    adjectiveAction->setToolTip(adjectiveAction->whatsThis());
    adjectiveAction->setStatusTip(adjectiveAction->whatsThis());
    adjectiveAction->setStatusTip(adjectiveAction->whatsThis());

    KAction *adverbAction = new KAction(this);
    parent->actionCollection()->addAction("wordtype_adverb", adverbAction);
    adverbAction->setText(i18n("Adverb"));
    adverbAction->setCheckable(true);
    adverbAction->setWhatsThis(i18n("This word type folder contains adverbs."));
    adverbAction->setToolTip(adverbAction->whatsThis());
    adverbAction->setStatusTip(adverbAction->whatsThis());
    adverbAction->setStatusTip(adverbAction->whatsThis());


    KAction *verbAction = new KAction(this);
    parent->actionCollection()->addAction("wordtype_verb", verbAction);
    verbAction->setText(i18n("Verb"));
    verbAction->setCheckable(true);
    verbAction->setWhatsThis(i18n("This word type folder contains verbs."));
    verbAction->setToolTip(verbAction->whatsThis());
    verbAction->setStatusTip(verbAction->whatsThis());
    verbAction->setStatusTip(verbAction->whatsThis());

    KAction *noneAction = new KAction(this);
    parent->actionCollection()->addAction("wordtype_none", noneAction);
    noneAction->setText(i18n("No Special Type"));
    noneAction->setCheckable(true);
    noneAction->setChecked(true);
    noneAction->setWhatsThis(i18n("This word type folder contains nones."));
    noneAction->setToolTip(noneAction->whatsThis());
    noneAction->setStatusTip(noneAction->whatsThis());
    noneAction->setStatusTip(noneAction->whatsThis());

    QAction* separator = new QAction(this);
    separator->setSeparator(true);

    actionSpecialTypeMenu->addAction(noneAction);
    actionSpecialTypeMenu->addAction(separator);
    actionSpecialTypeMenu->addAction(nounAction);
    actionSpecialTypeMenu->addAction(nounMaleAction);
    actionSpecialTypeMenu->addAction(nounFemaleAction);
    actionSpecialTypeMenu->addAction(nounNeutralAction);
    actionSpecialTypeMenu->addAction(adjectiveAction);
    actionSpecialTypeMenu->addAction(adverbAction);
    actionSpecialTypeMenu->addAction(verbAction);

    connect(actionNewWordType, SIGNAL(triggered()),
            SLOT(slotCreateNewWordType()));
    connect(actionRenameWordType, SIGNAL(triggered()),
            SLOT(slotRename()));
    connect(actionDeleteWordType, SIGNAL(triggered()),
            SLOT(slotDeleteWordType()));



    // right cick menu for the WordType view:
    addAction(actionNewWordType);
    addAction(actionRenameWordType);
    addAction(actionDeleteWordType);
    separator = new QAction(this);
    separator->setSeparator(true);
    addAction(separator);
    separator = new QAction(this);
    separator->setSeparator(true);
    addAction(separator);
    addAction(actionSpecialTypeMenu);
}


void WordTypeView::slotCreateNewWordType()
{
    QModelIndex selectedIndex = selectionModel()->currentIndex();

    if (!selectedIndex.isValid()) {
        selectedIndex = m_model->index(0, 0, QModelIndex());
    }

    QModelIndex modelIndex = m_model->appendWordType(selectedIndex);

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
         KMessageBox::warningYesNo(this, i18np("There is %1 with this word type. It will lose its type. Continue?", "There are %1 words left with this word type. They will lose their word type. Continue?", count)) == KMessageBox::Yes) {
        m_model->deleteWordType(selectedIndex);
    }
}



#include "wordtypeview.moc"

