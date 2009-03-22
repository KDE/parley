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

class ParleyDocument;

class LanguageProperties : public KPageDialog
{
Q_OBJECT
public:
    LanguageProperties(ParleyDocument* doc, QWidget *parent);

public slots:
    virtual void accept ();

private slots:
    void slotAppendIdentifier();
    void slotDeleteIdentifier();
    void pageNameChanged(const QString& newName);
    void pageIconChanged(const QString& newIcon);

private:
    KPageWidgetItem*  createPage(int identifierIndex);

    ParleyDocument* m_doc;
    QList<KPageWidgetItem*> m_pages;
};

#endif
