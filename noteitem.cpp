#include "noteitem.h"
#include <QPen>



NoteItem::NoteItem(Note &note, QGraphicsItem *parent)
    : QGraphicsItem(parent), textItem(new QGraphicsTextItem(note.NoteNum(), this)),
    //   topLine(new QGraphicsLineItem(10, 0, note.Ori().length() * 20-5, 0, this)),
      bottomLine(new QGraphicsLineItem(5, 80, note.NoteNum().length() * 40, 80, this)),
      bbottomLine(new QGraphicsLineItem(5, 87, note.NoteNum().length() * 40, 87, this)),
      topdot(new QGraphicsEllipseItem(note.NoteNum().length() * 20, 0, 3, 3, this)),  // Adjust the size of the dot
      bottomdot(new QGraphicsEllipseItem(note.NoteNum().length() * 20+4, 95, 3, 3, this)) , // Adjust the size of the dot
    //   rdot(new QGraphicsEllipseItem(note.NoteNum().length() * 40+5, 57, 3, 3, this)),  // Adjust the size of the dot
      sharp(new QGraphicsTextItem("#", this)) , // Add this line
      b(new QGraphicsTextItem("b", this)) , // Add this line
      duration(note.Duration()),
      rdot(new QGraphicsEllipseItem(note.NoteNum().length() * 40+5, 57, 3, 3, this)),
      note(note),
      selected(false) 
{
    textItem->setFont(QFont("Arial", 50));  // Set the font and size

    QPen pen;  // Create a QPen object
    pen.setWidth(3);  // Set the width of the pen
    pen.setColor(Qt::black);  // Set the color of the pen

    // topLine->setPen(pen);  // Apply the pen to the topLine
    bottomLine->setPen(pen);  // Apply the pen to the bottomLine
    bbottomLine->setPen(pen);  // Apply the pen to the bottomLine
    topdot->setPen(pen);  // Apply the pen to the dot
    bottomdot->setPen(pen);  // Apply the pen to the dot
    rdot->setPen(pen);  // Apply the pen to the dot

    sharp->setPos(-7, -10);  // Set the position of the sharp
    sharp->setFont(QFont("Arial", 20));  // Set the font and size of the sharp
    sharp->setVisible(note.Ori().contains("#"));  // Make the sharp visible if the original note contains "#"

    b->setPos(-8, -5);  // Set the position of the sharp
    b->setFont(QFont("Arial", 20));  // Set the font and size of the sharp
    b->setVisible(note.Ori().contains("b"));  // Make the sharp visible if the original note contains "#"

    // topLine->setVisible(note.Ori().contains("^"));
    bottomLine->setVisible(note.Duration() >= 8 );
    bbottomLine->setVisible(note.Duration() >= 16 );
    topdot->setVisible(note.Ori().contains("^"));
    bottomdot->setVisible(note.Ori().contains("_"));
    rdot->setVisible(false);
    if (note.Ori().contains(".")) {
      if (this -> Duration() < 2) {
        rdot->setVisible(true);
      }
    }
    double test =  this->Duration();


    // Create duration lines
    for (int i = 0; i < this->Duration()-1; ++i) {
        QGraphicsLineItem *line = new QGraphicsLineItem(60 + i * 60+10, 40, 60 + (i+1) * 60-20, 40, this);
        line->setPen(pen);
        durationLines.append(line);
        line->setVisible(true);
    }
    
    
}


void NoteItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Set the selected flag
    selected = true;

    // Create a QInputDialog
    QInputDialog dialog;

    // Set the dialog's title and label
    dialog.setWindowTitle("Change Note");
    dialog.setLabelText("Enter the new pitch:");

    // Show the dialog and get the user's input
    if (dialog.exec() == QDialog::Accepted) {
        QString newPitch = dialog.textValue();

        // Change the note's pitch based on the user's input
        note.changePitch(newPitch);
    }

    dialog.setLabelText("Enter the new duration:");

    // Show the dialog and get the user's input
    if (dialog.exec() == QDialog::Accepted) {
        QString newDuration = dialog.textValue();

        // Change the note's duration based on the user's input
        note.changeDuration(newDuration.toDouble());
    }

    // Update the item to redraw it
    this->updateNote();
}

void NoteItem::updateNote()
{
    // Update the textItem's text based on the new pitch
    textItem->setPlainText(note.NoteNum());

    // Update the visibility and position of other items
    sharp->setVisible(note.Ori().contains("#"));
    b->setVisible(note.Ori().contains("b"));
    bottomLine->setVisible(note.Duration() >= 8 );
    bbottomLine->setVisible(note.Duration() >= 16 );
    topdot->setVisible(note.Ori().contains("^"));
    bottomdot->setVisible(note.Ori().contains("_"));
    rdot->setVisible(note.Ori().contains(".") && this->Duration() < 2);

    // Update the durationLines based on the new duration
    // First, remove all existing duration lines
    for (QGraphicsLineItem *line : durationLines) {
        delete line;
    }
    durationLines.clear();

    // Then, create new duration lines based on the new duration
    QPen pen;  // Create a QPen object
    pen.setWidth(3);  // Set the width of the pen
    pen.setColor(Qt::black);  // Set the color of the pen
    for (int i = 0; i < this->Duration()-1; ++i) {
        QGraphicsLineItem *line = new QGraphicsLineItem(60 + i * 60+10, 40, 60 + (i+1) * 60-20, 40, this);
        line->setPen(pen);
        durationLines.append(line);
        line->setVisible(true);
    }

    // Call the parent class's update method to redraw the item
    QGraphicsItem::update();
}

