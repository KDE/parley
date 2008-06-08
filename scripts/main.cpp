#include "scripts.h"
#include "translationscript.h"

#include <QStringList>
#include <QTextCodec>

#include <KDebug>

int main() {
    QTextCodec* Utf8Codec  = QTextCodec::codecForName("utf-8");

    //TranslationScript script("google_translation.py");
    //script.getLanguagePairs();
    //kDebug() << script.translateWord("car","en","it");

    TranslationScript s("ectaco.py");
    foreach(QString word, s.translateWord("test","en","pl"))
        kDebug() << "trns: " << word;

    return 0;
} 
