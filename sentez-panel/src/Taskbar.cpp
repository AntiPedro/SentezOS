#include "Taskbar.h"
#include "WindowButton.h"
#include <QProcess>
#include <QDebug>

Taskbar::Taskbar(QWidget *parent) : QWidget(parent) {
    this->setFixedHeight(48); 
    
    // Glassmorphism Efekti
    this->setStyleSheet("background-color: rgba(20, 20, 25, 0.85); border-top: 1px solid rgba(255, 255, 255, 0.1);");

    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(10, 4, 10, 4);
    m_layout->setSpacing(8);

    // Başlat Menüsü İkonu
    QPushButton* startButton = new QPushButton("❄ Sentez", this);
    startButton->setStyleSheet("background-color: #3498db; color: white; border-radius: 6px; font-weight: bold; padding: 0 15px;");
    m_layout->addWidget(startButton);

    m_layout->addStretch();
    updateWindowList();
}

void Taskbar::updateWindowList() {
    // Gerçekte Hyprland IPC soketinden okunur. Şimdilik simüle ediyoruz.
    addWindowToTaskbar("0x55a3b2b", "Firefox Web");
    addWindowToTaskbar("0x55a3c4f", "Terminal");
}

void Taskbar::addWindowToTaskbar(const QString& windowId, const QString& title) {
    WindowButton* btn = new WindowButton(title, windowId, this);
    
    connect(btn, &WindowButton::requestClose, this, &Taskbar::handleWindowClose);
    connect(btn, &WindowButton::requestPin, this, &Taskbar::handleWindowPin);
    connect(btn, &WindowButton::requestFocus, this, &Taskbar::handleWindowFocus);

    m_layout->insertWidget(m_layout->count() - 1, btn);
}

void Taskbar::handleWindowClose(const QString& windowId) {
    qDebug() << "SentezOS WM -> Kapatma emri:" << windowId;
    QProcess::execute("hyprctl", {"dispatch", "closewindow", "address:" + windowId});
    if(auto btn = this->findChild<WindowButton*>(windowId)) {
        btn->deleteLater();
    }
}

void Taskbar::handleWindowFocus(const QString& windowId) {
    qDebug() << "SentezOS WM -> Odağa alınıyor:" << windowId;
    QProcess::execute("hyprctl", {"dispatch", "focuswindow", "address:" + windowId});
}

void Taskbar::handleWindowPin(const QString& windowId) {
    qDebug() << "SentezOS Cloud -> Ayar kaydediliyor. Pinlendi:" << windowId;
}
