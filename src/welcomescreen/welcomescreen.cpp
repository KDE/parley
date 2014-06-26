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

#include "welcomescreen.h"
#include "buttondelegate.h"
#include "parleymainwindow.h"
#include "parleydocument.h"
#include "practice/themedbackgroundrenderer.h"
#include "practice/imagewidget.h"
#include "statistics/statisticsmainwindow.h"
#include "statistics/statisticsmodel.h"
#include <QSignalMapper>
#include <KMimeType>

#include <QStandardItemModel>
#include <QTimer>
#include <QTime>
#include <QDebug>

#include <QtGui>
#include <Qt>

QColor gradeColor[11];


// Size constants for the collection widgets
//int COLLWIDTH   = 170; // Width in pixels of a collection widget
int COLLWIDTH   = 140; // Width in pixels of a collection widget
int COLLHEIGHT1 = 250; // Height in pixels of a collection widget not yet fully learned
int COLLHEIGHT2 = 100; // Height in pixels of a collection widget fully learned


// ================================================================
//                        private classes



// The RemoveButton is a button that the user can press in a collection to
// remove the collection from the word bank.


class RemoveButton : public QPushButton
{
public:
    RemoveButton(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);
};


RemoveButton::RemoveButton(QWidget *parent)
  : QPushButton(parent)
{
}


void RemoveButton::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPen pen(QColor(255,255,255));
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QBrush brush(QColor(49,54,59));
    painter.setBrush(brush);

    painter.drawEllipse(1, 1, height() - 1, height() - 1);
    painter.setFont( QFont( "Helvetica", 7, QFont::Bold, false));
    painter.drawText(2, 1, height() - 2, height() - 1, Qt::AlignCenter, "x");
}


// ----------------------------------------------------------------


class GradeReferenceWidget : public QWidget
{
public:
    GradeReferenceWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);

};

GradeReferenceWidget::GradeReferenceWidget(QWidget *parent)
  : QWidget(parent)
{
}

void GradeReferenceWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    const int legendWidth = this->width();
    const int legendHeight = this->height();
    const int legendOffsetY = 0;
    const int legendOffsetX = (this->width() / 2) - (legendWidth / 2);;
    const int gradeBarWidth = this->width()/8;
    //const int alphaValueIncrement = 35;
    QRect roundedRect(0 + legendOffsetX, 0 + legendOffsetY, legendWidth, legendHeight);
    roundedRect.adjust(1, 1, -1, -1);
    QPainterPath roundedPath;
    roundedPath.addRoundedRect(roundedRect, 2.0, 2.0);

    for (int i = 7; i >= 0; --i) {
        QRectF barElement(0 + legendOffsetX + (7 - i) * gradeBarWidth,
			  0 + legendOffsetY,
			  gradeBarWidth, legendHeight);
        QPainterPath barElementPath;
        barElementPath.addRect(barElement);
        QPainterPath barElementIntersectedPath = roundedPath.intersected(barElementPath);
        QColor color = gradeColor[i];
        painter.setBrush(QBrush(color));
        painter.drawPath(barElementIntersectedPath);
    }
}


// ----------------------------------------------------------------


class BarWidget : public QWidget
{
public:
    BarWidget();
    BarWidget(int [], int, int);

protected:
    void paintEvent(QPaintEvent *);

private:
    int dueWords[8];
    int totalDueWords;
    int percentageCompleted;
};

BarWidget::BarWidget()
{

}

BarWidget::BarWidget(int dueWords[], int totalDueWords, int percentageCompleted)
{
    QPalette palette(BarWidget::palette());
    palette.setColor(backgroundRole(), Qt::white);
    setPalette(palette);
    for (int i = 0; i < 8; i++) {
        this->dueWords[i] = dueWords[i];
    }
    this->totalDueWords = totalDueWords;
    this->percentageCompleted = percentageCompleted;
}

void BarWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    const int legendWidth = COLLWIDTH - 10;
    const int legendHeight = 20;
    const int legendOffsetY = 0;
    const int legendOffsetX = 0;
    //const int alphaValueIncrement = 35;

    int gradeBarWidth[9];
    gradeBarWidth[8] = 0;
    int gradeBarOffset[9];
    gradeBarOffset[8] = 0;
    if (percentageCompleted < 100) {
        for(int j = 7; j >= 0; j--) {
            gradeBarWidth[j] = (float)(dueWords[j]) / (float)(totalDueWords) * legendWidth;
            gradeBarOffset[j] = gradeBarOffset[j+1] + gradeBarWidth[j+1];
        }
    }
    else {
        for(int j = 6; j >= 0; j--) {
            gradeBarWidth[j] = 0;
            gradeBarOffset[j] = legendWidth;
        }
        gradeBarWidth[7] = legendWidth;
        gradeBarOffset[7] = 0;
    }
    if (percentageCompleted < 100 && totalDueWords == 0) {
        for(int j = 6; j >= 0; j--) {
            gradeBarWidth[j] = 0;
            gradeBarOffset[j] = legendWidth;
        }
        gradeBarWidth[7] = legendWidth;
        gradeBarOffset[7] = 0;
    }

    QPen penBar(QColor(255,255,255));
    painter.setPen(penBar);
    QRect roundedRect(0, 0, legendWidth, legendHeight);
    roundedRect.adjust(1, 1, -1, -1);
    QPainterPath roundedPath;
    roundedPath.addRoundedRect(roundedRect, 8.0, 8.0);

    for (int i = 7; i >= 0; i--) {
        QRectF barElement(0 + legendOffsetX + gradeBarOffset[i], 0 + legendOffsetY, gradeBarWidth[i], legendHeight);
        QPainterPath barElementPath;
        barElementPath.addRect(barElement);
        QPainterPath barElementIntersectedPath = roundedPath.intersected(barElementPath);
        QColor color;
        if (totalDueWords == 0 && percentageCompleted < 100) {
            color = QColor(0, 0, 0, 128);
        }
        else {
            color = gradeColor[i];
        }
        painter.setBrush(QBrush(color));
        painter.drawPath(barElementIntersectedPath);
    }
    QPen pen(QColor(255,255,255));
    painter.setPen(pen);
    if (percentageCompleted < 100) {
        painter.drawText(0, 0, legendWidth, 20, Qt::AlignCenter,
			 i18np("%1 word due", "%1 words due", totalDueWords));
    }
    else {
        painter.drawText(0, 0, legendWidth, 20, Qt::AlignCenter, i18n("Fully learned"));
    }
}

int WelcomeScreen::randInt(int low, int high)
{
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}


// ================================================================
//                         class WelcomeScreen


int ROWSIZE = 4;      // Number of collection widgets (+ 1 initial spacerItem) per row


WelcomeScreen::WelcomeScreen(ParleyMainWindow *parent)
    : KXmlGuiWindow(parent)
    , m_mainWindow(parent)
{
    // KXmlGui
    setXMLFile("welcomescreenui.rc");
    setObjectName("WelcomeScreen");

    m_widget = new Practice::ImageWidget(this);
    m_widget->setScalingEnabled(false, false);
    m_widget->setKeepAspectRatio(Qt::IgnoreAspectRatio);
    m_widget->setFadingEnabled(false);

    ui = new Ui::WelcomeScreen();
    ui->setupUi(m_widget);
    setCentralWidget(m_widget);
    signalMapper = new QSignalMapper(this);
    signalMapper2 = new QSignalMapper(this);

    gradeColor[0] = QColor(25,38,41);
    gradeColor[1] = QColor(25,38,41,64);// Need 8 colors, so find a suitable color for this grade, currently gray.
    gradeColor[2] = QColor(237,21,21);
    gradeColor[3] = QColor(246,116,0);
    gradeColor[4] = QColor(201,206,59);
    gradeColor[5] = QColor(28,220,154);
    gradeColor[6] = QColor(17,209,22);
    gradeColor[7] = QColor(61,174,253);
    gradeColor[8] = QColor(255,221,217);//These two are placeholders for the wordcloud background color.
    gradeColor[9] = QColor(238,232,213);//

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    QFont font = ui->recentLabel->font();
    font.setBold(true);
    ui->recentLabel->setFont(font);
    font = ui->completedLabel->font();
    font.setBold(true);
    ui->completedLabel->setFont(font);

    ui->newButton->setIcon(KIcon("document-new"));
    ui->openButton->setIcon(KIcon("document-open"));
    ui->ghnsButton->setIcon(KIcon("get-hot-new-stuff"));
    GradeReferenceWidget *gradeReferenceWidget = new GradeReferenceWidget();
    gradeReferenceWidget->setMinimumSize(m_widget->width(), 50);
    ui->gridLayout->addWidget(gradeReferenceWidget, 2, 0, 1, ROWSIZE, Qt::AlignCenter);

    m_subGridLayout = new QGridLayout();
    m_subGridLayout->setHorizontalSpacing(50);
    m_subGridLayout->setVerticalSpacing(30);
    ui->gridLayout->addLayout(m_subGridLayout, 5, 0, 1, 4);

    m_completedGridLayout = new QGridLayout();
    m_completedGridLayout->setHorizontalSpacing(50);
    m_completedGridLayout->setVerticalSpacing(30);
    ui->gridLayout->addLayout(m_completedGridLayout, 9, 0, 1, 4);

    populateMap();
    populateGrid();

    ParleyDocument* doc = m_mainWindow->parleyDocument();
    connect(ui->newButton,  SIGNAL(clicked()), doc, SLOT(slotFileNew()));
    connect(ui->openButton, SIGNAL(clicked()), doc, SLOT(slotFileOpen()));
    connect(ui->ghnsButton, SIGNAL(clicked()), doc, SLOT(slotGHNS()));

    connect(signalMapper, SIGNAL(mapped(const QString &)), this, SLOT(slotPracticeButtonClicked(const QString &)));
    connect(signalMapper2, SIGNAL(mapped(const QString &)), this, SLOT(slotRemoveButtonClicked(const QString &)));

    KConfigGroup cfg(KSharedConfig::openConfig("parleyrc"), objectName());
    applyMainWindowSettings(cfg);

    m_themedBackgroundRenderer = new Practice::ThemedBackgroundRenderer(this, "startpagethemecache.bin");

    connect(Prefs::self(), SIGNAL(configChanged()), this, SLOT(setTheme()));
    setTheme();

    connect(m_themedBackgroundRenderer, SIGNAL(backgroundChanged(QPixmap)), this, SLOT(backgroundChanged(QPixmap)));
    connect(m_widget, SIGNAL(sizeChanged()), this, SLOT(updateBackground()));

    /*statisticsWidget = new StatisticsMainWindow(m_mainWindow->parleyDocument()->document(), m_mainWindow);
    for (int i = 0; i < m_count; i++)
        statisticsHandler(urlArray[i]); */ //Used to get the statistics for each Collection. TODO find a better way.
}

