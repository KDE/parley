/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef WORDTYPEVIEW_H
#define WORDTYPEVIEW_H

#include "containerview.h"
#include "wordtypeview.h"

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

    void setTranslation(KEduVocExpression *entry, int translationId);

signals:
    void selectedWordTypeChanged(KEduVocWordType *wordType);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

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
    QAction *m_actionNewWordType{nullptr};
    QAction *m_actionDeleteWordType{nullptr};
    QAction *m_actionRenameWordType{nullptr};
    KSelectAction *m_actionSpecialTypeMenu{nullptr};

    QAction *m_noneAction{nullptr};
    QAction *m_separator{nullptr};
    QAction *m_nounAction{nullptr};
    QAction *m_nounMaleAction{nullptr};
    QAction *m_nounFemaleAction{nullptr};
    QAction *m_nounNeutralAction{nullptr};
    QAction *m_adjectiveAction{nullptr};
    QAction *m_adverbAction{nullptr};
    QAction *m_verbAction{nullptr};
    QAction *m_conjunctionAction{nullptr};
};
}

#endif
