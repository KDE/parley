/*
    Copyright (C) 2007 Mauricio Piacentini   <mauricio@tabuleiro.com>
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

#include "kgametheme.h"

#include <KStandardDirs>
#include <KConfig>
#include <KConfigGroup>
#include <KDebug>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QMap>
#include <QtGui/QPixmap>

class KGameThemePrivate
{
    public:
        KGameThemePrivate() : loaded(false) {}

        QMap<QString, QString> themeproperties;
        QString fullPath; ///< Full path e.g. "/opt/kde/share/apps/appname/default.desktop"
        QString fileName; ///< just e.g. "default.desktop"
        QString graphics; ///< The full path of the svg file
        QPixmap preview;
        QString prefix; ///< Filepath of the .desktop file without the filename e.g. "/opt/kde/share/apps/appname/"
        QString themeGroup;

        bool loaded;
};

KGameTheme::KGameTheme(const QString &themeGroup)
    : d(new KGameThemePrivate)
{
    d->themeGroup = themeGroup;
    //KGlobal::dirs()->addResourceType("gametheme", KStandardDirs::kde_default("data") + KGlobal::mainComponent().componentName());
}

KGameTheme::~KGameTheme() {
    delete d;
}

bool KGameTheme::loadDefault()
{
    return load("themes/default.desktop"); //TODO make this editable to match custom directories.
}

#define kThemeVersionFormat 1

bool KGameTheme::load(const QString &fileName) {
    if( fileName.isEmpty() )
    {
        kDebug(11000) << "Refusing to load theme with no name";
        return false;
    }
    QString filePath = KStandardDirs::locate("appdata", fileName);
    kDebug(11000) << "Attempting to load .desktop at" << filePath;
    if (filePath.isEmpty()) {
        return false;
    }

    // verify if it is a valid file first and if we can open it
    QFile themefile(filePath);
    if (!themefile.open(QIODevice::ReadOnly)) {
        kDebug(11000) << "Could not open .desktop theme file" << filePath;
        return false;
    }
    d->prefix = QFileInfo(themefile).absolutePath() + '/';
    themefile.close();

    KConfig themeconfig(filePath, KConfig::SimpleConfig);
    if (!themeconfig.hasGroup(d->themeGroup))
    {
        kDebug(11000) << "Config group" << d->themeGroup << "does not exist in" << filePath;
        return false;
    }
    KConfigGroup group = themeconfig.group(d->themeGroup);

    //Copy the whole entryMap, so we can inherit generic properties as well, reducing the need to subclass for simple implementations
    d->themeproperties = group.entryMap();

    //Version control
    int themeversion = group.readEntry("VersionFormat",0);
    //Format is increased when we have incompatible changes, meaning that older clients are not able to use the remaining information safely
    if (themeversion > kThemeVersionFormat) {
        return false;
    }

    QString graphName = group.readEntry("FileName");
    //d->graphics = KStandardDirs::locate("appdata", graphName);
    d->graphics = d->prefix + graphName;
    if (d->graphics.isEmpty()) return false;

    // let's see if svg file exists and can be opened
    QFile svgFile( d->graphics );
    if ( !svgFile.open( QIODevice::ReadOnly ) ) {
        kDebug(11000) << "Could not open file" << d->graphics;
        return false;
    }

    QString previewName = group.readEntry("Preview");
    //QString graphicsPath = KStandardDirs::locate("appdata", previewName);
    QString graphicsPath = d->prefix + previewName;
    d->preview = QPixmap(graphicsPath);

    d->fileName = fileName;
    d->fullPath = filePath;
    d->loaded = true;
    return true;
}

QString KGameTheme::property(const QString &key) const
{
    if(!d->loaded)
    {
        kDebug(11000) << "No theme file has been loaded. KGameTheme::load() or KGameTheme::loadDefault() must be called.";
        return QString();
    }
    KConfig themeconfig(path(), KConfig::SimpleConfig);
    KConfigGroup group = themeconfig.group(d->themeGroup);
    return group.readEntry(key, QString());
}

QString KGameTheme::path() const {
    if(!d->loaded)
    {
        kDebug(11000) << "No theme file has been loaded. KGameTheme::load() or KGameTheme::loadDefault() must be called.";
        return QString();
    }
    return d->fullPath;
}

QString KGameTheme::fileName() const {
    if(!d->loaded)
    {
        kDebug(11000) << "No theme file has been loaded. KGameTheme::load() or KGameTheme::loadDefault() must be called.";
        return QString();
    }
    return d->fileName;
}

QString KGameTheme::graphics() const {
    if(!d->loaded)
    {
        kDebug(11000) << "No theme file has been loaded. KGameTheme::load() or KGameTheme::loadDefault() must be called.";
        return QString();
    }
    return d->graphics;
}

QPixmap KGameTheme::preview() const {
    if(!d->loaded)
    {
        kDebug(11000) << "No theme file has been loaded. KGameTheme::load() or KGameTheme::loadDefault() must be called.";
        return QPixmap();
    }
    return d->preview;
}

QString KGameTheme::themeProperty(const QString &key) const {
    if(!d->loaded)
    {
        kDebug(11000) << "No theme file has been loaded. KGameTheme::load() or KGameTheme::loadDefault() must be called.";
        return QString();
    }
    return d->themeproperties[key];
}
