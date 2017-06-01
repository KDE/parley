/***************************************************************************
    Copyright 2008 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef LANGUAGEPROPERTIES_H
#define LANGUAGEPROPERTIES_H

#include <KPageDialog>

class KEduVocDocument;
class ParleyDocument;

class LanguageProperties : public KPageDialog
{
    Q_OBJECT
public:
    LanguageProperties(KEduVocDocument* doc, QWidget* parent);

public slots:
    void accept() Q_DECL_OVERRIDE;

private slots:
    void slotAppendIdentifier();
    void slotDeleteIdentifier();
    void pageNameChanged(const QString& newName);

private:
    KPageWidgetItem*  createPage(int identifierIndex);

    KEduVocDocument* m_doc;
    QList<KPageWidgetItem*> m_pages;
};

#endif
