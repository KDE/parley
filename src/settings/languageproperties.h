/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef LANGUAGEPROPERTIES_H
#define LANGUAGEPROPERTIES_H

#include <KPageDialog>

class KEduVocDocument;

class LanguageProperties : public KPageDialog
{
    Q_OBJECT
public:
    LanguageProperties(KEduVocDocument *doc, QWidget *parent);

public Q_SLOTS:
    void accept() override;

private Q_SLOTS:
    void slotAppendIdentifier();
    void slotDeleteIdentifier();
    void pageNameChanged(const QString &newName);

private:
    KPageWidgetItem *createPage(int identifierIndex);

    KEduVocDocument *m_doc{nullptr};
    QList<KPageWidgetItem *> m_pages;
};

#endif
