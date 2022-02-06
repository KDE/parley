/*
    SPDX-FileCopyrightText: 2007-2010 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef CONJUGATIONOPTIONS_H
#define CONJUGATIONOPTIONS_H

#include <QMap>
#include <QWidget>

class KEduVocDocument;
class QTreeWidget;
class QTreeWidgetItem;

class ConjugationOptions : public QWidget
{
    Q_OBJECT
public:
    ConjugationOptions(KEduVocDocument *doc, QWidget *parent);

public Q_SLOTS:
    void setLanguages(int from, int to);
    void updateSettings();

Q_SIGNALS:
    void checkBoxChanged();

private:
    void setupTenses();

private Q_SLOTS:
    void processCheckBoxChanged(QTreeWidgetItem *item, int column);

private:
    KEduVocDocument *m_doc;
    int m_language;
    QTreeWidget *m_treeWidget;
    QMap<QTreeWidgetItem *, Qt::CheckState> m_checkStates;
};

#endif
