#pragma once
#include <QPushButton>
#include <QMenu>
#include <QMouseEvent>
#include <QString>

class WindowButton : public QPushButton {
    Q_OBJECT

public:
    WindowButton(const QString& windowTitle, const QString& windowId, QWidget *parent = nullptr);

signals:
    void requestClose(const QString& windowId);
    void requestPin(const QString& windowId);
    void requestFocus(const QString& windowId);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QString m_windowId;
    void showContextMenu(const QPoint& pos);
};
