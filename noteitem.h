#ifndef NOTEITEM_H
#define NOTEITEM_H
#include <QGraphicsItem>
#include <QFont>
#include "note.h"
#include <QInputDialog>

class NoteItem : public QGraphicsItem
{
public:
    Note &note;
    NoteItem(Note &note, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override
    {
        return childrenBoundingRect();
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        Q_UNUSED(painter)
        Q_UNUSED(option)
        Q_UNUSED(widget)
    }
    double Duration() const {
        if (note.Duration() <= 1) {
            return 4;
        }else if(note.Duration() <= 1.5) {
            return 3;
        }else if (note.Duration() <= 2) {
            return 2;
        }else  {
            return 1;
        }
    }
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void updateNote();

private:
    QGraphicsTextItem *textItem;
    QGraphicsLineItem *bottomLine;
    QGraphicsLineItem *bbottomLine;
    QGraphicsEllipseItem *topdot; 
    QGraphicsEllipseItem *bottomdot;
    QGraphicsEllipseItem *rdot;
    QGraphicsTextItem *sharp;
    QGraphicsTextItem *b;
    QList<QGraphicsLineItem *> durationLines;  
    double duration;
    bool selected;
    
};



#endif // NOTEITEM_H
