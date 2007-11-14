/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef WORDTYPEWIDGET_H
#define WORDTYPEWIDGET_H


#include "ui_wordtypewidget.h"

#include <keduvocwordtype.h>
#include <keduvocdocument.h>

class WordTypeWidget : public QWidget, public Ui::WordTypeWidget
{
    Q_OBJECT
public:
    explicit WordTypeWidget(QWidget* parent = 0);
    void setDocument(KEduVocDocument *doc);

public slots:
    void setEntries(int currentTranslation, const QList<int>& entries);

signals:
    void signalTypeSelected(const QString &);

private:
    void updateMainTypeBoxContents();

private slots:
    /// Updates the subtype box
    void slotTypeBoxChanged(const QString &mainType);
    void slotSubTypeBoxChanged(const QString &mainType);

    void invokeTypeDlg();
    void slotSubDialogClosed();

private:
    KEduVocDocument     *m_doc;

    KDialog             *subDialog;

    /// Column in the document - corresponds to the language (-KV_COL_TRANS)
    int               m_currentTranslation;
    /// Selection in the doc - if more than one row is selected behavior is different
    QList<int>        m_entries;
    bool m_usageIsModified;
    KEduVocWordType   m_wordTypes;
};
#endif // WordTypeWidget_included
