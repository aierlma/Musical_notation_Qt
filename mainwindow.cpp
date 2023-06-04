#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <SFML/Audio.hpp>
#include <cmath>
#include "dialog.h"
#include "note.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QShortcut>
#include "noteitem.h"
#include "barlineitem.h"
#include <QTextStream>

// 部分仿自Qt官网教程


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize QGraphicsScene
    scene = new QGraphicsScene(this);

    // Set the QGraphicsScene to QGraphicsView
    ui->graphicsView->setScene(scene);

    // Connect the clicked signal of the update notes button to the on_updateNotesButton_clicked method
    connect(ui->UpdateNote, &QPushButton::clicked, this, &MainWindow::on_UpdateNote_clicked);  

        // 创建快捷键对象
    saveShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_S), this); // 设置快捷键为Ctrl+S
    saveAsShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_M), this); // 设置快捷键为Ctrl+M

    // 连接快捷键对象和槽函数
    connect(saveShortcut, &QShortcut::activated, this, &MainWindow::on_actionSave_triggered); // 保存（save）
    connect(saveAsShortcut, &QShortcut::activated, this, &MainWindow::on_actionSave_as_triggered); // 另存为（save as）
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNew_triggered()
{
        currentFile.clear();
        ui->textEdit->setText(QString());

}


void MainWindow::on_actionOpen_triggered()
{
        QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
        if (fileName.isEmpty())
            return;
        QFile file(fileName);
        currentFile = fileName;
        if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
            return;
        }
        setWindowTitle(fileName);
        QTextStream in(&file);
        QString text = in.readAll();
        ui->textEdit->setText(text);
        file.close();
}





void MainWindow::on_actionSave_triggered()
{

    QString fileName;
    // If we don't have a filename from before, get one.
    if (currentFile.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, "Save");
        if (fileName.isEmpty())
            return;
        currentFile = fileName;
    } else {
        fileName = currentFile;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();

}


void MainWindow::on_actionSave_as_triggered()
{

        QString fileName = QFileDialog::getSaveFileName(this, "Save as");
        if (fileName.isEmpty())
            return;
        QFile file(fileName);

        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
            return;
        }
        currentFile = fileName;
        setWindowTitle(fileName);
        QTextStream out(&file);
        QString text = ui->textEdit->toPlainText();
        out << text;
        file.close();

}



void MainWindow::on_playbutton_clicked()
{
    QString line = ui->textEdit->toPlainText();
    QStringList notes = line.split(' ');
    for (QString noteStr : notes) {
        Note * n = new Note(noteStr);
        n->play();
        delete n;

    }
}

void MainWindow::on_actionDoc_triggered()
{

    std::string document = "你可以输入1-7作为简谱的音符。\n\n"
                           "在数字之前加入#表示升半音，加b表示降半音。\n\n"
                           "例如，#1代表升半音的中央C#4，b2表示Db4。\n\n"
                           "在数字之前加上 _ 或者 ^ 表示低音或者高音音符，分别比不加前缀的低八度或低八度。\n\n"
                            "例如，1代表中央C4，_1代表低八度C3，^1代表高八度C5。\n\n"
                            "例如，^#1代表高八度升半音的中央C#5。\n\n"
                            "你可以输入0作为休止符，休止符的时长和音符一样。\n\n"
                            "可以通过在数字后面加入 - 和数字来表示音符的时长，例如1-2代表中央C4的二分音符。\n\n"
                            "默认为四分音符，即 1和1-4是一样的，4-1表示四分音符的四倍时长，一分音符，2-16表示十六分音符。\n\n"
                            "你可以输入多个音符，用空格隔开，例如 1 2 3 4 5 6 7 1 7 6 5 4 3 2 1。\n\n"
                            "你可以通过输入加号 + 来表示连音，例如 1-2+1-4表示一拍半的音符。\n\n"
                            "你可以通过在音符后加入 . 表示附点音符，例如 1-2.表示二分音符的附点音符，共三拍。\n\n"
                            "你可以通过点击播放按钮来播放你输入的音符。点击updateNote 在右侧显示音符\n\n"
                            "一个示例是5-8 6-8 7-8 5-8 ^2-4+^2-8 7-8 6 ^2 6 5-8 3-8 7-4+7-8 5-8 #4-2+#4-4。";
    DocumentationDialog dialog(this);
    dialog.setDocumentText(document.c_str());
    dialog.exec();


}


void MainWindow::on_UpdateNote_clicked()
{
    addNotesToScene();
}



