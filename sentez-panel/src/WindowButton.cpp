#include "WindowButton.h"
#include <QAction>
#include <QIcon>

WindowButton::WindowButton(const QString& windowTitle, const QString& windowId, QWidget *parent)
    : QPushButton(windowTitle, parent), m_windowId(windowId) {
    this->setObjectName(windowId);
    this->setMinimumHeight(40);
    this->setMinimumWidth(150);
    this->setStyleSheet("QPushButton { background-color: rgba(255,255,255,0.05); color: white; border-radius: 6px; padding: 5px; border: 1px solid rgba(255,255,255,0.1); }"
                        "QPushButton:hover { background-color: rgba(255,255,255,0.15); }");
}

void WindowButton::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        showContextMenu(event->globalPosition().toPoint());
    } else if (event->button() == Qt::LeftButton) {
        emit requestFocus(m_windowId);
        QPushButton::mousePressEvent(event); 
    }
}

void WindowButton::showContextMenu(const QPoint& pos) {
    QMenu contextMenu(this);
    contextMenu.setStyleSheet(
        "QMenu { background-color: rgba(30, 30, 35, 230); color: white; border-radius: 8px; border: 1px solid #444; }"
        "QMenu::item:selected { background-color: #3498db; border-radius: 4px; }"
    );

    QAction *pinAction = contextMenu.addAction("📌 Sabitle (Pin)");
    QAction *focusAction = contextMenu.addAction("🪟 Öne Getir");
    contextMenu.addSeparator(); 
    QAction *closeAction = contextMenu.addAction("❌ Kapat (Kill)");

    connect(pinAction, &QAction::triggered, this, [this]() { emit requestPin(m_windowId); });
    connect(focusAction, &QAction::triggered, this, [this]() { emit requestFocus(m_windowId); });
    connect(closeAction, &QAction::triggered, this, [this]() { emit requestClose(m_windowId); });

    contextMenu.exec(pos);
}
