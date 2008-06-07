#include <QObject>

class ScriptTest
    :public QObject /* necessary to instantiate Kross::Action */
{
public:
    
    void test();
    void testJS();
};
