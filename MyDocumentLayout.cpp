#include "MyDocumentLayout.h"

MyDocumentLayout::MyDocumentLayout(QTextDocument * document) :
    QAbstractTextDocumentLayout(document),
    _header_Height(50),_footer_Height(50),_size(document->size())
{
    _size.setHeight(_size.height() + _header_Height + _footer_Height);
}
void MyDocumentLayout::draw(QPainter *painter, const QAbstractTextDocumentLayout::PaintContext &context)
{
    // вызываем метод верхнего класса, чтобы нарисовать содержимое документа
    //QAbstractTextDocumentLayout::draw(painter, context);

    // рисуем верхний колонтитул
    QRectF headerRect(0, 0, documentSize().width(), _header_Height);
    QString headerText = "Верхний колонтитул";
    painter->drawText(headerRect, Qt::AlignCenter, headerText);

    // рисуем нижний колонтитул
    QRectF footerRect(0, documentSize().height() - _footer_Height, documentSize().width(), _footer_Height);
    QString footerText = "Нижний колонтитул";
    painter->drawText(footerRect, Qt::AlignCenter, footerText);
}

QSizeF MyDocumentLayout::documentSize() const
{
    return _size;
}


int MyDocumentLayout::pageCount() const
{
    return 1;//QAbstractTextDocumentLayout::pageCount();
}

void MyDocumentLayout::resizeInlineObject(QTextInlineObject item, int posInDocument, const QTextFormat &format)
{
    QAbstractTextDocumentLayout::resizeInlineObject(item, posInDocument, format);
}

QPaintDevice *MyDocumentLayout::paintDevice() const
{
    return this->QAbstractTextDocumentLayout::paintDevice();
}
