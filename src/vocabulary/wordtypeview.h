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

#ifndef WORDTYPEVIEW_H
#define WORDTYPEVIEW_H

#include <QtGui/QTreeView>

class KAction;
class KSelectAction;
class KEduVocWordType;
class KEduVocExpression;

namespace Editor {
    class EditorWindow;
    class WordTypeModel;

class WordTypeView :public QTreeView
{
    Q_OBJECT

public:
    WordTypeView(QWidget *parent = 0);
    void setModel(WordTypeModel* model);

public slots:
    /** Append a word type to the model and automatically set an edit up so the user can change "New lesson" into something meaningfull.*/
    void slotCreateNewWordType();

    /** Remove a word type. */
    void slotDeleteWordType();

    void setTranslation(KEduVocExpression* entry, int translationId);

signals:
    void selectedWordTypeChanged(KEduVocWordType* wordType);

protected:
    void contextMenuEvent ( QContextMenuEvent * event );

private slots:
    void setWordTypeNone();
    void setWordTypeNoun();
    void setWordTypeNounMale();
    void setWordTypeNounFemale();
    void setWordTypeNounNeutral();
    void setWordTypeAdjective();
    void setWordTypeAdverb();
    void setWordTypeVerb();

private:
    KAction *m_actionNewWordType;
    KAction *m_actionDeleteWordType;
    KAction *m_actionRenameWordType;
    KSelectAction *m_actionSpecialTypeMenu;

    KAction *m_noneAction;
    KAction *m_separator;
    KAction *m_nounAction;
    KAction *m_nounMaleAction;
    KAction *m_nounFemaleAction;
    KAction *m_nounNeutralAction;
    KAction *m_adjectiveAction;
    KAction *m_adverbAction;
    KAction *m_verbAction;

    WordTypeModel* m_model;
    
    // intentionally hide this to make sure a proper model is passed
    virtual void setModel(QAbstractItemModel *model) { Q_UNUSED(model) }
};
}

#endif
