/***************************************************************************
    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef IMAGEPRACTICE_H
#define IMAGEPRACTICE_H

#include "ui_imagepracticedlg.h"
#include "practicedialog.h"

class ImagePracticeDlg : public PracticeDialog, Ui::ImagePracticeDlg
{
    Q_OBJECT

public:
    ImagePracticeDlg(KEduVocDocument *doc, QWidget *parent);

    ~ImagePracticeDlg();

    void setEntry(TestEntry* entry);
    void setProgressCounter(int current, int total);

public slots:
    void showSolution();
    void slotAnswerChanged();
    void verifyClicked();

private:
    void setStatusText(const QString &statusText) {}
    QProgressBar* timebar();

    void setQueryFieldWordwrap();
    QGraphicsScene* m_scene;
    bool m_wrongAnswer;
};

#endif
