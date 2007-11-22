/***************************************************************************

    Copyright: 2007 Peter Murdoch <pete@pmurdoch.com>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef WORDTYPEDOCKWIDGET_H
#define WORDTYPEDOCKWIDGET_H

#include <QDockWidget>

class KEduVocDocument;
class LessonView;
class LessonModel;
class KComboBox;
class ParleyApp;

/**
	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 */
class WordTypeDockWidget : public QDockWidget
{
  Q_OBJECT
  public:
    WordTypeDockWidget(ParleyApp *parent);

    ~WordTypeDockWidget();

    void setDocument(KEduVocDocument *doc);

    //int addLesson();

    void saveOptions();

    /** Make sure, the lesson is visible - if combo is set to
    1. all, no problem
    2. in query -> if current not in query change combo?
    3. current -> make the lesson current */
   // void makeLessonVisibleInTable(int lessonIndex);

    LessonView* wordTypeView();

  private:

    /** m_lessonView is the lesson list at the left side. */
    LessonView  *m_wordTypeView;
    LessonModel *m_wordTypeModel;

    /** m_editCombo selects which lessons to display in m_tableView (the main table) */
    KComboBox *m_wordTypeSelectionCombo;

    ParleyApp *m_parleyApp;
};

#endif
