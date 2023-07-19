#ifndef ABOUTPROGRAMDIALOG_H
#define ABOUTPROGRAMDIALOG_H
#include<QtWidgets>

class AboutProgramDialog : public QDialog
{
    Q_OBJECT
public:
    AboutProgramDialog(QWidget * parent = nullptr);
    void closeEvent(QCloseEvent *) override;
};


#endif // ABOUTPROGRAMDIALOG_H
