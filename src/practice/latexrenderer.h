/***************************************************************************
    Copyright 2010 Daniel Laidig <laidig@kde.org>
    Copyright 2009 Alexander Rieder <alexanderrieder@gmail.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// based on Cantor's LaTeX rendering code

#ifndef PRACTICE_LATEXRENDERER_H
#define PRACTICE_LATEXRENDERER_H

#include <QObject>

class QLabel;

namespace Practice {

class LatexRenderer : public QObject
{
    Q_OBJECT

public:
    LatexRenderer(QObject* parent = 0);
    void setResultLabel(QLabel *label) {m_label = label;}
    void renderLatex(QString tex);

    static bool isLatex(const QString& tex);

private slots:
    void convertToPs();
    void convertToImage();
    void latexRendered();

private:
    QLabel *m_label;
    QString m_latexFilename;
};

}

#endif // PRACTICE_LATEXRENDERER_H
