#include "mainwindow.h"
#include "pagesettings.h"

#include <QPrinter>
#include <QPrintDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      textEdit(new QTextEdit()),
      zoom(1),
      statusBar(new QStatusBar(this)),
      file(new ap::SaveFileCreate()),
      findDialog(new FindDialog(textEdit,this)),
      pageSettings(new PageSettings(this)),
      lineGoDialog(new LineGoDialog(textEdit,this)),
      aboutProgramDialog(new AboutProgramDialog())
{
    this->setWindowTitle("Блокнот");
    this->resize(834, 541);
    this->setMinimumSize(325, 76);
    this->setCentralWidget(textEdit);
    this->setStatusBar(statusBar);

    this->setWindowIcon(QPixmap(":/images/Windows_Notepad_Icon.png"));


    QTextOption textOption;
    textOption.setWrapMode(QTextOption::NoWrap);

    font.setFamily(QString::fromUtf8("Arial"));
    font.setPointSize(10);

    textEdit->document()->setDefaultTextOption(textOption);
    textEdit->setParent(this);
    textEdit->setFont(font);
    textEdit->setFrameShape(QFrame::NoFrame);
    textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    textEdit->setStyleSheet("QTextEdit{"
                            "selection-color: white;"
                            "selection-background-color: #0078d7;"
                            "}");

    auto menu_file = menuFile();
    auto menu_correction = menuCorrection();
    auto menu_format = menuFormat();
    auto menu_view = menuView();
    auto menu_reference = menuReference();

    auto menuBar = new QMenuBar(this);
    this->setMenuBar(menuBar);

    menuBar->addAction(menu_file->menuAction());
    menuBar->addAction(menu_correction->menuAction());
    menuBar->addAction(menu_format->menuAction());
    menuBar->addAction(menu_view->menuAction());
    menuBar->addAction(menu_reference->menuAction());

    auto text = textEdit->cursor();
    QTextCodec * codec = QTextCodec::codecForLocale();
    QString encodingStr = codec->name();

    QWidget * widgetEmptiness = new QWidget();

    labelPagePosition = new QLabel("Стр 16, стлб 112");
    labelPagePosition->setContentsMargins(0,0,70,0);

    labelZoom = new QLabel("100%");
    labelZoom->setContentsMargins(0,0,10,0);

    QLabel * labelWindows = new QLabel("Winddows(SRLF)");
    labelWindows->setContentsMargins(0,0,20,0);

    QLabel * labelEncoding = new QLabel("UTF-8");
    labelEncoding->setContentsMargins(0,0,60,0);

    statusBar->addPermanentWidget(widgetEmptiness);
    statusBar->addPermanentWidget(labelPagePosition);
    statusBar->addPermanentWidget(labelZoom);
    statusBar->addPermanentWidget(labelWindows);
    statusBar->addPermanentWidget(labelEncoding);
    statusBar->hide();
};

//menu File
QMenu * MainWindow::menuFile(){
    auto create = new QAction(this);
    auto newWindow = new QAction(this);
    auto open = new QAction(this);
    auto seve = new QAction(this);
    auto seve_as = new QAction(this);
    auto pageSettings = new QAction(this);
    auto print = new QAction(this);
    auto exit = new QAction(this);

    create->setText("Создать");
    create->setShortcut(QKeySequence("Ctrl+N"));
    connect(create, &QAction::triggered,this,&MainWindow::file_create_triggered);

    newWindow->setText("Новое окно");
    newWindow->setShortcut(QKeySequence("Ctrl+Shift+N"));
    connect(newWindow, &QAction::triggered,this,&MainWindow::file_newWindow_triggered);

    open->setText("Открыть...");
    open->setShortcut(QKeySequence("Ctrl+O"));
    connect(open, &QAction::triggered,this,&MainWindow::file_open_triggered);

    seve->setText("Сохранить");
    seve->setShortcut(QKeySequence("Ctrl+S"));
    connect(seve, &QAction::triggered,this,&MainWindow::file_seve_triggered);

    seve_as->setText("Сохранить как...");
    seve_as->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(seve_as, &QAction::triggered,this,&MainWindow::file_seve_as_triggered);

    pageSettings->setText("Параметры страницы...");
    connect(pageSettings, &QAction::triggered,this->pageSettings,&PageSettings::exec);

    print->setText("Печать...");
    print->setShortcut(QKeySequence("Ctrl+P"));
    connect(print, &QAction::triggered,this,&MainWindow::file_print_triggered);

    exit->setText("Выход");
    connect(exit, &QAction::triggered,this,&MainWindow::close);

    auto file = new QMenu;
    file->setTitle("Файл");
    file->addAction(create);
    file->addAction(newWindow);
    file->addAction(open);
    file->addAction(seve);
    file->addAction(seve_as);
    file->addSeparator();
    file->addAction(pageSettings);
    file->addAction(print);
    file->addSeparator();
    file->addAction(exit);

    return file;
}

void MainWindow::file_create_triggered()
{
    textEdit->clear();
    file.reset(new ap::SaveFileCreate());
}

