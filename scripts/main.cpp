#include "scripts.h"

#include <QStringList>

#include <KDebug>

int main() {
    ScriptTest s;

    kDebug() << QString("Translations: ") << s.translateWord("baby","en","it");
    s.getLanguagePairs();
} 
