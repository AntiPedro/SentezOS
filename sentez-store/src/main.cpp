#include <QApplication>
#include "StoreWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("SentezOS App Store");

    // SentezOS modern koyu teması
    app.setStyleSheet("QWidget { background-color: #121212; color: #FFFFFF; font-family: 'Inter'; }"
                      "QPushButton { background-color: #1E88E5; border-radius: 6px; padding: 6px; font-weight: bold; }");

    StoreWindow window;
    window.resize(1024, 768);
    window.show();

    return app.exec();
}
