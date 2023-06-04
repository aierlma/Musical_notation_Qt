#include "mainwindow.h"
#include "welcomedialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    WelcomeDialog welcomeDialog;
    if (welcomeDialog.exec() == QDialog::Accepted) {
        // User clicked "Start Creating", open the main window
        MainWindow mainWindow;
        mainWindow.show();
        return app.exec();
    } else {
        // User closed the welcome dialog without clicking "Start Creating", exit the application
        return 0;
    }
}
    