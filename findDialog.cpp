#include "FindDialog.h"

FindDialog::FindDialog(QTextEdit * textEdit,QWidget *parent) :
    QDialog(parent),textEdit(textEdit)
{
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint |
                   Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    this->setMaximumSize(492, 148);
    this->setWindowTitle("Найти");

    QSizePolicy sizePolicy(QSizePolicy::Minimum,QSizePolicy::Maximum);

    labelFind = new QLabel("Что:");

    lineEditFind = new QLineEdit();

    auto fSaveLineEdit = [](QLineEdit * self){
        return self->text();
    };
    auto fRestoreLineEdit = [](QLineEdit * self,QString save){
        self->setText(save);
    };

    state.addState(lineEditFind,fSaveLineEdit,fRestoreLineEdit);

    auto buttonEnable=[this](const QString &text){
        if(text.isEmpty()){
            buttonFindNext->setEnabled(false);
            buttonReplace->setEnabled(false);
            buttonReplaceAll->setEnabled(false);
        }else{
            buttonFindNext->setEnabled(true);
            buttonReplace->setEnabled(true);
            buttonReplaceAll->setEnabled(true);
        }
    };
    connect(lineEditFind,&QLineEdit::textChanged,buttonEnable);

    labelFindReplace=new QLabel("Чем:");

    lineEditFindReplace = new QLineEdit();

    state.addState(lineEditFindReplace,fSaveLineEdit,fRestoreLineEdit);

    connect(lineEditFind,&QLineEdit::textEdited,buttonEnable);

    QGridLayout * gridLayoutFind=new QGridLayout();
    gridLayoutFind->addWidget(labelFind,0,0,1,1);
    gridLayoutFind->addWidget(lineEditFind,0,1,1,12);
    gridLayoutFind->addWidget(labelFindReplace,1,0,1,1);
    gridLayoutFind->addWidget(lineEditFindReplace,1,1,1,12);

    buttonFindNext = new QPushButton();
    buttonFindNext->setText("Найти далее");
    buttonFindNext->setSizePolicy(sizePolicy);
    buttonFindNext->setFocus(Qt::FocusReason::ActiveWindowFocusReason);
    connect(buttonFindNext,&QPushButton::clicked,[this](){
        auto statust = find();
        if(!statust){
            mesageBox();
        }
        state.saveState();
    });

    buttonReplace = new QPushButton();
    buttonReplace->setText("Заменить");
    buttonReplace->setSizePolicy(sizePolicy);
    connect(buttonReplace,&QPushButton::clicked,[this](){
        auto statust = find();
        if(statust){
            this->textEdit->textCursor().insertText(lineEditFindReplace->text());
        }else{
            mesageBox();
        }
        state.saveState();
    });

    buttonReplaceAll = new QPushButton();
    buttonReplaceAll->setText("Заменить все");
    buttonReplaceAll->setSizePolicy(sizePolicy);
    connect(buttonReplaceAll,&QPushButton::clicked,[this](){
        this->textEdit->moveCursor(QTextCursor::Start);
        while(find()){
            this->textEdit->textCursor().insertText(lineEditFindReplace->text());
        }
        state.saveState();
    });

    buttonCancel = new QPushButton();
    buttonCancel->setText("Отмена");
    buttonCancel->setSizePolicy(sizePolicy);
    connect(buttonCancel,&QPushButton::clicked,[this](){
        state.restoreState();
        this->reject();
    });
    QVBoxLayout * vBoxLayoutButtonGroup=new QVBoxLayout();
    vBoxLayoutButtonGroup->addWidget(buttonFindNext);
    vBoxLayoutButtonGroup->addWidget(buttonReplace);
    vBoxLayoutButtonGroup->addWidget(buttonReplaceAll);
    vBoxLayoutButtonGroup->addWidget(buttonCancel);

    groupBoxDirection = new QGroupBox();
    groupBoxDirection->setTitle("Направление");

    radioButtonUp = new QRadioButton();
    radioButtonUp->setText("Вверх");
    connect(radioButtonUp,&QRadioButton::toggled,[this](bool checked){
        if(checked){
            this->options = this->options | QTextDocument::FindFlag::FindBackward;
        }else{
            this->options = this->options & ~QTextDocument::FindFlag::FindBackward;
        }
    });

    radioButtonDown = new QRadioButton();
    radioButtonDown->setText("Вниз");
    radioButtonDown->setChecked(true);

    QButtonGroup * buttonGroupDirection = new QButtonGroup();
    buttonGroupDirection->addButton(radioButtonUp);
    buttonGroupDirection->addButton(radioButtonDown);

    state.addState(buttonGroupDirection,[](QButtonGroup * self){
        return self->checkedButton();
    },[](QAbstractButton * radioButton){
        radioButton->setChecked(true);
    });

    QHBoxLayout * hBoxLayout= new QHBoxLayout(groupBoxDirection);
    hBoxLayout->addWidget(radioButtonUp);
    hBoxLayout->addWidget(radioButtonDown);

    auto checkBoxFSave=[](QCheckBox * self){
        return self->isChecked();
    };
    auto checkBoxFRestore=[](QCheckBox * self,bool save){
        self->setChecked(save);
    };

    checkBoxCaseSensitive = new QCheckBox();
    checkBoxCaseSensitive->setText("С учетом регистра");
    checkBoxCaseSensitive->setSizePolicy(sizePolicy);
    connect(checkBoxCaseSensitive,&QCheckBox::toggled,[this](bool checked){
        if(checked){
            this->options= this->options | QTextDocument::FindFlag::FindCaseSensitively;
        }else{
            this->options= this->options & ~QTextDocument::FindFlag::FindCaseSensitively;
        }
    });

    state.addState(checkBoxCaseSensitive,checkBoxFSave,checkBoxFRestore);

    checkBoxAround = new QCheckBox();
    checkBoxAround->setText("Обтекание текстом");
    checkBoxAround->setSizePolicy(sizePolicy);

    state.addState(checkBoxAround,checkBoxFSave,checkBoxFRestore);

    auto gridLayout=new QGridLayout();

    gridLayout->addLayout(gridLayoutFind,0,0,1,12,Qt::AlignmentFlag::AlignTop);

    gridLayout->addWidget(groupBoxDirection,1,5,1,7,Qt::AlignmentFlag::AlignTop);

    gridLayout->addWidget(checkBoxCaseSensitive,1,0,1,4,Qt::AlignmentFlag::AlignBottom | Qt::AlignmentFlag::AlignLeft );
    gridLayout->addWidget(checkBoxAround,2,0,1,4,Qt::AlignmentFlag::AlignTop | Qt::AlignmentFlag::AlignLeft);

    gridLayout->addLayout(vBoxLayoutButtonGroup,0,13,3,7,Qt::AlignmentFlag::AlignTop);

    this->setLayout(gridLayout);

    buttonEnable(lineEditFind->text());
}

