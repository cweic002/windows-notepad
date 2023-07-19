#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include "State.h"

class FindDialog : public QDialog
{
    Q_OBJECT
public slots:
    void findActive();
    void findReplaceActive();
    void findUp();
    void findDown();
public:
    FindDialog(QTextEdit * textEdit,QWidget * parent = nullptr);
private:
    void closeEvent(QCloseEvent *) override;
    void inline activeFind();
    void inline activeFindReplace();
    void mesageBox();
    bool find();
    QTextDocument::FindFlags options;
    State state;
    QTextEdit * textEdit;
    QLabel * labelFind;
    QLineEdit * lineEditFind;
    QLabel * labelFindReplace;
    QLineEdit * lineEditFindReplace;

    QPushButton * buttonFindNext;
    QPushButton * buttonReplace;
    QPushButton * buttonReplaceAll;
    QPushButton * buttonCancel;

    QGroupBox * groupBoxDirection;
    QRadioButton * radioButtonUp;
    QRadioButton * radioButtonDown;

    QCheckBox *checkBoxCaseSensitive;
    QCheckBox *checkBoxAround;
};

#endif // SEARCHDIALOG_H
