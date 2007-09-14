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

#include <QLayout>
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>

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

#include <keduvocdocument.h>
#include "generaloptions.h"
#include "viewoptions.h"
#include "groupoptions.h"
#include "queryoptions.h"
#include "thresholdoptions.h"
#include "blockoptions.h"
#include "practice/kvtquery.h"
#include "profilesdialog.h"

static const char unapplied[] = I18N_NOOP(
                                    "You have made changes that are not yet applied.\n"
                                    "If you save a profile, those changes will not be included.\n"
                                    "Do you wish to continue?");

KVocTrainPrefs::KVocTrainPrefs(KEduVocDocument *doc, QWidget *parent, const QString &name, KConfigSkeleton *config)
  : KConfigDialog(parent, name, config)
{
    setButtons(Default|Ok|Apply|Cancel|Help|User1);
    setDefaultButton(Ok);
    setModal(true);

    m_doc = doc;

    m_languagePage = 0;
    m_config = config;

    m_generalOptions = new GeneralOptions(0);
    addPage(m_generalOptions, i18n("General"), "kvoctrain", i18n("General Settings"), true);
    connect(m_generalOptions, SIGNAL(widgetModified()), this, SLOT(updateButtons()));

    m_viewOptions = new ViewOptions(0);
    addPage(m_viewOptions, i18n("View"), "view-choose", i18n("View Settings"), true);

    m_queryOptions = new QueryOptions(0);
    addPage(m_queryOptions, i18nc("Configure settings for practicing vocabulary, short title in config dialog.", "Practice"), "run_query", i18nc("Configure settings for practicing vocabulary.", "Practice Settings"), true);

    m_thresholdOptions = new ThresholdOptions(m_doc, 0);
    addPage(m_thresholdOptions, i18n("Thresholds"), "configure", i18n("Threshold Settings"), true);
    connect(m_thresholdOptions, SIGNAL(widgetModified()), this, SLOT(updateButtons()));

    m_blockOptions = new BlockOptions(0);
    addPage(m_blockOptions, i18n("Blocking"), "configure", i18n("Blocking Settings"), true);
    connect(m_blockOptions, SIGNAL(widgetModified()), this, SLOT(updateButtons()));
    connect(m_blockOptions, SIGNAL(blockExpireChanged(bool, bool)), m_thresholdOptions, SLOT(slotBlockExpire(bool, bool)));

    setButtonGuiItem(KDialog::User1, KGuiItem(i18n("&Profiles...")));
    setButtonToolTip(KDialog::User1, i18n("Save/load specific practice settings in/from a profile"));
    setButtonWhatsThis(KDialog::User1, i18n("A Profile is a set of settings (settings related to practices) which you can save/load in order to use again later. This button allows you to see existing profiles, to load a new profile and to save your current settings in a  new profile."));
    connect(this,SIGNAL(user1Clicked()),this,SLOT(slotUser1()));
}

void KVocTrainPrefs::selectLanguagePage()
{
    if (m_languagePage)
        setCurrentPage(m_languagePage);
}

bool KVocTrainPrefs::hasChanged()
{
    return m_generalOptions->hasChanged() |
           m_thresholdOptions->hasChanged() |
           m_blockOptions->hasChanged();
}

bool KVocTrainPrefs::isDefault()
{
    return m_generalOptions->isDefault() &&
           m_thresholdOptions->isDefault() &&
           m_blockOptions->isDefault();
}

void KVocTrainPrefs::updateSettings()
{
    m_generalOptions->updateSettings();
    m_thresholdOptions->updateSettings();
    m_blockOptions->updateSettings();
    emit settingsChanged("");
}

void KVocTrainPrefs::updateWidgetsDefault()
{
    bool bUseDefaults = m_config->useDefaults(true);
    m_generalOptions->updateWidgets();
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


    if ( m_doc == 0 ) {
        kError() << "Preferences do not work with an empty document! (KVocTrainPrefs::slotUser1())" << endl;
    }

    if (showDlg) {
        ProfilesDialog * dlg = new ProfilesDialog(m_doc, this);
        connect(dlg, SIGNAL(profileActivated()), this, SLOT(updateWidgets()));
        dlg->show();
    }
}

void KVocTrainPrefs::updateWidgets()
{
    m_generalOptions->updateWidgets();
    m_queryOptions->updateWidgets();
    m_thresholdOptions->updateWidgets();
    m_blockOptions->updateWidgets();
}

#include "kvoctrainprefs.moc"
