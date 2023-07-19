#include "pagesettings.h"
#include <QPageSize>
#include <QDebug>
#include <QProxyStyle>

PageSettings::PageSettings(QWidget *parent):
    QDialog(parent),
    page(),
    view(),
    padding(),
    header("Верхний колонтитул:"),
    footer("Нижний колонтитул:")
{
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint |
                   Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    this->setWindowTitle("Параметры страницы");
    this->resize(712, 391);
    QFont font;
    font.setFamily(QString::fromUtf8("Arial"));
    font.setPointSize(9);
    this->setFont(font);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    connect(buttonBox,&QDialogButtonBox::accepted,[this](){
        this->state.saveState();
        this->accept();
    });
    connect(buttonBox,&QDialogButtonBox::rejected,[this](){
        this->state.restoreState();
        this->reject();
    });

    auto _orientation =new Orientation();

    QGridLayout * gridLayout=new QGridLayout(this);
    gridLayout->setContentsMargins(10,10,10,10);
    gridLayout->setSpacing(15);
    gridLayout->addWidget(page.groupBox,0,0,1,12);
    gridLayout->addWidget(_orientation->groupBox,1,0,1,4);
    gridLayout->addWidget(padding.groupBox,1,4,1,8);

    gridLayout->addWidget(header.label,2,0,1,4);
    gridLayout->addWidget(header.lineEdit,2,4,1,8);

    gridLayout->addWidget(footer.label,3,0,1,4);
    gridLayout->addWidget(footer.lineEdit,3,4,1,8);

    gridLayout->addWidget(view.groupBox,0,12,4,10);
    gridLayout->addWidget(buttonBox,4,12,1,10);

    connect(page.comboBoxSize,SIGNAL(currentIndexChanged(int)),SLOT(setStyleTextBrowser(int)));
    connect(padding.left.spinBox,SIGNAL(valueChanged(int)),SLOT(setStyleTextBrowser(int)));
    connect(padding.top.spinBox,SIGNAL(valueChanged(int)),SLOT(setStyleTextBrowser(int)));
    connect(padding.right.spinBox,SIGNAL(valueChanged(int)),SLOT(setStyleTextBrowser(int)));
    connect(padding.bottom.spinBox,SIGNAL(valueChanged(int)),SLOT(setStyleTextBrowser(int)));

    connect(_orientation->radioButtonPortrait,&QRadioButton::toggled,[this](bool checked){
        if(checked){
            this->orientation=QPageLayout::Orientation::Portrait;
        }else{
            this->orientation=QPageLayout::Orientation::Landscape;
        }
    });

    auto fSaveComboBox=[](QComboBox * self){
        return self->currentIndex();
    };
    auto fRestoreComboBox=[](QComboBox * self,int currentIndex){
        self->setCurrentIndex(currentIndex);
    };

    state.addState(page.comboBoxSize,fSaveComboBox,fRestoreComboBox);
    state.addState(page.comboBoxSupply,fSaveComboBox,fRestoreComboBox);

    state.addState(_orientation->buttonGroup,[](QButtonGroup * self){
        return self->checkedButton();
    },[](QAbstractButton* button){
        button->setChecked(true);
    });

    auto fSaveSpinBox=[](QSpinBox * self){
        return self->value();
    };
    auto fRestoreSpinBox=[](QSpinBox * self,int value){
        self->setValue(value);
    };

    state.addState(padding.top.spinBox,fSaveSpinBox,fRestoreSpinBox);
    state.addState(padding.left.spinBox,fSaveSpinBox,fRestoreSpinBox);
    state.addState(padding.right.spinBox,fSaveSpinBox,fRestoreSpinBox);
    state.addState(padding.bottom.spinBox,fSaveSpinBox,fRestoreSpinBox);

    auto fSaveLineEdit=[](QLineEdit * self){
        return self->text();
    };
    auto fRestoreLineEdit=[](QLineEdit * self,QString text){
        self->setText(text);
    };

    state.addState(header.lineEdit,fSaveLineEdit,fRestoreLineEdit);
    state.addState(footer.lineEdit,fSaveLineEdit,fRestoreLineEdit);
}

PageSettings::Page::Page(){
    groupBox = new QGroupBox("Бумага");

    auto * gridLayout = new QGridLayout(groupBox);
    gridLayout->setContentsMargins(10,10,10,10);

    auto labelSize = new QLabel("Размер:");
    gridLayout->addWidget(labelSize,0,0,1,2);

    comboBoxSize = new QComboBox();
    comboBoxSize->addItem("A3",QPageSize::A3);
    comboBoxSize->addItem("A4",QPageSize::A4);
    comboBoxSize->addItem("A5",QPageSize::A5);
    comboBoxSize->addItem("B4(JIS)",QPageSize::B4);
    comboBoxSize->addItem("B5(JIS)",QPageSize::B5);
    comboBoxSize->addItem("Executive",QPageSize::Executive);
    comboBoxSize->addItem("Legal",QPageSize::Legal);
    comboBoxSize->addItem("Letter",QPageSize::Letter);
    comboBoxSize->addItem("Statement",QPageSize::Statement);
    comboBoxSize->addItem("Таблоид",QPageSize::Tabloid);
    comboBoxSize->setCurrentText("A4");

    gridLayout->addWidget(comboBoxSize,0,4,1,7);

    auto labelSupply = new QLabel("Подача:",groupBox);
    labelSupply->setEnabled(false);

    gridLayout->addWidget(labelSupply,1,0,1,2);

    comboBoxSupply = new QComboBox(groupBox);
    comboBoxSupply->setEnabled(false);

    gridLayout->addWidget(comboBoxSupply,1,4,1,7);
}

