

#ifndef PRACTICE_MAINWINDOW_H
#define PRACTICE_MAINWINDOW_H

#include <KXmlGuiWindow>

namespace Practice {    

class PracticeMainWindow : public KXmlGuiWindow
    {
        public:
            PracticeMainWindow(QWidget* parent = 0, Qt::WindowFlags f = 0)
            : KXmlGuiWindow(parent, f) {};
    };
}    


#endif