#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("CodeEditorWidgets");
    app.setOrganizationName("MyCompany");

    MainWindow window;
    window.show();

    return app.exec();
}