void FindDialog::mesageBox(){
    QMessageBox * messageBox = new QMessageBox(
                QMessageBox::Icon::Information,"Блокнот",
                "Не удается найти \"" + this->lineEditFind->text()+'"',
                QMessageBox::StandardButton::Ok,this);
    messageBox->exec();
}

void FindDialog::findActive(){
    activeFind();
    show();
}

void FindDialog::activeFind(){
    labelFindReplace->hide();
    lineEditFindReplace->hide();
    buttonReplace->hide();
    buttonReplaceAll->hide();
    groupBoxDirection->show();
}

void FindDialog::findReplaceActive(){
    activeFindReplace();
    show();
}

void FindDialog::activeFindReplace(){
    radioButtonDown->setChecked(true);
    labelFindReplace->show();
    lineEditFindReplace->show();
    buttonReplace->show();
    buttonReplaceAll->show();
    groupBoxDirection->hide();
}

void FindDialog::findUp(){
    groupBoxDirection->show();
    auto saveOption=options;
    options = options | QTextDocument::FindFlag::FindBackward;
    radioButtonUp->setChecked(true);
    if(buttonFindNext->isEnabled()){
        activeFind();
        buttonFindNext->click();
        options = saveOption;
    }else{
        options = saveOption;
        findActive();
    }
}

void FindDialog::findDown(){
    groupBoxDirection->show();
    auto saveOption = options;
    options = options & ~QTextDocument::FindFlag::FindBackward;
    radioButtonDown->setChecked(true);
    if(buttonFindNext->isEnabled()){
        activeFind();
        buttonFindNext->click();
        options = saveOption;
    }else{
        options = saveOption;
        findActive();
    }
}

bool FindDialog::find(){
    bool status = this->textEdit->find(lineEditFind->text(),options);
    if(!status){
        if(checkBoxAround->isChecked()){
            if(options & QTextDocument::FindFlag::FindBackward){
                this->textEdit->moveCursor(QTextCursor::End);
            }
            else{
                this->textEdit->moveCursor(QTextCursor::Start);
            }
        }
        status = this->textEdit->find(lineEditFind->text(),options);
    }
    return status;
}

void FindDialog::closeEvent(QCloseEvent *){
    state.restoreState();
}


