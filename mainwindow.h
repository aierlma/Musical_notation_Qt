#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>
#include <QMediaPlayer>
#include "note.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QHboxLayout>
#include <QShortcut>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();


    void on_playbutton_clicked();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionDoc_triggered();

    void addNotesToScene();

    void on_UpdateNote_clicked();

    void on_updateNoteonScence_clicked();

    void on_saveNote_clicked();

    void on_playScene_clicked();

    void on_action8_16_16_triggered();

    void on_action16_16_8_triggered();

    void on_actionC_minor_triggered();

    void on_actionC_minor_2_triggered();

    void on_actionD_major_2_triggered();

    void on_actionD_minor_triggered();

    void on_actionE_major_2_triggered();

    void on_actionE_minor_triggered();

    void on_actionF_major_2_triggered();

    void on_actionF_minor_triggered();

    void on_actionG_major_2_triggered();

    void on_actionG_minor_triggered();

    void on_actionA_major_2_triggered();

    void on_actionA_minor_triggered();

    void on_actionB_major_2_triggered();

    void on_actionB_minor_triggered();

private:
    Ui::MainWindow *ui;
    QString currentFile;
    QShortcut *saveAsShortcut;
    QShortcut *saveShortcut;
private:
    QGraphicsView *view;
    QGraphicsScene *scene;
    QHBoxLayout *layout;


};


#endif // MAINWINDOW_H
