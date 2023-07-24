#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "iFille.h"
#include "findDialog.h"
#include "pagesettings.h"
#include "lineGoDialog.h"
#include "aboutProgramDialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QTextEdit * textEdit;
    QAction * goLine;
    QFont font;
    double zoom;
    QLabel * labelPagePosition;
    QLabel * labelZoom;

    QStatusBar * statusBar;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void file_create_triggered();
    void file_newWindow_triggered();
    void file_open_triggered();
    void file_seve_triggered();
    void file_seve_as_triggered();
    void file_print_triggered();
private:
    QMenu * menuFile();
    QMenu * menuCorrection();
    QMenu * menuFormat();
    QMenu * menuView();
    QMenu * menuReference();
    void setZoom();
    void labelPagePositionUpdate();
    std::unique_ptr<ap::IFile> file;
    FindDialog * findDialog;
    PageSettings * pageSettings;
    LineGoDialog * lineGoDialog;
    AboutProgramDialog * aboutProgramDialog;
};
#endif // MAINWINDOW_H
