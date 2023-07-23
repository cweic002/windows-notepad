#ifndef PAGESETTINGS_H
#define PAGESETTINGS_H
#include "State.h"
#include <QtWidgets>

class PageSettings : public QDialog
{
    Q_OBJECT
    //Paper setting
    struct Page{
        QGroupBox * groupBox;
        QComboBox * comboBoxSize;
        QComboBox * comboBoxSupply;
        Page();
    }page;
    //Paper setting

    struct View{
        QGroupBox * groupBox;
        QFrame * frame;
        QTextBrowser * textBrowser;
        View();
    }view;

    struct Orientation{
        QGroupBox * groupBox;
        QButtonGroup * buttonGroup;
        QRadioButton * radioButtonPortrait;
        Orientation();
    };

    struct Padding{
        QGroupBox * groupBox;
        struct Position{
            QLabel *label;
            QSpinBox *spinBox;
            Position(QString labelName);
        }left,top,right,bottom;
        Padding();
    }padding;
    struct Title{
        QLabel * label;
        QLineEdit * lineEdit;
        Title(QString name);
    }header,footer;
    QDialogButtonBox * buttonBox;
    State state;
private slots:
    void setStyleTextBrowser(int value);
public:
    QPageLayout::Orientation orientation;
    explicit PageSettings(QWidget *parent = nullptr);
    QPageSize::PageSizeId size();
    QMargins margins();
};

#endif // PAGESETTINGS_H
