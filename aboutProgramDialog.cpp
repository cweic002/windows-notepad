#include "aboutProgramDialog.h"

AboutProgramDialog::AboutProgramDialog(QWidget * parent):QDialog(parent){
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint |
                   Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint );//Qt::MSWindowsFixedSizeDialogHint
    this->setMaximumSize(572, 484);
    this->resize(572, 484);
    this->setWindowTitle("Блокнот: сведения");

    QPixmap pix;
    pix.load(":/images/windows10.png");


    QString textHtml;
    textHtml="<center><img width=\"300\" align=\"\" src=\":images/windows10.png\"></center>";

    QLabel * labelAbout = new QLabel(this);
    labelAbout->resize(572,100);
    labelAbout->setText(textHtml);

}

void AboutProgramDialog::closeEvent(QCloseEvent *){
}
