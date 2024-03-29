/*
    dialog page for multiple choice suggestions
    SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    SPDX-FileCopyrightText: 2005-2006 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MULTIPLECHOICEWIDGET_H
#define MULTIPLECHOICEWIDGET_H

#include "ui_multiplechoicewidget.h"

#include <QModelIndex>

class KEduVocExpression;
class KEduVocTranslation;
class QStringListModel;

namespace Editor
{
class MultipleChoiceWidget : public QWidget, public Ui::multipleChoiceWidget
{
    Q_OBJECT

public:
    explicit MultipleChoiceWidget(QWidget *parent = nullptr);

public Q_SLOTS:
    void setTranslation(KEduVocExpression *entry, int translation);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private Q_SLOTS:
    void slotDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void slotAddChoiceButton();
    void slotRemoveChoiceButton();

private:
    KEduVocTranslation *m_translation{nullptr};
    QStringListModel *m_choicesModel{nullptr};
};
}

#endif