void MainWindow::file_newWindow_triggered()
{
     MainWindow * newWindow=new MainWindow();
     newWindow->show();
}

void MainWindow::file_open_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Открыть файл"), QDir::currentPath(), tr("*.txt"));
    if (filename.isEmpty() == false)
    {
        file.reset(new ap::SaveOpenFile(filename));
        QFile file(filename);
           if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
               return;
        textEdit->setText(file.readAll());
        file.close();
    }
}

void MainWindow::file_seve_triggered()
{
    this->file->text=textEdit->toPlainText().toUtf8();
    this->file->save();
}

void MainWindow::file_seve_as_triggered()
{
    this->file->text=textEdit->toPlainText().toUtf8();
    this->file->save_as();
}

void MainWindow::file_print_triggered()
{
    QPrinter printer(QPrinter::PrinterMode::ScreenResolution);
    QPrintDialog * dialog = new QPrintDialog(&printer,this);
    QFont font;
    font.setFamily(QString::fromUtf8("Arial"));
    font.setPointSize(9);
    dialog->setFont(font);
    if (dialog->exec() == QDialog::Accepted){
        QTextDocument * document=new QTextDocument(this);
        document->setHtml(textEdit->toHtml());
        // Настраиваем параметры страницы
        QPageSize pageSize(pageSettings->size());
        QMargins margins=pageSettings->margins();
        QPageLayout::Orientation orientation(pageSettings->orientation);
        QPageLayout pageLayout(pageSize,orientation, margins,QPageLayout::Unit::Millimeter);
        document->setPageSize(pageLayout.fullRect(QPageLayout::Unit::Point).size());
        //настройка принтера(QPrinter::Orientation)orientation
        printer.setOrientation(static_cast<QPrinter::Orientation>(orientation));
        printer.setPageMargins(margins,QPageLayout::Unit::Millimeter);
        //настройка принтера
        document->print(&printer);
    }
}
//menu File

//menu Correction
QMenu * MainWindow::menuCorrection(){
    auto cancel = new QAction(this);
    auto cut = new QAction(this);
    auto copy = new QAction(this);
    auto paste = new QAction(this);
    auto _delete = new QAction(this);
    auto find_yandex = new QAction(this);
    auto find = new QAction(this);
    auto find_next = new QAction(this);
    auto find_earlier = new QAction(this);
    auto replace = new QAction(this);
    goLine = new QAction(this);
    auto selectAll = new QAction(this);
    auto timeDate = new QAction(this);

    cancel->setEnabled(false);
    cancel->setText("Отменить");
    cancel->setShortcut(QKeySequence("Ctrl+Z"));
    connect(cancel, &QAction::triggered,textEdit,&QTextEdit::undo);
    connect(textEdit, &QTextEdit::undoAvailable,cancel,&QAction::setEnabled);

    cut->setEnabled(false);
    cut->setText("Вырезать");
    cut->setShortcut(QKeySequence("Ctrl+X"));
    connect(cut, &QAction::triggered,textEdit,&QTextEdit::cut);
    connect(textEdit,&QTextEdit::copyAvailable,cut,&QAction::setEnabled);

    copy->setEnabled(false);
    copy->setText("Копировать");
    copy->setShortcut(QKeySequence("Ctrl+C"));
    connect(copy, &QAction::triggered,textEdit,&QTextEdit::copy);
    connect(textEdit,&QTextEdit::copyAvailable,copy,&QAction::setEnabled);

    paste->setEnabled(false);
    paste->setText("Вставить");
    paste->setShortcut(QKeySequence("Ctrl+V"));
    auto clipboard = QApplication::clipboard();
    auto func=[clipboard,paste](){
        if(clipboard->text().isEmpty()){
            paste->setEnabled(false);
        }else{
            paste->setEnabled(true);
        }
    };
    func();
    connect(paste, &QAction::triggered,textEdit,&QTextEdit::paste);
    connect(clipboard,&QClipboard::dataChanged,func);

    _delete->setEnabled(false);
    _delete->setText("Удалить");
    _delete->setShortcut(QKeySequence("Del"));
    connect(_delete, &QAction::triggered,[this](){
        textEdit->textCursor().removeSelectedText();
    });
    connect(textEdit,&QTextEdit::copyAvailable,_delete,&QAction::setEnabled);

    find_yandex->setEnabled(false);
    find_yandex->setText("Поиск с помощью Yandex...");
    connect(find_yandex, &QAction::triggered,[this](){
        QString url="https://yandex.ru/search/?text=";
        url+=textEdit->textCursor().selectedText();
        QDesktopServices::openUrl(QUrl(url));
    });
    connect(textEdit,&QTextEdit::copyAvailable,find_yandex,&QAction::setEnabled);

    find->setText("Найти...");
    find->setShortcut(QKeySequence("Ctrl+F"));
    connect(find,&QAction::triggered,findDialog,&FindDialog::findActive);

    find_next->setText("Найти далее");
    find_next->setShortcut(QKeySequence("F3"));
    connect(find_next,&QAction::triggered,findDialog,&FindDialog::findDown);

    find_earlier->setText("Найти ранее");
    find_earlier->setShortcut(QKeySequence("Shift+F3"));
    connect(find_earlier,&QAction::triggered,findDialog,&FindDialog::findUp);

    replace->setText("Заменить...");
    replace->setShortcut(QKeySequence("Ctrl+H"));
    connect(replace,&QAction::triggered,findDialog,&FindDialog::findReplaceActive);

    goLine->setText("Перейти...");
    goLine->setShortcut(QKeySequence("Ctrl+G"));
     connect(goLine,&QAction::triggered,lineGoDialog,&LineGoDialog::show);

    selectAll->setText("Выделить все");
    selectAll->setShortcut(QKeySequence("Ctrl+A"));
    connect(selectAll,&QAction::triggered,textEdit,&QTextEdit::selectAll);

    timeDate->setText("Время и дата");
    timeDate->setShortcut(QKeySequence("F5"));
    connect(timeDate,&QAction::triggered,[this](){
        QString strDateTime = QDateTime::currentDateTime().toString("hh:mm dd.MM.yyyy");
        this->textEdit->insertPlainText(strDateTime);
    });

    auto correction = new QMenu;
    correction->setTitle("Правка");
    correction->addAction(cancel);
    correction->addSeparator();
    correction->addAction(cut);
    correction->addAction(copy);
    correction->addAction(paste);
    correction->addAction(_delete);
    correction->addSeparator();
    correction->addAction(find_yandex);
    correction->addAction(find);
    correction->addAction(find_next);
    correction->addAction(find_earlier);
    correction->addAction(replace);
    correction->addAction(goLine);
    correction->addSeparator();
    correction->addAction(selectAll);
    correction->addAction(timeDate);
    return correction;
}
//menu Correction