void MainWindow::addNotesToScene()
{
    // Clear the scene
    scene->clear();

    // Get the notes from the text editor
    QString line = ui->textEdit->toPlainText();
    QStringList notes = line.split(' ');

    // Initialize the x and y position
    int x = 0;
    int y = 0;

    // The width of each note item
    int noteWidth = 60;

    // The height of each note item
    int noteHeight = 120;

    // The width of the view
    int viewWidth = ui->graphicsView->width();

    // The beat and bar
    double beat = 4;  // Four quarter notes per beat
    double bar = 3.99;  // Three beats per bar
    double currentBeat = 0.0;  // Current beat count

    // Add each note to the scene
    for (int i = 0; i < notes.size(); ++i) {
        QString noteStr = notes[i];
        Note *note = new Note(noteStr);


        // Create a NoteItem
        NoteItem *noteItem = new NoteItem(*note);

        // Set the position of the NoteItem
        noteItem->setPos(x, y);

        // Add the NoteItem to the scene
        scene->addItem(noteItem);
        x += (noteItem->Duration()-1)*noteWidth;

        // Update the current beat count
        currentBeat +=  beat / note->Duration();

        
        // Update the x position for the next note
        x += noteWidth;
        // If the current beat count reaches the beat, add a bar line
        if (currentBeat >= bar) {
            BarLineItem *barLineItem = new BarLineItem();
            barLineItem->setPos(x, y);
            scene->addItem(barLineItem);
            currentBeat = 0.0;  // Reset the current beat count
        }
        // If the next note will be out of the view, reset x and increase y
        if (x + noteWidth > viewWidth) {
            x = 0;
            y += noteHeight;
        }
    }


}


void MainWindow::on_updateNoteonScence_clicked()
{
    // Get all items in the scene
    QList<QGraphicsItem *> items = scene->items();

    // Reverse the list to keep the original order of the notes
    std::reverse(items.begin(), items.end());

    // Initialize the x and y position
    int x = 0;
    int y = 0;

    // The width of each note item
    int noteWidth = 60;

    // The height of each note item
    int noteHeight = 120;

    // The width of the view
    int viewWidth = ui->graphicsView->width();

    // The beat and bar
    double beat = 4;  // Four quarter notes per beat
    double bar = 3.99;  // Three beats per bar
    double currentBeat = 0.0;  // Current beat count

    // Iterate over all items
    for (QGraphicsItem *item : items) {
        BarLineItem *barLineItem = dynamic_cast<BarLineItem *>(item);
        if (barLineItem) {
            // Remove the BarLineItem from the scene
            scene->removeItem(barLineItem);
            // Delete the BarLineItem
            delete barLineItem;
            continue;
        }
        // Check if the item is a NoteItem
        NoteItem *noteItem = dynamic_cast<NoteItem *>(item);
        if (noteItem) {
            // Update the NoteItem
            noteItem->updateNote();

            // Set the position of the NoteItem
            noteItem->setPos(x, y);

            x += (noteItem->Duration()-1)*noteWidth;

            // Update the current beat count
            currentBeat +=  beat / noteItem->note.Duration();

            // Update the x position for the next note
            x += noteWidth;
            // If the current beat count reaches the beat, add a bar line
            if (currentBeat >= bar) {
                BarLineItem *barLineItem = new BarLineItem();
                barLineItem->setPos(x, y);
                scene->addItem(barLineItem);
                currentBeat = 0.0;  // Reset the current beat count
            }
            // If the next note will be out of the view, reset x and increase y
            if (x + noteWidth > viewWidth) {
                x = 0;
                y += noteHeight;
            }
        }
    }
}



void MainWindow::on_saveNote_clicked()
{
    // Get the file name from the user
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("PNG Image (*.png);;JPEG Image (*.jpg *.jpeg)"));

    if (fileName.isEmpty()) {
        return;
    }

    // Create a QImage object with the same size as the scene
    QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::white);  // Fill the image with white

    // Create a QPainter object and render the scene to the image
    QPainter painter(&image);
    scene->render(&painter);

    // Save the image to the file
    image.save(fileName);
}


void MainWindow::on_playScene_clicked()
{
    // Get all items in the scene
    QList<QGraphicsItem *> items = scene->items();

    // Reverse the list to keep the original order of the notes
    std::reverse(items.begin(), items.end());
    // Iterate over all items
    for (QGraphicsItem *item : items) {
        // Check if the item is a NoteItem
        NoteItem *noteItem = dynamic_cast<NoteItem *>(item);
        if (noteItem) {
            // Play the note
            noteItem->note.play();
        }
    }
}


