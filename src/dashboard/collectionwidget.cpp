/*
    SPDX-FileCopyrightText: 2014 Andreas Xavier
    SPDX-FileCopyrightText: 2014 Inge Wallin <inge@lysator.liu.se>
    SPDX-License-Identifier: GPL-2.0-or-later
*/


// Own
#include "collectionwidget.h"

// Parley

// KDE
#include <KLocalizedString>

// Qt
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#include <QVBoxLayout>
#include <QLabel>

// Parley
#include "utils.h"


// Size constants for the collection widgets
int COLLWIDTH   = 155; // Width in pixels of a collection widget
//int COLLHEIGHT1 = 250; // Height in pixels of a collection widget not yet fully learned
int COLLHEIGHT1 = 175; // Height in pixels of a collection widget not yet fully learned
int COLLHEIGHT2 = 125; // Height in pixels of a collection widget fully learned


// ================================================================
//                        private classes



// The RemoveButton is a button that the user can press in a collection to
// remove the collection from the word bank.


class RemoveButton : public QPushButton
{
public:
    RemoveButton(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
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

    painter.drawEllipse(QRectF(1.0, 1.0, height() - 1.0, height() - 1.0));
    painter.setFont( QFont( QStringLiteral("Helvetica"), 7, QFont::Bold, false));
    painter.drawText(2, 1, height() - 2, height() - 1, Qt::AlignCenter, QStringLiteral("x"));
}

CollectionWidget::CollectionWidget(Collection *collection, WordCount *dueWords, QWidget *parent)
    : QWidget(parent)
    , m_collection(collection)
{
    setupWidget(dueWords);
    fillWidget();
}

CollectionWidget::~CollectionWidget()
{
}


Collection *CollectionWidget::collection() const
{
    return m_collection;
}

void CollectionWidget::setCollection(Collection *collection)
{
    m_collection = collection;
}

void CollectionWidget::updateDue()
{
    WordCount  due;
    m_collection->numDueWords(due);
    m_barWidget->setDue(due);

    if (due.totalWords == 0 /* && due->percentageCompleted < 100*/) {
        m_practiceButton->setText(i18n("Practice Anyway"));
    }
    else {
        m_practiceButton->setText(i18n("Practice"));
    }
}


// ----------------------------------------------------------------
//                         private classes


void CollectionWidget::setupWidget(WordCount *dueWords)
{
    // Set a nice shadow effect.
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(50);
    this->setGraphicsEffect(effect);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    // Fill in the contents of the widget.

    // mainLayout is the main vertical layout for one collection widget.
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setAlignment(Qt::AlignCenter);
    this->setLayout(mainLayout);

    // One collection is laid out vertically like this:
    //  1. titleLabel:   contains the title of the collection
    //  2. thumbnail:    an image that represents the collection. It could be a
    //                   wordcloud generated from the words in the collection, a
    //                   logo or something else.
    //  3. barWidget:    a visual bar showing the training status of the words in
    //                   the collection
    //  4. buttonLayout: a horizontal row of pushbuttons for delete, practice, etc
    m_titleLabel = new QLabel(this);
    mainLayout->addWidget(m_titleLabel);

    m_thumbnail = new QLabel(this);
    m_thumbnail->setFixedSize(COLLWIDTH - 10, COLLHEIGHT1 - COLLHEIGHT2 + 10);
    QPixmap pixmap(m_thumbnail->size());
    pixmap.fill(Qt::lightGray);
    m_thumbnail->setPixmap(pixmap);

    int  percentageCompleted = dueWords->percentageCompleted();
    if (percentageCompleted != 100) {
        mainLayout->addWidget(m_thumbnail);
    }

    m_barWidget = new BarWidget(dueWords, this);
    m_barWidget->setFixedSize(COLLWIDTH - 10, 45);
    mainLayout->addWidget(m_barWidget);
    m_practiceButton = new QPushButton(this);
    QString buttonStyleSheet = "QPushButton { border: none; margin: 0px; padding: 0px; color: " +
            palette.color(QPalette::Active, QPalette::ButtonText).name()+ "; background-color: " +
            palette.color(QPalette::Active, QPalette::Button).name() + "; border-radius: 3px;}";
    m_practiceButton->setStyleSheet(buttonStyleSheet);

    // buttonLayout is the horizontal layout for the bottom line in the
    // collection widget: delete button, practice button, etc
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    mainLayout->addLayout(buttonLayout);
    m_removeButton = new RemoveButton(this);
    m_removeButton->setFixedSize(20, 20);
    buttonLayout->setAlignment(m_removeButton, Qt::AlignLeft | Qt::AlignVCenter);
    buttonLayout->setAlignment(m_practiceButton, Qt::AlignCenter);
    buttonLayout->addWidget(m_removeButton);
    buttonLayout->addWidget(m_practiceButton);
    buttonLayout->addItem(new QSpacerItem(20, 20));

    connect(m_practiceButton, &QPushButton::clicked, this, &CollectionWidget::practiceButtonClicked);
    connect(m_removeButton, &RemoveButton::clicked, this, &CollectionWidget::removeButtonClicked);
}


void CollectionWidget::fillWidget()
{
    m_titleLabel->setText(m_collection->eduVocDocument()->title());

    updateDue();
}
