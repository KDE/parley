/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef WORDTYPEVIEW_H
#define WORDTYPEVIEW_H

#include "wordtypeview.h"
#include "containerview.h"

class QAction;
class KSelectAction;
class KEduVocWordType;
class KEduVocExpression;
namespace Editor
{
class EditorWindow;

class WordTypeView : public ContainerView
{
    Q_OBJECT

public:
    explicit WordTypeView(EditorWindow *parent);

public slots:
    /** Append a word type to the model and automatically set an edit up so the user can change "New lesson" into something meaningful.*/
    void slotCreateNewWordType();

    /** Remove a word type. */
    void slotDeleteWordType();

    void setTranslation(KEduVocExpression* entry, int translationId);

signals:
    void selectedWordTypeChanged(KEduVocWordType* wordType);

protected:
    void contextMenuEvent(QContextMenuEvent * event) Q_DECL_OVERRIDE;

private slots:
    void setWordTypeNone();
    void setWordTypeNoun();
    void setWordTypeNounMale();
    void setWordTypeNounFemale();
    void setWordTypeNounNeutral();
    void setWordTypeAdjective();
    void setWordTypeAdverb();
    void setWordTypeVerb();
    void setWordTypeConjunction();

private:
    QAction *m_actionNewWordType;
    QAction *m_actionDeleteWordType;
    QAction *m_actionRenameWordType;
    KSelectAction *m_actionSpecialTypeMenu;

    QAction *m_noneAction;
    QAction *m_separator;
    QAction *m_nounAction;
    QAction *m_nounMaleAction;
    QAction *m_nounFemaleAction;
    QAction *m_nounNeutralAction;
    QAction *m_adjectiveAction;
    QAction *m_adverbAction;
    QAction *m_verbAction;
    QAction *m_conjunctionAction;
};
}

#endif
