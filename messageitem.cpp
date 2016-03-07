#include "messageitem.h"
#include <QDebug>
QPoint MessageItem::itemPoint() const
{
    return _itemPoint;
}

void MessageItem::setItemPoint(const QPoint &itemPoint)
{
    _itemPoint = itemPoint;
}

int MessageItem::itemType() const
{
    return _itemType;
}

void MessageItem::setItemType(int itemType)
{
    _itemType = itemType;
}

QString MessageItem::value()
{
    return _value;
}

void MessageItem::setValue(QString &value)
{
    _value = value;
}

QFont MessageItem::font() const
{
    return _font;
}

void MessageItem::setFont(const QFont &font)
{
    _font = font;
}

QColor MessageItem::color() const
{
    return _color;
}

void MessageItem::setColor(const QColor &color)
{
    _color = color;
}

QString MessageItem::preValue() const
{
    return _preValue;
}

void MessageItem::setPreValue(const QString &preValue)
{
    _preValue = preValue;
}

QString MessageItem::postValue() const
{
    return _postValue;
}

void MessageItem::setPostValue(const QString &postValue)
{
    _postValue = postValue;
}

QString MessageItem::cssClassName() const
{
    return _cssClassName;
}

void MessageItem::setCssClassName(const QString &cssClassName)
{
    _cssClassName = cssClassName;
}

bool MessageItem::wordWrap() const
{
    return _wordWrap;
}

void MessageItem::setWordWrap(bool wordWrap)
{
    _wordWrap = wordWrap;
}

QStaticText MessageItem::staticText()
{
    return _staticText;
}

void MessageItem::setStaticText( QStaticText &staticText)
{
    _staticText = staticText;
}

void MessageItem::setStaticTextWidth(int width)
{
    QStaticText tmpStaticText = QStaticText(this->value());
    tmpStaticText.setTextWidth(width);
    QTextOption textOption = QTextOption(Qt::AlignLeft);
    textOption.setWrapMode(QTextOption::WordWrap);
    tmpStaticText.setTextOption(textOption);
    setStaticText(tmpStaticText);
}

/**
 * @brief MessageItem::MessageItem
 * @param itemType
 * @param rect
 * @param value
 * @param pre
 * @param post
 * @param wordWrap
 * @param css
 * @param fontFamily
 * @param colorHex
 */
MessageItem::MessageItem(QWidget *parent, int itemType, QPoint point, QString value, QString pre, QString post, bool wordWrap, QString css, QString fontFamily, QString colorHex)
{
    setItemType(itemType);
    setItemPoint(point);
    setValue(value);
    setPreValue(pre);
    setPostValue(post);
    setWordWrap(wordWrap);
    setCssClassName(css);
    setFont(QFont(fontFamily));
    setColor(QColor(colorHex));

    _staticText = QStaticText(this->preValue()+this->value()+this->postValue());
//    _staticText = QStaticText(this->value());
     QTextOption opt;
     if(wordWrap){
        opt.setWrapMode(QTextOption::WordWrap);
        _staticText.setTextOption(opt);
     }
//    if(itemType != MessageItemType::ServiceIcon){
//        _staticText = QStaticText(pre+value+pro);
//    }
}

