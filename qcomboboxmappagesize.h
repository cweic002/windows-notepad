#ifndef QCOMBOBOXMAPITEM_H
#define QCOMBOBOXMAPITEM_H

#include <QtWidgets>

class QComboBoxMapPageSize:public QComboBox{
    QMap<QString,QPageSize::PageSizeId> map;
public:
    QComboBoxMapPageSize(QWidget *);
    void addItem(const QString &,const QPageSize::PageSizeId &);
    QPageSize::PageSizeId currentQPageSize();
};

#endif // QCOMBOBOXMAPITEM_H
