/***************************************************************************
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include "ui_exportdialog.h"

#include <QDialog>

class KEduVocDocument;
class QUrl;

class ExportDialog : public QDialog
{
    Q_OBJECT
public:
    ExportDialog( KEduVocDocument *doc, QWidget *parent = 0 );

signals:
    /**
     * Signal connected in KEduVocEditorDocument like this
     * 
     * connect( exportDialog, &ExportDialog::saveEditorDocument, this, &KEduVocEditorDocument::saveAs( filename ) );
     */
    void saveEditorDocument( QUrl filename );

private slots:
    void accept();

private:
    QUrl getFileName(const QString& filter);

    Ui::ExportOptions* ui;
    KEduVocDocument *m_doc;
    QWidget *m_parent;
};

#endif
