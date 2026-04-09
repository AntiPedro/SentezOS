#include <QApplication>
#include <LayerShellQt/Window>
#include "Taskbar.h"

int main(int argc, char *argv[]) {
    // SentezOS için Wayland platformunu zorla
    qputenv("QT_QPA_PLATFORM", "wayland");

    QApplication app(argc, argv);
    app.setApplicationName("SentezOS Panel");

    Taskbar taskbar;
    
    // Qt Layer Shell Entegrasyonu (Wayland Paneli)
    if (LayerShellQt::Window *layerWindow = LayerShellQt::Window::get(&taskbar)) {
        layerWindow->setLayer(LayerShellQt::Window::LayerTop);
        layerWindow->setAnchors(LayerShellQt::Window::AnchorBottom |
                                LayerShellQt::Window::AnchorLeft |
                                LayerShellQt::Window::AnchorRight);
        layerWindow->setExclusiveZone(taskbar.height()); 
    }

    taskbar.show();
    return app.exec();
}
