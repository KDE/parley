/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include "ui_exportdialog.h"

#include <QDialog>

class ParleyDocument;
class QUrl;

class ExportDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ExportDialog(ParleyDocument *doc, QWidget *parent = 0);

private slots:
    void accept() Q_DECL_OVERRIDE;

private:
    QUrl getFileName(const QString& filter);

    Ui::ExportOptions* ui;
    ParleyDocument *m_doc;
    QWidget *m_parent;
};

#endif