WelcomeScreen::~WelcomeScreen()
{
    KConfigGroup cfg(KSharedConfig::openConfig("parleyrc"), objectName());
    saveMainWindowSettings(cfg);
}


void WelcomeScreen:: clearGrid()
{
   remove(m_subGridLayout,
	  m_subGridLayout->rowCount() - 1, m_subGridLayout->columnCount() - 1,
	  true);
   remove(m_completedGridLayout,
	  m_completedGridLayout->rowCount() - 1, m_completedGridLayout->columnCount() - 1,
	  true);
}

/**
 * Helper function. Removes all layout items within the given @a layout
 * which either span the given @a row or @a column. If @a deleteWidgets
 * is true, all concerned child widgets become not only removed from the
 * layout, but also deleted.
 */
void WelcomeScreen::remove(QGridLayout *layout, int row, int column, bool deleteWidgets) {
    // We avoid usage of QGridLayout::itemAtPosition() here to improve performance.
    for (int i = layout->count() - 1; i >= 0; i--) {
        int r, c, rs, cs;
        layout->getItemPosition(i, &r, &c, &rs, &cs);
        if ((r + rs - 1 <= row) || (c + cs - 1 <= column)) {
            // This layout item is subject to deletion.
            QLayoutItem *item = layout->takeAt(i);
            if (deleteWidgets) {
                deleteChildWidgets(item);
            }
            delete item;
        }
    }
}

/**
 * Helper function. Deletes all child widgets of the given layout @a item.
 */
void WelcomeScreen::deleteChildWidgets(QLayoutItem *item)
{
    if (item->layout()) {
        // Process all child items recursively.
        for (int i = 0; i < item->layout()->count(); i++) {
            deleteChildWidgets(item->layout()->itemAt(i));
        }
    }
    delete item->widget();
}

void WelcomeScreen::populateMap()
{
    KConfig parleyConfig("parleyrc");
    KConfigGroup recentFilesGroup(&parleyConfig, "Recent Files");
    for (int i = recentFilesGroup.keyList().count() / 2; i > 0 ; i--) {
        QString urlString = recentFilesGroup.readPathEntry("File" + QString::number(i), QString());
        QString nameString = recentFilesGroup.readEntry("Name" + QString::number(i), QString());
        recentFilesMap.insert(urlString,nameString);
    }
}

