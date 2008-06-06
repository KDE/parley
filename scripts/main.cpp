#include "scripts.h"

#include <QStringList>

#include <KDebug>

int main() {
    ScriptTest s;

    kDebug() << QString("Translations: ") << s.translateWord("hello","en","de");
} 
