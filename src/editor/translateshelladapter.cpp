/*
    SPDX-FileCopyrightText: 2021 Andreas Cord-Landwehr <cordlandwehr@kde.org>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "translateshelladapter.h"
#include <QDebug>
#include <QProcess>
#include <QtConcurrent>

bool TranslateShellAdapter::isTranslateShellAvailable() const
{
    if (!m_translateShellAvailable) {
        QProcess process;
        process.start(QStringLiteral("trans"), QStringList() << QStringLiteral("--version"));
        process.waitForFinished(1000);
        if (process.error() != QProcess::ProcessError::FailedToStart) {
            qDebug() << "Translateshell process found";
            m_translateShellAvailable = true;
        }
    }
    return m_translateShellAvailable;
}

QFuture<TranslateShellAdapter::Translation>
TranslateShellAdapter::translateAsync(const QString &word, const QString &sourceLanguage, const QString &targetLanguage)
{
    QFuture<TranslateShellAdapter::Translation> result = QtConcurrent::run([word, sourceLanguage, targetLanguage]() {
        return TranslateShellAdapter::translate(word, sourceLanguage, targetLanguage);
    });
    return result;
}

TranslateShellAdapter::Translation TranslateShellAdapter::translate(const QString &word, const QString &sourceLanguage, const QString &targetLanguage)
{
    TranslateShellAdapter::Translation translation;

    QProcess process;
    process.start(QStringLiteral("trans"),
                  {QStringLiteral("-l"),
                   QStringLiteral("en"), // output language of CLI
                   QStringLiteral("-t"),
                   targetLanguage,
                   QStringLiteral("-s"),
                   sourceLanguage,
                   word,
                   QStringLiteral("-no-ansi"),
                   QStringLiteral("-show-alternatives=y"),
                   QStringLiteral("-show-original=n"),
                   QStringLiteral("-show-languages=n"),
                   QStringLiteral("-show-original-dictionary=n"),
                   QStringLiteral("-show-dictionary=y"),
                   QStringLiteral("-no-warn")});
    process.waitForFinished();
    if (process.exitCode() != 0) {
        TranslateShellAdapter::Translation translation;
        translation.m_error = true;
        return {};
    } else {
        return TranslateShellAdapter::parseTranslateShellResult(QString::fromUtf8(process.readAll()));
    }
}

QFuture<bool> TranslateShellAdapter::downloadSoundFile(const QString &word, const QString &language, const QString &filePath)
{
    if (!filePath.endsWith(QStringLiteral(".ts"))) {
        qWarning() << "Sound file will have TS format and should have that suffix";
    }
    if (QFile::exists(filePath)) {
        qWarning() << "File already exists, this operation will override the file" << filePath;
    }
    QDir directory = QFileInfo(filePath).absoluteDir();
    if (!directory.exists(directory.absolutePath())) {
        qWarning() << "Output directory does not exist, creating it" << directory;
        directory.mkpath(directory.absolutePath());
    }

    QFuture<bool> result = QtConcurrent::run([word, language, filePath]() {
        QProcess process;
        process.start(QStringLiteral("trans"),
                      {QStringLiteral("-l"),
                       QStringLiteral("en"), // output language of CLI
                       QStringLiteral("-t"),
                       language,
                       word,
                       QStringLiteral("-no-translate"),
                       QStringLiteral("-download-audio-as"),
                       filePath});
        process.waitForFinished();
        return process.error() != QProcess::ProcessError::FailedToStart && process.exitCode() == 0;
    });
    return result;
}

TranslateShellAdapter::Translation TranslateShellAdapter::parseTranslateShellResult(const QString &output)
{
    const QStringList lines = output.split(QLatin1Char('\n'));

    Translation result;
    if (lines.count() < 1) {
        result.m_error = true;
        return result;
    }

    // magic line parsing, since current version of translate-shell does not support stable CLI API
    result.m_suggestions = QStringList() << lines.at(0).trimmed();
    for (int i = 5; i < lines.count(); ++i) {
        result.m_synonyms << lines.at(i).trimmed();
    }

    return result;
}
