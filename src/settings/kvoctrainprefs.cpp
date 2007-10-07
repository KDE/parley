/***************************************************************************
             kvoctrainprefs.cpp  -  Parley configuration dialog

                             -------------------
    begin         : Fri Mar 25 2005

    copyright     : (C) 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kvoctrainprefs.h"

#include "generaloptions.h"
#include "viewoptions.h"
#include <KConfigSkeleton>

static const char unapplied[] = I18N_NOOP(
                                    "You have made changes that are not yet applied.\n"
                                    "If you save a profile, those changes will not be included.\n"
                                    "Do you wish to continue?");

KVocTrainPrefs::KVocTrainPrefs(KEduVocDocument *doc, QWidget *parent, const QString &name, KConfigSkeleton *config)
  : KConfigDialog(parent, name, config)
{
    setButtons(Default|Ok|Apply|Cancel|Help);
    setDefaultButton(Ok);
    setModal(true);

    m_doc = doc;
    m_config = config;

    m_generalOptions = new GeneralOptions(0);
    addPage(m_generalOptions, i18nc("title:window general settings", "General"), "kvoctrain", i18n("General Settings"), true);
    connect(m_generalOptions, SIGNAL(widgetModified()), this, SLOT(updateButtons()));

    m_viewOptions = new ViewOptions(0);
    addPage(m_viewOptions, i18n("View"), "view-choose", i18n("View Settings"), true);
}

bool KVocTrainPrefs::hasChanged()
{
    return m_generalOptions->hasChanged();
}

bool KVocTrainPrefs::isDefault()
{
    return m_generalOptions->isDefault();
}

void KVocTrainPrefs::updateSettings()
{
    m_generalOptions->updateSettings();
    emit settingsChanged("");
}

void KVocTrainPrefs::updateWidgetsDefault()
{
    m_config->useDefaults(true);
    m_generalOptions->updateWidgets();
    m_config->useDefaults(false);
}

void KVocTrainPrefs::updateWidgets()
{
    m_generalOptions->updateWidgets();
}

#include "kvoctrainprefs.moc"
