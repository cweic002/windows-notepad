#ifndef LINEEDITSTATE_H
#define LINEEDITSTATE_H
#include <QtWidgets>


class LineEditState: public QLineEdit{
    QString previousText;
public:
    void saveState(){
         previousText = text();
    }
    void restoreState(){
        setText(previousText);
    }
};


#endif // LINEEDITSTATE_H
