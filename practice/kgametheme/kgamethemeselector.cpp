/*
    Copyright (C) 2007 Mauricio Piacentini  <mauricio@tabuleiro.com>
    Copyright (C) 2007 Matt Williams   <matt@milliams.com>

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "kgamethemeselector.h"

#include <KLocale>
#include <KStandardDirs>
#include <KConfigSkeleton>
#include <knewstuff2/engine.h>
#include <KComponentData>

#include "ui_kgamethemeselector.h"
#include "kgametheme.h"

class KGameThemeSelector::KGameThemeSelectorPrivate
{
    public:
        KGameThemeSelectorPrivate(KGameThemeSelector* parent) : q(parent) {}
        ~KGameThemeSelectorPrivate() { qDeleteAll(themeMap); }

        KGameThemeSelector* q;

        QMap<QString, KGameTheme*> themeMap;
        Ui::KGameThemeSelectorBase ui;
        QString lookupDirectory;
        QString groupName;

        void setupData(KConfigSkeleton* config, KGameThemeSelector::NewStuffState knsflags);
        void findThemes(QString &initialGroup);

        // private slots
        void _k_updatePreview();
        void _k_openKNewStuffDialog();
};

KGameThemeSelector::KGameThemeSelector(QWidget* parent, KConfigSkeleton * aconfig, KGameThemeSelector::NewStuffState knsflags, const QString &groupName, const QString &directory)
    : QWidget(parent), d(new KGameThemeSelectorPrivate(this))
{
    d->lookupDirectory = directory;
    d->groupName = groupName;
    d->setupData(aconfig, knsflags);
}

KGameThemeSelector::~KGameThemeSelector()
{
    delete d;
}

void KGameThemeSelector::KGameThemeSelectorPrivate::setupData(KConfigSkeleton * aconfig, KGameThemeSelector::NewStuffState knsflags)
{
    ui.setupUi(q);
    
    //Get our currently configured Tileset entry
    KConfig * config = aconfig->config();
    KConfigGroup group = config->group("General");
    QString initialGroup = group.readEntry("Theme"); //Should be someting like "themes/default.desktop"

    //The lineEdit widget holds our bg path, but the user does not manipulate it directly
    ui.kcfg_Theme->hide();
    
    //Disable KNS button?
    if (knsflags==KGameThemeSelector::NewStuffDisableDownload) {
      ui.getNewButton->hide();
    }

    //Now get our tilesets into a list
    KGlobal::dirs()->addResourceType("gamethemeselector", "data", KGlobal::mainComponent().componentName() + '/' + lookupDirectory + '/');
    findThemes(initialGroup);
    connect(ui.getNewButton, SIGNAL(clicked()), q, SLOT(_k_openKNewStuffDialog()));
}

void KGameThemeSelector::KGameThemeSelectorPrivate::findThemes(QString &initialGroup)
{
    
    QStringList themesAvailable;
    KGlobal::dirs()->findAllResources("gamethemeselector", QString("*.desktop"), KStandardDirs::Recursive, themesAvailable);
    QString namestr("Name");
    int numvalidentries = 0;
    qDeleteAll(themeMap.values());
    themeMap.clear();
    //Disconnect the themeList as we are going to clear it and do not want previews generated
    ui.themeList->disconnect();
    ui.themeList->clear();
    
    for (int i = 0; i < themesAvailable.size(); ++i)
    {
      KGameTheme* atheme = new KGameTheme(groupName);
      QString themepath = lookupDirectory + '/' + themesAvailable.at(i);
      if (atheme->load(themepath)) {
        themeMap.insert(atheme->themeProperty(namestr), atheme);
        ui.themeList->addItem(atheme->themeProperty(namestr));
        //Find if this is our currently configured Theme
        if (themepath==initialGroup) {
                    //Select current entry
          ui.themeList->setCurrentRow(numvalidentries);
          _k_updatePreview();
        }
        ++numvalidentries;
      } else {
        delete atheme;
      }
    }
    ui.themeList->sortItems();
    //reconnect the themeList
    connect(ui.themeList, SIGNAL(currentItemChanged ( QListWidgetItem * , QListWidgetItem * )), q, SLOT(_k_updatePreview()));
}

void KGameThemeSelector::KGameThemeSelectorPrivate::_k_updatePreview()
{
    KGameTheme * seltheme = themeMap.value(ui.themeList->currentItem()->text());
        //Sanity checkings. Should not happen.
    if (!seltheme) return;
    if (seltheme->path() == ui.kcfg_Theme->text()) {
        return;
    }
    ui.kcfg_Theme->setText(seltheme->fileName());

    QString authstr("Author");
    QString contactstr("AuthorEmail");
    QString descstr("Description");
    QString emailstr;
    if (!seltheme->themeProperty(contactstr).isEmpty() ) {
        emailstr = QString("<a href=\"mailto:%1\">%1</a>").arg(seltheme->themeProperty(contactstr));
    }

    ui.themeAuthor->setText(seltheme->themeProperty(authstr));
    ui.themeContact->setText(emailstr);
    ui.themeDescription->setText(seltheme->themeProperty(descstr));

    //Draw the preview
    QPixmap pix(seltheme->preview());
    ui.themePreview->setPixmap(pix.scaled(ui.themePreview->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

}

void KGameThemeSelector::KGameThemeSelectorPrivate::_k_openKNewStuffDialog()
{
    KNS::Entry::List entries = KNS::Engine::download();
    //rescan themes directory
    QString currentthemepath = ui.kcfg_Theme->text();
    if (entries.size()>0) findThemes(currentthemepath);
    //Needed as the static KNS constructor made copies
    qDeleteAll(entries);
}

#include "kgamethemeselector.moc"
