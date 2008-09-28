/***************************************************************************
    Copyright 2008 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "welcomescreen.h"
#include "buttondelegate.h"
#include "parley.h"
#include "parleydocument.h"

#include <KMimeType>

#include <QStandardItemModel>

WelcomeScreen::WelcomeScreen(ParleyApp *parent)
 : QWidget(parent), m_parleyApp(parent)
{
    ui = new Ui::WelcomeScreen();
    ui->setupUi(this);

    QColor fgColor = palette().text().color();

    QString css = QString::fromUtf8(
       "#recentLabel {"
        "   font-weight: bold;"
        "   border-bottom: 1px solid %1;"
        "}"

        "QListView {"
        "   background-color: transparent;"
        "}"
        )
        .arg(fgColor.name());
    setStyleSheet(css);

    ui->iconLabel->setPixmap(KIcon("parley").pixmap(128, 128));
    ui->newButton->setIcon(KIcon("document-new"));
    ui->openButton->setIcon(KIcon("document-open"));
    ui->ghnsButton->setIcon(KIcon("get-hot-new-stuff"));

    m_recentFilesModel = new QStandardItemModel(this);
    updateRecentFilesModel();
    ui->recentFiles->setModel(m_recentFilesModel);
    ui->recentFiles->setSelectionMode(QAbstractItemView::NoSelection);

    ButtonDelegate* delegate = new ButtonDelegate(ui->recentFiles, this);
    ui->recentFiles->setItemDelegate(delegate);

    ParleyDocument* doc = m_parleyApp->parleyDocument();
    connect(ui->newButton, SIGNAL(clicked()), doc, SLOT(slotFileNew()));
    connect(ui->openButton, SIGNAL(clicked()), doc, SLOT(slotFileOpen()));
    connect(ui->ghnsButton, SIGNAL(clicked()), doc, SLOT(slotGHNS()));
    connect(ui->recentFiles, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(slotDoubleClicked(const QModelIndex&)));
}

void WelcomeScreen::updateRecentFilesModel()
{
    KConfig parleyConfig("parleyrc");
    KConfigGroup recentFilesGroup(&parleyConfig, "Recent Files");

    m_recentFilesModel->clear();
    for (int i = recentFilesGroup.keyList().count() / 2; i > 0 ; i--) {
        QString urlString = recentFilesGroup.readPathEntry("File"+QString::number(i), QString());
        QString nameString = recentFilesGroup.readEntry("Name"+QString::number(i), QString());

        KUrl url(urlString);

        QStandardItem* item = new QStandardItem;
        item->setEditable(false);
        item->setText(nameString+" ("+url.pathOrUrl()+')');
        item->setToolTip(nameString+" ("+url.pathOrUrl()+')');
        item->setData(QVariant(url), Qt::UserRole);

        QString iconName = KMimeType::iconNameForUrl(url);
        item->setIcon(KIcon(iconName));

        m_recentFilesModel->appendRow(item);
    }
}

void WelcomeScreen::slotOpenUrl(const KUrl& url)
{
    m_parleyApp->parleyDocument()->open(url);
}

void WelcomeScreen::slotDoubleClicked(const QModelIndex& index)
{
    KUrl url = index.data(Qt::UserRole).toUrl();
    slotOpenUrl(url);
}

void WelcomeScreen::slotPracticeUrl(const KUrl & url)
{
    m_parleyApp->parleyDocument()->open(url);
    m_parleyApp->startPractice();
}

#include "welcomescreen.moc"
