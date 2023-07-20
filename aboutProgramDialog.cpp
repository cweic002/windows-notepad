#include "aboutProgramDialog.h"

AboutProgramDialog::AboutProgramDialog(QWidget * parent):QDialog(parent){
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint |
                   Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint );//Qt::MSWindowsFixedSizeDialogHint
    this->setWindowTitle("Блокнот: сведения");

    QPixmap pix;
    pix.load(":/images/windows10.png");

    pix = pix.scaledToHeight(75,Qt::SmoothTransformation);

    QLabel * labelWindows10Image = new QLabel();
    labelWindows10Image->setPixmap(pix);
    labelWindows10Image->setAlignment(Qt::AlignmentFlag::AlignCenter);

    QLabel * QLabelFrame =new QLabel();
    QLabelFrame->setFrameStyle(QFrame::Box | QFrame::Raised);
    QLabelFrame->setMaximumHeight(1);

    QLabel * labelAbout = new QLabel();

    labelAbout->setText("<html>"
                        "<head>"
                        "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"
                        "<title>О программе</title>"
                        "</head>"
                        " <style>"
                        "p{"
                            "margin-left:10px;"
                        "}"
                        "body{"
                            "background-color: red;"
                        "}"
                        "</style>"
                        "<body>"
                        "<h4>Cweic002</h4>"
                        "<p>Программное обеспечение было написанно на фреймворке Qt.В качестве примера была"
                        "<br>использованна программа notepad включенная в состав операционной системы Microsoft Windows."
                        "<br>Данная пограмма была создана с целью демонстраций своих возможностей и не является"
                        "<br>коммерческим продуктом.</p>"
                        "<p align=\"right\">С уважением, Cweic002</p>"
                        "</body>"
                        "</html>");
    labelAbout->setStyleSheet("");
    QVBoxLayout * vBoxLayoutAbout = new QVBoxLayout();
    vBoxLayoutAbout->addWidget(labelAbout);

    QPushButton * PushButtonOk = new QPushButton("Ок");
    PushButtonOk->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Minimum);
    connect(PushButtonOk,&QPushButton::clicked,this,&AboutProgramDialog::accept);

    QVBoxLayout * vBoxLayout = new QVBoxLayout();
    vBoxLayout->addWidget(labelWindows10Image);
    vBoxLayout->addWidget(QLabelFrame);
    vBoxLayout->addLayout(vBoxLayoutAbout);
    vBoxLayout->addWidget(PushButtonOk,1,Qt::AlignmentFlag::AlignRight);

    this->setLayout(vBoxLayout);
}

void AboutProgramDialog::closeEvent(QCloseEvent *){
}
