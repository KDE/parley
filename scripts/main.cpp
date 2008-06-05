#include "scripts.h"

#include <QStringList>

#include <KDebug>

int main() {
    ScriptTest s;
    QStringList l = s.translateWord("love");
    kDebug() << "List of translation results" << l;
} 
