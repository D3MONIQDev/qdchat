#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

#include <QColor>
#include <QFont>
#include <QPointF>
#include <QStaticText>
#include <QString>



class MessageItem
{
private:
    int _itemType;
    QString _value;
    QPoint _itemPoint;
    QFont _font;
    QColor _color;
    QString _preValue;
    QString _postValue;
    QString _cssClassName;
    QStaticText _staticText;
    bool _wordWrap;
public:
    MessageItem(QWidget *parent, int itemType, QPoint point, QString value, QString pre="", QString post="", bool wordWrap=false, QString css="", QString fontFamily ="", QString colorHex="");
    MessageItem(QWidget *parent = 0);
    int itemType() const;
    void setItemType(int itemType);
    QString value();
    void setValue(QString &value);
    QFont font() const;
    void setFont(const QFont &font);
    QColor color() const;
    void setColor(const QColor &color);
    QString preValue() const;
    void setPreValue(const QString &preValue);
    QString postValue() const;
    void setPostValue(const QString &postValue);
    QString cssClassName() const;
    void setCssClassName(const QString &cssClassName);
    bool wordWrap() const;
    void setWordWrap(bool wordWrap);
    enum MessageItemType{
        Text,
        ServiceIcon
    };
    QString itemName() const;
    void setItemName(const QString &itemName);
    QPoint itemPoint() const;
    void setItemPoint(const QPoint &itemPoint);
    QStaticText staticText();
    void setStaticText( QStaticText &staticText);
    void setStaticTextWidth(int width);
};

#endif // MESSAGEITEM_H
