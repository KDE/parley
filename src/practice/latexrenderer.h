/*
    SPDX-FileCopyrightText: 2010 Daniel Laidig <laidig@kde.org>
    SPDX-FileCopyrightText: 2009 Alexander Rieder <alexanderrieder@gmail.com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

// based on Cantor's LaTeX rendering code

#ifndef PRACTICE_LATEXRENDERER_H
#define PRACTICE_LATEXRENDERER_H

#include <QObject>

class QLabel;

namespace Practice
{

class LatexRenderer : public QObject
{
    Q_OBJECT

public:
    explicit LatexRenderer(QObject* parent = 0);
    void setResultLabel(QLabel *label) {
        m_label = label;
    }
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