void WelcomeScreen::populateGrid()
{
    int j = 0, k = 0, jc = 0, kc = 0;

    QMapIterator<QString, QString> it(recentFilesMap);
    while (it.hasNext()) {
        it.next();
        QString urlString  = it.key();
        QString nameString = it.value();

        // FIXME: This is only for testing purposes. We need a way to get the
        //        grades and words due for every document.
        int dueWords[8]; //Due words categorized in grades.
        int firstGrade = randInt(0,7); //This is done for vanity purposes only, giving due word values to only two grades for now.
        int secondGrade = randInt(0,7);
        int totalDueWords = 0;
        for (int x = 0; x < 8; x++) {
            if (x == firstGrade || x == secondGrade) {
                dueWords[x] = randInt(0,20);
                totalDueWords += dueWords[x];
            }
            else {
                dueWords[x] = 0;
            }
        }
        int percentageCompleted = randInt(98,100); //To test randomnly for Complete Collections. Again to be obtained from document.

        KUrl url(urlString);
        urlArray[k] = url;
        if (percentageCompleted != 100) {
            if (j % ROWSIZE == 0) {
                m_subGridLayout->addItem(new QSpacerItem(50,1), j / ROWSIZE, 0);
                j++;
            }
        }
        else {
            if (jc % ROWSIZE == 0) {
                m_completedGridLayout->addItem(new QSpacerItem(50,1), jc / ROWSIZE, 0);
                jc++;
            }
        }

	// backWidget is the main widget for one collection
        QWidget* backWidget = new QWidget;
        QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
        effect->setBlurRadius(50);
        backWidget->setGraphicsEffect(effect);
        QPalette palette = backWidget->palette();
        palette.setColor(QPalette::Background, Qt::white);
        backWidget->setAutoFillBackground(true);
        backWidget->setPalette(palette);
        if (percentageCompleted != 100) {
                backWidget->setFixedSize(COLLWIDTH, COLLHEIGHT1);
                m_subGridLayout->addWidget(backWidget, j / ROWSIZE, j % ROWSIZE);
        }
        else {
                backWidget->setFixedSize(COLLWIDTH, COLLHEIGHT2);
                m_completedGridLayout->addWidget(backWidget, jc / ROWSIZE, jc % ROWSIZE);
        }

	// vBoxLayout is the main vertical layout for one collection
        QVBoxLayout* vBoxLayout = new QVBoxLayout();
        vBoxLayout->setAlignment(Qt::AlignCenter);
        backWidget->setLayout(vBoxLayout);

	// One collection is laid out vertically like this:
	//  1. nameLabel:  contains the name of the collection
	//  2. wordcloud:  a wordcloud generated from the words in the collection
	//  3. barWidget:  a visual bar showing the training status of the words in the collection
	//  4. hBoxLayout: a horizontal row of pushbuttons for delete, practice, etc
        nameLabel[k] = new QLabel(nameString);
        vBoxLayout->addWidget(nameLabel[k]);
        wordCloud[k] = new QWidget;
        palette = wordCloud[k]->palette();
        int y = randInt(8, 9);
        palette.setColor(QPalette::Background, gradeColor[y]);
        wordCloud[k]->setAutoFillBackground(true);
        wordCloud[k]->setPalette(palette);
        wordCloud[k]->setFixedSize(COLLWIDTH - 10, COLLHEIGHT1 - COLLHEIGHT2 + 10);
        if (percentageCompleted != 100) {
            vBoxLayout->addWidget(wordCloud[k]);
        }

        BarWidget *barWidget = new BarWidget(dueWords, totalDueWords, percentageCompleted);
        barWidget->setFixedSize(COLLWIDTH - 10, 20);
        vBoxLayout->addWidget(barWidget);
        if (totalDueWords == 0 && percentageCompleted < 100) {
            practiceButton[k] = new QPushButton(i18n("Practice Anyway"));
        }
        else {
            practiceButton[k] = new QPushButton(i18n("Practice"));
        }
        practiceButton[k]->setStyleSheet("QPushButton {border: none; margin: 0px;   padding: 0px;}");

	// hBoxLayout is the horizontal layout for the bottom line in the
	// collection widget: delete button, practice button, etc
        QHBoxLayout *hBoxLayout = new QHBoxLayout();
        vBoxLayout->addLayout(hBoxLayout);
        removeButton[k] = new RemoveButton();
        removeButton[k]->setFixedSize(20, 20);
        hBoxLayout->setAlignment(removeButton[k], Qt::AlignLeft | Qt::AlignVCenter);
        hBoxLayout->setAlignment(practiceButton[k], Qt::AlignCenter);
        hBoxLayout->addWidget(removeButton[k]);
        hBoxLayout->addWidget(practiceButton[k]);
        hBoxLayout->addItem(new QSpacerItem(20, 20));

        signalMapper->setMapping(practiceButton[k], urlString);
        connect(practiceButton[k], SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper2->setMapping(removeButton[k], urlString);
        connect(removeButton[k], SIGNAL(clicked()), signalMapper2, SLOT(map()));

        if (percentageCompleted != 100) {
            j++;
        }
        else {
            jc++;
            kc++;
        }
        k++;
    }

    m_count = k;
    m_completedGridLayout->addItem(new QSpacerItem(50, 1, 
						   QSizePolicy::Expanding, QSizePolicy::Fixed),
				   m_completedGridLayout->rowCount() - 1,
				   m_completedGridLayout->columnCount());
    m_subGridLayout->addItem(new QSpacerItem(50,1,QSizePolicy::Expanding, QSizePolicy::Fixed),
			     m_subGridLayout->rowCount() - 1, m_subGridLayout->columnCount());
    if (k - kc) {
        ui->recentLabel->setText(i18n("Active Collections"));
    } else {
        ui->recentLabel->clear();
    }
    if (kc) {
        ui->completedLabel->setText(i18n("Completed Collections"));
    } else {
        ui->completedLabel->clear();
    }
}

void WelcomeScreen::statisticsHandler(KUrl url)
{
#if 1
    Q_UNUSED(url);
#else
    if (!m_mainWindow->parleyDocument()->open(url)) {
        return;
    }

    // Find due words. TODO find a better way.
    m_mainWindow->m_sessionManager.setDocument(m_mainWindow->parleyDocument()->document());

    qDebug()<<"Session Manager, allEntryCount="<<m_mainWindow->m_sessionManager.allDueEntryCount();
    statisticsWidget->setDocument(m_mainWindow->parleyDocument()->document());

    // Find the percentage completion, to categorize as active or completed collection.
    QModelIndex index = statisticsWidget->statisticsModel()->index(0, 2, QModelIndex());
    qDebug() << "Percentage:" << index.data(StatisticsModel::TotalPercent).toInt();
#endif
}

void WelcomeScreen::slotOpenUrl(const KUrl& url)
{
    if (!m_mainWindow->parleyDocument()->open(url)) {
        return;
    }
    m_mainWindow->showEditor();
}

void WelcomeScreen::slotPracticeButtonClicked(const QString& urlString)
{
    KUrl url(urlString);
    m_openUrl = url;
    QTimer::singleShot(0, this, SLOT(slotDoubleClickOpen()));
}

void WelcomeScreen::slotRemoveButtonClicked(const QString& urlString)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, i18n("Remove"),
				  i18n("Are you sure you want to remove this collection?"),
				  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        recentFilesMap.remove(urlString);
        m_mainWindow->removeRecentFile(KUrl(urlString));
        clearGrid();
        populateGrid();
    }
}

