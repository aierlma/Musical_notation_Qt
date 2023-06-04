#ifndef BARLINEITEM_H
#define BARLINEITEM_H
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QPen>

class BarLineItem : public QGraphicsLineItem
{
public:
    BarLineItem(QGraphicsItem *parent = nullptr)
        : QGraphicsLineItem(-5, 0, -5, 110, parent)
    {
        QPen pen;  // Create a QPen object
        pen.setWidth(3);  // Set the width of the pen
        pen.setColor(Qt::black);  // Set the color of the pen

        setPen(pen);  // Apply the pen to the barLine
    }
    ~BarLineItem() = default;

};
#endif // BARLINEITEM_H
