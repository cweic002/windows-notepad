#ifndef LINEGODIALOG_H
#define LINEGODIALOG_H

#include "lineEditState.h"

class LineGoDialog:public QDialog
{
    Q_OBJECT
    QLineEdit * lineEdit;
    QTextEdit * textEdit;

    QPushButton * buttonJump;
    QPushButton * buttonCancel;

public:
    void show();
    LineGoDialog(QTextEdit * textEdit,QWidget * parent = nullptr);
};


#endif // LINEGODIALOG_H