PageSettings::View::View(){
    groupBox = new QGroupBox("Просмотр");

    textBrowser = new QTextBrowser();
    textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QHBoxLayout * hBoxLayout = new QHBoxLayout(groupBox);
    hBoxLayout->addWidget(textBrowser,Qt::AlignCenter);

    QFont font;
    font.setFamily(QString::fromUtf8("Arial"));
    font.setPointSize(3);
    textBrowser->setFont(font);
    textBrowser->setStyleSheet(QString::fromUtf8("QTextBrowser{\n"
    "   background: white;\n"
    "   border-width: 1px;\n"
    "	border-style: solid;\n"
    "	border-color: #bababa;\n"
    "	padding : 0px 0px 0px 0px;\n"
    "}"));
    textBrowser->setText("Eh bien, mon prince. Gênes et Lucques ne sont plus que des apanages, des поместья, de la famille Buonaparte."
                   "Non, je vous préviens que si vous ne me dites pas que nous avons la guerre, si vous vous permettez encore de pallier toutes les infamies,"
                   "toutes les atrocités de cet Antichrist (ma parole, j'y crois) — je ne vous connais plus, vous n'êtes plus mon ami, vous n'êtes plus мой верный раб, comme vous dites"
                   "Ну, здравствуйте, здравствуйте. Je vois que je vous fais peur 2, садитесь и рассказывайте.\n"
                   "Так говорила в июле 1805 года известная Анна Павловна Шерер, фрейлина и приближенная императрицы Марии Феодоровны, встречая важного и чиновного князя Василия, первого приехавшего на ее вечер"
                   "Анна Павловна кашляла несколько дней, у нее был грипп, как она говорила (грипп был тогда новое слово, употреблявшееся только редкими)"
                   "В записочках, разосланных утром с красным лакеем, было написано без различия во всех:\n"
                   "«Si vous n'avez rien de mieux à faire, Monsieur le comte (или mon prince), et si la perspective de passer la soirée chez une pauvre malade ne vous effraye pas trop, je serai charmée de vous voir chez moi entre 7 et 10 heures. Annette Scherer».");
}

PageSettings::Orientation::Orientation(){
    groupBox = new QGroupBox("Просмотр");

    QVBoxLayout * vBoxLayout =new QVBoxLayout(groupBox);

    buttonGroup = new QButtonGroup(groupBox);
    radioButtonPortrait = new QRadioButton("Книжная");
    radioButtonPortrait->setChecked(true);

    auto radioButtonLandscape = new QRadioButton("Альбомная");

    vBoxLayout->addWidget(radioButtonPortrait);
    vBoxLayout->addWidget(radioButtonLandscape);

    buttonGroup->addButton(radioButtonPortrait, QPageLayout::Orientation::Portrait);
    buttonGroup->addButton(radioButtonLandscape,QPageLayout::Orientation::Landscape);
}

PageSettings::Padding::Padding():
    groupBox(new QGroupBox("Поля (мм)")),
    left("Левый:"),
    top("Верхний:"),
    right("Правый:"),
    bottom("Нижний:")
{
    QGridLayout * gridLayout = new QGridLayout(groupBox);
    gridLayout->setContentsMargins(10,10,10,10);
    gridLayout->setSpacing(10);
    gridLayout->addWidget(left.label,0,0,1,1);
    gridLayout->addWidget(left.spinBox,0,1,1,1);

    gridLayout->addWidget(right.label,0,2,1,1);
    gridLayout->addWidget(right.spinBox,0,3,1,1);

    gridLayout->addWidget(top.label,1,0,1,1);
    gridLayout->addWidget(top.spinBox,1,1,1,1);

    gridLayout->addWidget(bottom.label,1,2,1,1);
    gridLayout->addWidget(bottom.spinBox,1,3,1,1);
}

PageSettings::Padding::Position::Position(QString labelName):
    label(new QLabel(labelName)),
    spinBox(new QSpinBox)
{
    spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    spinBox->setValue(10);
}

PageSettings::Title::Title(QString labelName):label(new QLabel(labelName)),
lineEdit(new QLineEdit){
}

void PageSettings::setStyleTextBrowser(int){
    auto sizePoint = QPageSize(size()).size(QPageSize::Unit::Millimeter);
    QString top=padding.top.spinBox->text();
    QString right=padding.right.spinBox->text();
    QString bottom=padding.bottom.spinBox->text();
    QString left=padding.left.spinBox->text();

    view.textBrowser->setStyleSheet("QTextBrowser{background: white;padding: "+top+"px "+right+"px "+bottom+"px "+left+"px;}");
    //view.frame->setStyleSheet("QFrame{border-width: 1px;border-style: dotted;border-color: black;margin: "+top+"px "+right+"px "+bottom+"px "+left+"px;}");
}

QPageSize::PageSizeId PageSettings::size(){
    return (page.comboBoxSize->currentData()).value<QPageSize::PageSizeId>();
}

QMargins PageSettings::margins(){
    return QMargins(padding.left.spinBox->value(),
                    padding.top.spinBox->value(),
                    padding.right.spinBox->value(),
                    padding.bottom.spinBox->value());
}