//menu Format
QMenu * MainWindow::menuFormat(){
    auto wordWrap = new QAction(this);
    auto font = new QAction(this);

    wordWrap->setText("Перенос по словам");
    wordWrap->setCheckable(true);
    connect(wordWrap,&QAction::toggled,[this](bool status){
        this->goLine->setEnabled(!status);
        if(status){
            QTextOption textOption;
            textOption.setWrapMode(QTextOption::WrapMode::WrapAnywhere);
            this->textEdit->document()->setDefaultTextOption(textOption);
        }else{
            QTextOption textOption;
            textOption.setWrapMode(QTextOption::NoWrap);
            this->textEdit->document()->setDefaultTextOption(textOption);
        }
    });

    font->setText("Шрифт...");
    connect(font,&QAction::triggered,[this](){
        bool status;
        QFont font = QFontDialog::getFont(&status, this->font, this);
        if(status){
            this->font = font;
            setZoom();
        }
    });

    auto format = new QMenu;
    format->setTitle("Формат");
    format->addAction(wordWrap);
    format->addAction(font);

    return format;
}
//menu Format

//menu View

QMenu * MainWindow::menuView(){
    auto zoomIn = new QAction(this);
    auto zoomOut = new QAction(this);
    auto zoomRestore = new QAction(this);

    zoomIn->setText("Увеличить");
    zoomIn->setShortcut(QKeySequence(QKeySequence::ZoomIn));
    connect(zoomIn,&QAction::triggered,[this](){
        zoom *=1.1;
        setZoom();
    });

    zoomOut->setText("Уменьшить");
    zoomOut->setShortcut(QKeySequence(QKeySequence::ZoomOut));
    connect(zoomOut,&QAction::triggered,[this](){
        zoom /=1.1;
        setZoom();
    });

    zoomRestore->setText("Восстановить масштаб по умолчанию");
    zoomRestore->setShortcut(QKeySequence("Ctrl+0"));
    connect(zoomRestore,&QAction::triggered,[this](){
        zoom = 1;
        setZoom();
    });

    auto scale = new QMenu(this);
    scale->setTitle("Масштаб");
    scale->addAction(zoomIn);
    scale->addAction(zoomOut);
    scale->addAction(zoomRestore);

    auto statusBar = new QAction(this);
    statusBar->setCheckable(true);
    statusBar->setText("Строка состояния");
    connect(statusBar,&QAction::triggered,[this](bool chehked){
        if(chehked){
            this->statusBar->show();
        }else{
            this->statusBar->hide();
        }
    });

    auto view = new QMenu;
    view->setTitle("Вид");
    view->addMenu(scale);
    view->addAction(statusBar);

    return view;
}

void MainWindow::setZoom(){
    labelZoom->setText(QString::number(zoom*100)+"%");
    auto font=this->font;
    font.setPointSize(static_cast<int>(font.pointSize()*zoom));
    this->textEdit->setFont(font);
}

//menu View


QMenu * MainWindow::menuReference(){
    auto about_program = new QAction(this);

    about_program->setText("О программе");
    connect(about_program,&QAction::triggered,[this](){
        this->aboutProgramDialog->exec();
    });

    auto reference = new QMenu;
    reference->setTitle("Справка");
    reference->addAction(about_program);

    return reference;
}

MainWindow::~MainWindow()
{
    delete pageSettings;
    delete findDialog;
};
