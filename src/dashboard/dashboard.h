/***************************************************************************
    Copyright 2008-2010 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "ui_dashboard.h"
#include "statistics/statisticsmainwindow.h"

#include <KXmlGuiWindow>
#include <QSignalMapper>
#include <QUrl>

static const int N = 50;

class ParleyMainWindow;
class QStandardItemModel;

namespace Practice
{
class ThemedBackgroundRenderer;
class ImageWidget;
}


class CollectionWidget;
class RemoveButton;

class Dashboard : public KXmlGuiWindow
{
    Q_OBJECT
public:
    Dashboard(ParleyMainWindow *parent);
    ~Dashboard();

public slots:
    void slotOpenUrl(const QUrl& url);
    void slotPracticeUrl(const QUrl& url);
    //void slotDoubleClicked(const QModelIndex& index);
    void slotDoubleClickOpen();
    //void updateRecentFilesModel();
    void populateGrid();
    void populateMap();
    void clearGrid();
    void slotPracticeButtonClicked(const QString& urlString);
    void slotRemoveButtonClicked(const QString& urlString);
    void statisticsHandler(QUrl url);
    void remove(QGridLayout *layout, int row, int column, bool deleteWidgets);
    void deleteChildWidgets(QLayoutItem *item);

private slots:
    void setTheme();
    void updateWidgets();
    void backgroundChanged(const QPixmap& pixmap);
    void updateFontColors();
    void updateBackground();

private:
    Ui::Dashboard *m_ui;
    ParleyMainWindow *m_mainWindow;
    StatisticsMainWindow *m_statisticsWidget;
    Practice::ThemedBackgroundRenderer *m_themedBackgroundRenderer;
    Practice::ImageWidget* m_widget;

    QMap<QString, QString> m_recentFilesMap;  // url, name
    QUrl m_openUrl;
    QGridLayout *m_subGridLayout;
    QGridLayout *m_completedGridLayout;
    QList<CollectionWidget *>  m_collectionWidgets; // List of the widgets in the dashboard.

    // The parts of the collections
    QUrl          m_urlArray[N];

    QSignalMapper *m_practiceSignalMapper;  // For the practice buttons
    QSignalMapper *m_removeSignalMapper; // For the remove buttons

    int m_count;
};

#endif
