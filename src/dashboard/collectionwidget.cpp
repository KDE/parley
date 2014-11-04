/***************************************************************************
    Copyright 2014 Andreas Xavier
    Copyright 2014 Inge Wallin
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


// Own
#include "collectionwidget.h"

// Parley
#include "collection.h"
#include "barwidget.h"

// KDE
#include <klocalizedstring.h>

// Qt
#include <QtGui>
#include <QColor>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGraphicsDropShadowEffect>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>


// Size constants for the collection widgets
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


static int randInt(int low, int high)
{
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}


// This is just used until the real handling of the due words works.
DueWords::DueWords()
{
    // FIXME: This is only for testing purposes. We need a way to get the
    //        grades and words due for every document.

    totalDueWords = 0;

#if 0
    int firstGrade = randInt(0,7); //This is done for vanity purposes only, giving due word values to only two grades for now.
    int secondGrade = randInt(0,7);
#else
    // Disable this until it works in lower layers too
    int firstGrade = 0;
    int secondGrade = 7;
#endif
    for (int x = 0; x < 8; x++) {
   if (x == firstGrade || x == secondGrade) {
       dueWords[x] = randInt(0,20);
       totalDueWords += dueWords[x];
   }
   else {
       dueWords[x] = 0;
   }
    }

    //To test randomnly for Complete Collections. Again to be obtained from document.
    percentageCompleted = randInt(98, 99);

}

// ----------------------------------------------------------------


CollectionWidget::CollectionWidget(const QString &titleString, DueWords *due,
				   QWidget *parent)
    : QWidget(parent)
{
    // Set a nice shadow effect.
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(50);
    this->setGraphicsEffect(effect);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, Qt::white);
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
    m_titleLabel = new QLabel(titleString);
    mainLayout->addWidget(m_titleLabel);
    m_thumbnail = new QWidget;
    palette = m_thumbnail->palette();
    int y = randInt(8, 9);
    palette.setColor(QPalette::Background, gradeColor[y]);
    m_thumbnail->setAutoFillBackground(true);
    m_thumbnail->setPalette(palette);
    m_thumbnail->setFixedSize(COLLWIDTH - 10, COLLHEIGHT1 - COLLHEIGHT2 + 10);
    if (due->percentageCompleted != 100) {
	mainLayout->addWidget(m_thumbnail);
    }

    BarWidget *barWidget = new BarWidget(due->dueWords, due->totalDueWords,
                due->percentageCompleted);
    barWidget->setFixedSize(COLLWIDTH - 10, 20);
    mainLayout->addWidget(barWidget);
    if (due->totalDueWords == 0 && due->percentageCompleted < 100) {
        m_practiceButton = new QPushButton(i18n("Practice Anyway"));
    }
    else {
        m_practiceButton = new QPushButton(i18n("Practice"));
    }
    m_practiceButton->setStyleSheet("QPushButton {border: none; margin: 0px; padding: 0px;}");

    // buttonLayout is the horizontal layout for the bottom line in the
    // collection widget: delete button, practice button, etc
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    mainLayout->addLayout(buttonLayout);
    m_removeButton = new RemoveButton();
    m_removeButton->setFixedSize(20, 20);
    buttonLayout->setAlignment(m_removeButton, Qt::AlignLeft | Qt::AlignVCenter);
    buttonLayout->setAlignment(m_practiceButton, Qt::AlignCenter);
    buttonLayout->addWidget(m_removeButton);
    buttonLayout->addWidget(m_practiceButton);
    buttonLayout->addItem(new QSpacerItem(20, 20));

    connect(m_practiceButton, SIGNAL(clicked()), this, SIGNAL(practiceButtonClicked()));
    connect(m_removeButton,   SIGNAL(clicked()), this, SIGNAL(removeButtonClicked()));
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

