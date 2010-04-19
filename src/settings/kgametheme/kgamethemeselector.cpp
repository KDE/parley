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
#include <KComponentData>
#include <KNS3/DownloadDialog>

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
        void findThemes(const QString &initialSelection);

        // private slots
        void _k_updatePreview();
        void _k_updateThemeList(const QString& strTheme);
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
    ui.getNewButton->setIcon(KIcon("get-hot-new-stuff"));

    //The lineEdit widget holds our theme path for automatic connection via KConfigXT.
    //But the user should not manipulate it directly, so we hide it.
    ui.kcfg_Theme->hide();
    connect(ui.kcfg_Theme, SIGNAL(textChanged(const QString&)), q, SLOT(_k_updateThemeList(const QString&)));

    //Disable KNS button?
    if (knsflags==KGameThemeSelector::NewStuffDisableDownload) {
      ui.getNewButton->hide();
    }

    //Get the last used theme path from the KConfigSkeleton
    KConfigSkeletonItem * configItem = aconfig->findItem("Theme");
    QString lastUsedTheme = configItem->property().toString();

    //Now get our themes into the list widget
    KGlobal::dirs()->addResourceType("gamethemeselector", "data", KGlobal::mainComponent().componentName() + '/' + lookupDirectory + '/');
    findThemes(lastUsedTheme);

    connect(ui.getNewButton, SIGNAL(clicked()), q, SLOT(_k_openKNewStuffDialog()));
}

void KGameThemeSelector::KGameThemeSelectorPrivate::findThemes(const QString &initialSelection)
{
    qDeleteAll(themeMap.values());
    themeMap.clear();

    //Disconnect the themeList as we are going to clear it and do not want previews generated
    ui.themeList->disconnect();
    ui.themeList->clear();
    ui.themeList->setSortingEnabled(true);

    QStringList themesAvailable;
    KGlobal::dirs()->findAllResources("gamethemeselector", "*.desktop", KStandardDirs::Recursive, themesAvailable);

    bool initialFound = false;
    foreach (const QString &file, themesAvailable)
    {
      QString themePath = lookupDirectory + '/' + file;
      KGameTheme* atheme = new KGameTheme(groupName);

      if (atheme->load(themePath)) {
        QString themeName = atheme->themeProperty("Name");
        //Add underscores to avoid duplicate names.
        while (themeMap.contains(themeName))
          themeName += '_';
        themeMap.insert(themeName, atheme);
        QListWidgetItem * item = new QListWidgetItem(themeName, ui.themeList);

        //Find if this is our currently configured theme
        if (themePath==initialSelection) {
          initialFound = true;
          ui.themeList->setCurrentItem(item);
          _k_updatePreview();
        }
      } else {
        delete atheme;
      }
    }

    if (!initialFound)
    {
      // TODO change this if we ever change KGameTheme::loadDefault
      QString defaultPath = "themes/default.desktop";
      foreach(KGameTheme* theme, themeMap)
      {
        if (theme->path().endsWith(defaultPath))
        {
          const QList<QListWidgetItem *> itemList = ui.themeList->findItems(theme->themeProperty("Name"), Qt::MatchExactly);
          // never can be != 1 but better safe than sorry
          if (itemList.count() == 1)
          {
            ui.themeList->setCurrentItem(itemList.first());
            _k_updatePreview();
          }
        }
      }
    }

    //Reconnect the themeList
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

void KGameThemeSelector::KGameThemeSelectorPrivate::_k_updateThemeList(const QString& strTheme)
{
    //find theme and set selection to the current theme; happens when pressing "Default"
    QListWidgetItem * currentItem = ui.themeList->currentItem();
    if(!currentItem || themeMap.value(currentItem->text())->fileName() != strTheme)
    {
        for(int i = 0; i < ui.themeList->count(); i++)
        {
            if(themeMap.value(ui.themeList->item(i)->text())->fileName() == strTheme)
            {
                ui.themeList->setCurrentItem(ui.themeList->item(i));
                break;
            }
        }
    }
}

void KGameThemeSelector::KGameThemeSelectorPrivate::_k_openKNewStuffDialog()
{
    KNS3::DownloadDialog dialog( q );
    dialog.exec();
    if ( !dialog.changedEntries().isEmpty() )
        findThemes( ui.kcfg_Theme->text() );
}

#include "kgamethemeselector.moc"
