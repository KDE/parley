/***************************************************************************
             kvoctrainprefs.cpp  -  KVocTrain configuration dialog

                             -------------------
    begin                : Fri Mar 25 2005

    copyright            : (C) 2005 Peter Hedlund <peter@peterandlinda.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qwidgetstack.h>
#include <qlayout.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qlistview.h>
#include <qlabel.h>

#include <klocale.h>
#include <kstandarddirs.h>
#include <kiconloader.h>
#include <krestrictedline.h>
#include <kmessagebox.h>
#include <kpushbutton.h>
#include <kdebug.h>
#include <kconfigskeleton.h>
#include <kcolorbutton.h>
#include <kfontrequester.h>
#include <kcombobox.h>

#include "kvoctrainprefs.h"
#include "kvoctraindoc.h"
#include "generaloptions.h"
#include "languageoptions.h"
#include "viewoptions.h"
#include "pasteoptions.h"
#include "groupoptions.h"
#include "queryoptions.h"
#include "thresholdoptions.h"
#include "blockoptions.h"
#include "langset.h"
#include "QueryManager.h"
#include "profilesdialog.h"

static const char unapplied[] = I18N_NOOP(
    "You have made changes that are not yet applied.\n"
    "If you save a profile, those changes will not be included.\n"
    "Do you wish to continue?");

KVocTrainPrefs::KVocTrainPrefs(LangSet & ls, kvoctrainDoc * doc, KComboBox * lessons, QueryManager * m, QWidget *parent,
  const char *name,  KConfigSkeleton *config, DialogType dialogType, int /*dialogButtons*/, ButtonCode /*defaultButton*/,
  bool /*modal*/)
  : KConfigDialog(parent, name, config, dialogType, Default|Ok|Apply|Cancel|Help|User1, Ok, true), m_langSet(ls)
{
  m_config = config;
  m_queryManager = m;

  m_generalOptions = new GeneralOptions(0, "General Settings");
  addPage(m_generalOptions, i18n("General"), "kvoctrain", i18n("General Settings"), true);

  m_languageOptions = new LanguageOptions(m_langSet, 0, "Language Settings");
  addPage(m_languageOptions, i18n("Languages"), "set_language", i18n("Language Settings"), true);
  connect(m_languageOptions, SIGNAL(widgetModified()), this, SLOT(updateButtons()));

  m_viewOptions = new ViewOptions(0, "View Settings");
  addPage(m_viewOptions, i18n("View"), "view_choose", i18n("View Settings"), true);

  m_pasteOptions = new PasteOptions(m_langSet, doc, 0, "Copy & Paste Settings");
  addPage(m_pasteOptions, i18n("Copy & Paste"), "editpaste", i18n("Copy & Paste Settings"), true);
  connect(m_pasteOptions, SIGNAL(widgetModified()), this, SLOT(updateButtons()));

  m_queryOptions = new QueryOptions(0, "Query Settings");
  addPage(m_queryOptions, i18n("Query"), "run_query", i18n("Query Settings"), true);

  m_thresholdOptions = new ThresholdOptions(lessons, m_queryManager, 0, "Threshold Settings");
  addPage(m_thresholdOptions, i18n("Thresholds"), "configure", i18n("Threshold Settings"), true);
  connect(m_thresholdOptions, SIGNAL(widgetModified()), this, SLOT(updateButtons()));

  m_blockOptions = new BlockOptions(0, "Blocking Settings");
  addPage(m_blockOptions, i18n("Blocking"), "configure", i18n("Blocking Settings"), true);
  connect(m_blockOptions, SIGNAL(widgetModified()), this, SLOT(updateButtons()));
  connect(m_blockOptions, SIGNAL(blockExpireChanged(bool, bool)), m_thresholdOptions, SLOT(slotBlockExpire(bool, bool)));

  setButtonGuiItem(KDialogBase::User1, KGuiItem(i18n("&Profiles...")));
}

void KVocTrainPrefs::selectPage(int index)
{
  showPage(index);
}

bool KVocTrainPrefs::hasChanged()
{
  return m_pasteOptions->hasChanged() |
         m_languageOptions->hasChanged() |
         m_thresholdOptions->hasChanged() |
         m_blockOptions->hasChanged();
}

bool KVocTrainPrefs::isDefault()
{
  return m_pasteOptions->isDefault() &&
         m_languageOptions->isDefault() &&
         m_thresholdOptions->isDefault() &&
         m_blockOptions->isDefault();
}

void KVocTrainPrefs::updateSettings()
{
  m_pasteOptions->updateSettings();
  m_languageOptions->updateSettings();
  m_thresholdOptions->updateSettings();
  m_blockOptions->updateSettings();
  emit settingsChanged();
}

void KVocTrainPrefs::updateWidgetsDefault()
{
  bool bUseDefaults = m_config->useDefaults(true);
  m_pasteOptions->updateWidgets();
  m_languageOptions->updateWidgets();
  m_thresholdOptions->updateWidgets();
  m_blockOptions->updateWidgets();
  m_config->useDefaults(bUseDefaults);
}

void KVocTrainPrefs::slotUser1()
{
  bool showDlg = true;
  if (hasChanged())
    if (KMessageBox::Yes != KMessageBox::questionYesNo(this, i18n(unapplied), i18n("Unapplied Changes")))
      showDlg = false;

  if (showDlg)
  {
    ProfilesDialog * dlg = new ProfilesDialog(m_queryManager, this, 0, true);
    connect(dlg, SIGNAL(profileActivated()), this, SLOT(updateWidgets()));
    dlg->show();
  }
}

void KVocTrainPrefs::updateWidgets()
{
  m_queryOptions->updateWidgets();
  m_thresholdOptions->updateWidgets();
  m_blockOptions->updateWidgets();
}

#include "kvoctrainprefs.moc"
