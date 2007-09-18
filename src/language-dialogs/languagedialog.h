/***************************************************************************

                             kvtnewdocumentwizardlanguagepage

    -----------------------------------------------------------------------

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef LANGUAGEDIALOG_H
#define LANGUAGEDIALOG_H

#include <KPageDialog>

class KEduVocDocument;

/**
 *
	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class LanguageDialog : public KPageDialog
{
Q_OBJECT
public:
    LanguageDialog(KEduVocDocument* doc, QWidget *parent);
    ~LanguageDialog();

    void accept();
    void reject();

signals:
    void signalCommitData();

private slots:
    void slotAppendIdentifier();
    void slotDeleteIdentifier();
    void pageNameChanged(const QString & newName);
    void pageIconChanged(const QString& newIcon);

private:
    KPageWidgetItem*  createPage(int identifierIndex);

    KEduVocDocument* m_doc;
    QList<KPageWidgetItem*> m_pages;
    QList<int> m_appendList;
    QList<int> m_deleteList;
};

#endif