void WelcomeScreen::slotDoubleClickOpen()
{
    slotPracticeUrl(m_openUrl);
}

void WelcomeScreen::slotPracticeUrl(const KUrl & url)
{
    if (!m_mainWindow->parleyDocument()->open(url)) {
        return;
    }
    m_mainWindow->showPracticeConfiguration();
}

void WelcomeScreen::backgroundChanged(const QPixmap &pixmap)
{
    m_widget->setPixmap(pixmap);
}

void WelcomeScreen::setTheme()
{
    m_themedBackgroundRenderer->setTheme(Prefs::theme());
    updateFontColors();
    updateBackground();
    m_widget->setContentsMargins(m_themedBackgroundRenderer->contentMargins());
}

void WelcomeScreen::updateFontColors()
{
    QPalette p(QApplication::palette());
    QColor c = m_themedBackgroundRenderer->fontColor("Start", p.color(QPalette::Active, QPalette::WindowText));
    p.setColor(QPalette::Base, Qt::transparent);
    p.setColor(QPalette::Text, c);
    p.setColor(QPalette::WindowText, c);
    m_widget->setPalette(p);
}

void WelcomeScreen::updateBackground()
{
    m_themedBackgroundRenderer->clearRects();
    m_themedBackgroundRenderer->addRect("startbackground", QRect(QPoint(), m_widget->size()));
    QRect headerRect = ui->headingLabel->frameGeometry();
    headerRect.setBottom(ui->recentFiles->frameGeometry().top() - 1);
    m_themedBackgroundRenderer->addRect("startheader", headerRect);
    m_themedBackgroundRenderer->addRect("recentfiles", ui->recentFiles->frameGeometry());
    QPixmap pixmap = m_themedBackgroundRenderer->getScaledBackground();
    if (!pixmap.isNull()) {
        m_widget->setPixmap(pixmap);
    }
    m_themedBackgroundRenderer->updateBackground();
}

#include "welcomescreen.moc"
