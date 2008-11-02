/***************************************************************************
    Copyright 2008 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PRACTICESTARTDIALOG_H
#define PRACTICESTARTDIALOG_H

#include <KDialog>

class KEduVocDocument;

class PracticeStartDialog : public KDialog
{
Q_OBJECT
public:
    PracticeStartDialog(QWidget *parent, KEduVocDocument *doc);
    ~PracticeStartDialog();
//     void updateRecentFilesModel();
// 
public slots:
    void configurePractice();
//     void slotPracticeUrl(const KUrl& url);
//     void slotDoubleClicked(const QModelIndex& index);
// 
private:
    KEduVocDocument *m_doc;
//     Ui::WelcomeScreen *ui;
//     ParleyMainWindow *m_mainWindow;
//     QStandardItemModel *m_recentFilesModel;
};

#endif
