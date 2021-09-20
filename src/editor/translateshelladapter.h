/*
    SPDX-FileCopyrightText: 2021 Andreas Cord-Landwehr <cordlandwehr@kde.org>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef TRANSLATESHELLADAPTER_H
#define TRANSLATESHELLADAPTER_H

#include <QFuture>
#include <QProcessEnvironment>
#include <QStringList>

/**
 * @brief Adapter to online translation services
 *
 * This class can be used to obtain translations of words via translatorshell.
 * It is important to keep in mind that the whole integration is a best-effort integration. Always check first that
 * translateshell is available and give reasonable information to the user. Moreover, some of the translateshell
 * online APIs stop answering translation requests after a short time, which then leads to empty translation results,
 * because no error responses are available.
 */
class TranslateShellAdapter
{
public:
    /**
     * @brief Result object of a translation request
     */
    struct Translation {
        QStringList m_suggestions;
        QStringList m_synonyms;
        QString phonetic;
        bool m_error{false};
    };

    /**
     * @brief Check if translateshell is supported
     *
     * Note that this call starts the translateshell --version option and thus you might not run this call in UI thread.
     *
     * @return true if application can be found, otherwise false
     */
    bool isTranslateShellAvailable() const;

    static TranslateShellAdapter::Translation parseTranslateShellResult(const QString &output);

    /**
     * @brief Asynchronous call to translate method
     *
     * This should be the preferred way to request translations. By using a watcher object the future object can be
     * monitored in the main thread and it can be acted upon the availability of the translation without blocking UI.
     *
     * It is expected to check first if translateshell is available and supported with isTranslateShellAvailable().
     * If process cannout be found or aborts, the result just provides an error flag.
     *
     * @param word the string that shall be translated, can also be a sequence of words
     * @param sourceLanguage the source language identifier as ISO-639-1 language code
     * @param targetLanguage the target language identifier as ISO-639-1 language code
     * @return returns a list of available translations
     */
    static QFuture<Translation> translateAsync(const QString &word, const QString &sourceLanguage, const QString &targetLanguage);

    /**
     * @brief Synchronous call to translate method
     *
     * Note that calling this method blocks the calling thread.
     *
     * It is expected to check first if translateshell is available and supported with isTranslateShellAvailable().
     * If process cannout be found or aborts, the result just provides an error flag.
     *
     * @param word the string that shall be translated, can also be a sequence of words
     * @param sourceLanguage the source language identifier as ISO-639-1 language code
     * @param targetLanguage the target language identifier as ISO-639-1 language code
     * @return returns a list of available translations
     */
    static TranslateShellAdapter::Translation translate(const QString &word, const QString &sourceLanguage, const QString &targetLanguage);

    /**
     * @brief Asynchronous download of sound file for given phrase
     *
     */
    static QFuture<bool> downloadSoundFile(const QString &word, const QString &language, const QString &filePath);

private:
    mutable bool m_translateShellAvailable{false};
};

#endif
