/***************************************************************************
                kvoctrainprefs.h  -  KVocTrain configuration dialog

                             -------------------
    begin                : Fri Mar 25 2005

    copyright            : (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KVOCTRAINPREFS_H
#define KVOCTRAINPREFS_H

#include <kconfigdialog.h>

/**
  *@author Peter Hedlund
*/

class GeneralOptions;
class LanguageOptions;
class ViewOptions;
class PasteOptions;
class QueryOptions;
class ThresholdOptions;
class BlockOptions;

class LangSet;
class kvoctrainDoc;
class QueryManager;
class KComboBox;

class KVocTrainPrefs : public KConfigDialog
{
  Q_OBJECT

public:
  /**
   * Creates a configuration dialog.
   */
  KVocTrainPrefs(LangSet & ls, kvoctrainDoc * doc, KComboBox * lessons, QueryManager * m, QWidget *parent=0,
    const char *name=0, KConfigSkeleton *config=0, DialogType dialogType=IconList,
    int dialogButtons=Default|Ok|Apply|Cancel|Help, ButtonCode defaultButton=Ok, bool modal=false);

  /**
   * calls showPage() to display a particular page in the dialog.
   * @param  page The page to display
   */
  void selectPage(int page);

protected slots:
  /**
   * Called when the user clicks Apply or OK.
   */
  void updateSettings();
  /**
   * Updates dialog widgets. Here only used after loading a profile.
   * Profiles only store the settings of the last three pages in the dialog.
   */
  void updateWidgets();
  /**
   * Called when the user clicks Default
   */
  void updateWidgetsDefault();
  /**
   * Called when the user clicks Profiles...
   */
  void slotUser1();

protected:
  /**
   * Returns true if the current state of the dialog is different from the saved settings
   */
  bool hasChanged();
  /**
   * Returns true if the current state of the dialog represents the default settings.
   */
  bool isDefault();

private:
  GeneralOptions * m_generalOptions;
  LanguageOptions * m_languageOptions;
  ViewOptions * m_viewOptions;
  PasteOptions * m_pasteOptions;
  QueryOptions * m_queryOptions;
  ThresholdOptions * m_thresholdOptions;
  BlockOptions * m_blockOptions;

  KConfigSkeleton * m_config;
  LangSet & m_langSet;
  kvoctrainDoc * m_doc;
  KComboBox m_lessons;
  QueryManager * m_queryManager;
};

#endif
