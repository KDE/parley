#include <QObject>

class ScriptTest
    :public QObject /* necessary to instantiate Kross::Action */
{
public:
    
    void test();
    QStringList translateWord(QString word, QString from, QString to);
    void testJS();
    void getLanguagePairs();
};
