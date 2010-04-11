/***************************************************************************

                        show document statistics

    -----------------------------------------------------------------------

    begin         : Sun Sep 19 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    Copyright 2008-2010 Frederik Gladhorn <gladhorn@kde.org>
    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STATISTICSDIALOG_H
#define STATISTICSDIALOG_H

#include <KXmlGuiWindow>

class LessonStatisticsView;
class StatisticsModel;
class KEduVocDocument;
class ParleyMainWindow;
namespace Ui {
    class StatisticsMainWindow;
}

class StatisticsMainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    StatisticsMainWindow(KEduVocDocument* doc, ParleyMainWindow* parent);
    ~StatisticsMainWindow();
    
    void setDocument(KEduVocDocument* doc);

private slots:
    void configurePractice();
    void languagesChanged();

private:
    void initActions();
    void initLanguages();
    
    ParleyMainWindow* m_mainWindow; 
    KEduVocDocument* m_doc;
    StatisticsModel* m_statisticsModel;
    Ui::StatisticsMainWindow* m_ui;
};

#endif
