/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    Copyright 2008 David Capel <wot.narg@gmail.com>

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef PARLEYPRACTICEMAINWINDOW_H
#define PARLEYPRACTICEMAINWINDOW_H

#include <KXmlGuiWindow>

#include <prefs.h>

class QString;

class ParleyPracticeMainWindow : public KXmlGuiWindow
{
        Q_OBJECT

    public:
        ParleyPracticeMainWindow(QWidget *parent = 0);
        /*
            ~ParleyPracticeMainWindow();*/

    // States that the actions can be in.
    static const int CheckAnswer = 1;
    static const int Continue = 2;

    protected:
        class PracticeView* m_view;
        class QGraphicsSvgItem* m_layout;
        class QGraphicsScene* m_scene;
        class KSvgRenderer* m_renderer;

        class PracticeEntryManager * m_manager;
        class Statistics* m_stats;
        class AnswerValidator* m_validator;

        int m_state;
        Prefs::EnumTestType::type m_mode;

        void setupBase();
        void setupActions();
        void setupModeSpecifics();

        // Mode specific setup functions
        void setupWritten();
	void setupMultipleChoice();
    public slots:
        void slotCheckAnswer(const QString& input);
        void slotShowSolution();
        void slotToggleCheckAnswerContinueActions();
        void slotCreatePreferencesDialog();
    signals:
        void signalCheckAnswer(const QString&, const QString&);
        void signalShowSolution(const QString&, int);
        void signalCheckAnswerContinueActionsToggled(int);
};


#endif
