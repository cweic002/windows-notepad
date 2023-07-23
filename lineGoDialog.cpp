#include "lineGoDialog.h"

LineGoDialog::LineGoDialog(QTextEdit * textEdit,QWidget * parent):
    QDialog(parent),textEdit(textEdit){
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint |
                   Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    this->setMaximumSize(492, 148);
    this->setWindowTitle("Переход на строку");

    auto label = new QLabel("Номер строки:");

    QIntValidator * validator = new QIntValidator(1,999999,this);

    lineEdit = new QLineEdit();
    lineEdit->setValidator(validator);

    connect(lineEdit,&QLineEdit::textChanged,[this,validator](const QString &text){
        if(text.isEmpty()){
            buttonJump->setEnabled(false);
        }else{
            int pos = 0;
            if(QValidator::State::Acceptable == validator->validate(const_cast<QString &>(text),pos)){
                buttonJump->setEnabled(true);
            }
        }
    });

    buttonJump = new QPushButton("Переход");
    buttonJump->setEnabled(false);
    connect(buttonJump,&QPushButton::clicked,[this](){
        auto document=this->textEdit->document();
        auto number=lineEdit->text().toInt()-1;
            if(document->blockCount() > number){
            QTextCursor cursor = this->textEdit->textCursor();
            cursor.movePosition(QTextCursor::Start); // Перемещаем курсор в начало текста
            cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, number);
            this->textEdit->setTextCursor(cursor);
            this->textEdit->ensureCursorVisible();
            this->accept();
        }else{
            QMessageBox msgBox;
            msgBox.setWindowTitle("Блокнот - " + this->windowTitle());
            msgBox.setText("Номер строки превышает общее число строк");
            msgBox.setContentsMargins(0,10,30,0);
            msgBox.exec();
        }
    });

    buttonCancel = new QPushButton("Отмена");
    connect(buttonCancel,&QPushButton::clicked,this,&LineGoDialog::reject);

    QGridLayout * gridLayout = new QGridLayout();
    gridLayout->addWidget(label,0,0,1,1);
    gridLayout->addWidget(lineEdit,1,0,1,12);
    gridLayout->addWidget(buttonJump,2,4,1,4);
    gridLayout->addWidget(buttonCancel,2,8,1,4);

    this->setLayout(gridLayout);
}

void LineGoDialog::show(){
    lineEdit->setText(QString::number(this->textEdit->textCursor().blockNumber()+1));
    QDialog::show();
}
