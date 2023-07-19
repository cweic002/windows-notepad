#include "mainwindow.h"
#include <QtGlobal>
#include <QtCore>
#include <QtAlgorithms>
#include <State.h>

#ifdef QT_DEBUG
void messageToFile(QtMsgType type,
                   const QMessageLogContext& context,
                   const QString& msg)
{
    QFile file("protocol.log");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        return;
    QString strDateTime = QDateTime::currentDateTime().toString("hh:mm-dd.MM.yyyy");
    QTextStream out(&file);
    switch(type){
        case QtDebugMsg:{
            out << strDateTime <<" Debug: " << msg
            << ", " << context.file << endl;
            break;
        }
        case QtWarningMsg:{
            out << strDateTime << "Warning: " << msg
            << ", " << context.file << endl;
            break;
        }
        case QtCriticalMsg:{
            out << strDateTime << "Critical: " << msg
            << ", " << context.file << endl;
            break;
        }
        case QtFatalMsg:{
            out << strDateTime << "Fatal: " << msg
            << ", " << context. file << endl;
            std::terminate();
        }
        default:{
            break;
        }
    }
}
#endif

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    #ifdef QT_DEBUG
        //qInstallMessageHandler(messageToFile);
        qDebug() << "start program";
    #endif
    MainWindow mainWindow;
    mainWindow.show();
    return application.exec();
}
