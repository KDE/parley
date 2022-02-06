/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef VOCABULARYHEADERVIEW_H
#define VOCABULARYHEADERVIEW_H

#include <QHeaderView>

namespace Editor
{
class VocabularyHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit VocabularyHeaderView(Qt::Orientation orientation, QWidget *parent = nullptr);

private Q_SLOTS:
    void updateSorting(int);

private:
    int m_sortSection{-1};
};
}

#endif