void MainWindow::on_action8_16_16_triggered()
{
    QString rhythmCode = "1-8 2-16 3-16";
    QString currentText = ui->textEdit->toPlainText();
    ui->textEdit->setPlainText(currentText + " " + rhythmCode);
}


void MainWindow::on_action16_16_8_triggered()
{
    QString rhythmCode = "1-16 2-16 3-8";
    QString currentText = ui->textEdit->toPlainText();
    ui->textEdit->setPlainText(currentText + " " + rhythmCode);
}


void MainWindow::on_actionC_minor_triggered()
{
    QString melodyCode = "1 2 3 4 5 6 7 ^1";
    QString currentText = ui->textEdit->toPlainText();
    ui->textEdit->setPlainText(currentText + melodyCode);
}


void MainWindow::on_actionC_minor_2_triggered()
{
    QString melodyCode = "1 2 b3 4 5 b6 b7 ^1";
    QString currentText = ui->textEdit->toPlainText();
    ui->textEdit->setPlainText(currentText + melodyCode);
}


void MainWindow::on_actionD_major_2_triggered()
{
    QString melodyCode = "2 3 #4 5 6 7 ^#1 ^2";
    QString currentText = ui->textEdit->toPlainText();
    ui->textEdit->setPlainText(currentText + melodyCode);
}


void MainWindow::on_actionD_minor_triggered()
{
    QString melodyCode = "2 3 4 5 6 b7 ^1 ^2";
    QString currentText = ui->textEdit->toPlainText();
    ui->textEdit->setPlainText(currentText + melodyCode);
}


void MainWindow::on_actionE_major_2_triggered()
{
    QString melodyCode = "3 #4 #5 6 7 ^#1 ^#2 ^3";
    QString currentText = ui->textEdit->toPlainText();
    ui->textEdit->setPlainText(currentText + melodyCode);
}


void MainWindow::on_actionE_minor_triggered()
{
    QString melodyCode = "3 #4 5 6 7 ^1 ^2 ^3";
    QString currentText = ui->textEdit->toPlainText();
    ui->textEdit->setPlainText(currentText + melodyCode);
}


void MainWindow::on_actionF_major_2_triggered()
{
    QString melodyCode = "4 5 6 b7 ^1 ^2 ^3 ^4";
    QString currentText = ui->textEdit->toPlainText();
    ui->textEdit->setPlainText(currentText + melodyCode);
}


void MainWindow::on_actionF_minor_triggered()
{
    QString melodyCode = "4 5 b6 b7 ^1 ^b2 ^b3 ^4";
    QString currentText = ui->textEdit->toPlainText();
    ui->textEdit->setPlainText(currentText + melodyCode);
}


void MainWindow::on_actionG_major_2_triggered()
{
    QString melodyCode = "5 6 7 ^1 ^2 ^3 ^#4 ^5";
    QString currentText = ui->textEdit->toPlainText();
    ui->textEdit->setPlainText(currentText + melodyCode);
}


void MainWindow::on_actionG_minor_triggered()
{
    QString melodyCode = "5 6 b7 ^1 ^2 ^b3 ^4 ^5";
    QString currentText = ui->textEdit->toPlainText();
    ui->textEdit->setPlainText(currentText + melodyCode);
}


void MainWindow::on_actionA_major_2_triggered()
{
    QString melodyCode = "6 7 ^#1 ^2 ^3 ^#4 ^#5 ^6";
    QString currentText = ui->textEdit->toPlainText();
    ui->textEdit->setPlainText(currentText + melodyCode);
}


void MainWindow::on_actionA_minor_triggered()
{
    QString melodyCode = "6 7 ^1 ^2 ^3 ^4 ^5 ^6";
    QString currentText = ui->textEdit->toPlainText();
    ui->textEdit->setPlainText(currentText + melodyCode);
}


void MainWindow::on_actionB_major_2_triggered()
{
    QString melodyCode = "7 ^#1 ^#2 ^3 ^#4 ^#5 ^#6 ^7";
    QString currentText = ui->textEdit->toPlainText();
    ui->textEdit->setPlainText(currentText + melodyCode);
}


void MainWindow::on_actionB_minor_triggered()
{
    QString melodyCode = "7 ^#1 ^2 ^3 ^#4 ^5 ^6 ^7";
    QString currentText = ui->textEdit->toPlainText();
    ui->textEdit->setPlainText(currentText + melodyCode);
}

