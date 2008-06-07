#include "scripts.h"
#include "translationscript.h"

#include <QStringList>

#include <KDebug>

int main() {
    TranslationScript script("google_translation.py");
    script.getLanguagePairs();
    kDebug() << script.translateWord("car","en","it");
} 
