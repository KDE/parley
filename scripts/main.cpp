#include "scripts.h"
#include "translationscript.h"

#include <QStringList>

#include <KDebug>

int main() {
    //ScriptTest s;

    //kDebug() << QString("Translations: ") << s.translateWord("baby","en","it");
    //s.getLanguagePairs();
    TranslationScript script("google_translation.py");
    script.getLanguagePairs();
    kDebug() << script.translateWord("car","en","it");
} 
